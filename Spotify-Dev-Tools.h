#include <windows.h>
#include <psapi.h>
#undef UNICODE
#include <tlhelp32.h>
#define UNICODE
#include <iostream>
#include <tchar.h>
#include <string>
#include <unordered_map>

#pragma comment(lib, "version.lib")

uintptr_t SpotifyVersionMap(std::string x)
{
	std::unordered_map<std::string, int> version;

	version.insert(std::pair<std::string, int>("1.1.80.699", 0x16525FD));
	version.insert(std::pair<std::string, int>("1.1.81.598", 0x170273E));
	version.insert(std::pair<std::string, int>("1.1.81.604", 0x170B73E));
	version.insert(std::pair<std::string, int>("1.1.82.754", 0x16E5A7E));
	version.insert(std::pair<std::string, int>("1.1.82.758", 0x16E4A7E));
	version.insert(std::pair<std::string, int>("1.1.82.760", 0x170BAFE));
	version.insert(std::pair<std::string, int>("1.1.83.944", 0x16AAC3D));
	version.insert(std::pair<std::string, int>("1.1.83.954", 0x16AAC3D));
	version.insert(std::pair<std::string, int>("1.1.83.956", 0x16ACC3D));
	version.insert(std::pair<std::string, int>("1.1.84.716", 0x16A597C));
	version.insert(std::pair<std::string, int>("1.1.85.884", 0x16ACCBC));
	version.insert(std::pair<std::string, int>("1.1.85.895", 0x16B7C7C));
	version.insert(std::pair<std::string, int>("1.1.86.857", 0x16EFC3D));
	version.insert(std::pair<std::string, int>("1.1.87.612", 0x1700ABD));
	version.insert(std::pair<std::string, int>("1.1.88.595", 0x17121C5));
	version.insert(std::pair<std::string, int>("1.1.88.612", 0x17131C5));
	version.insert(std::pair<std::string, int>("1.1.89.854", 0x16F9C86));
	version.insert(std::pair<std::string, int>("1.1.89.858", 0x16FBC86));
	version.insert(std::pair<std::string, int>("1.1.89.862", 0x16FCC86));
	version.insert(std::pair<std::string, int>("1.1.90.855", 0x16F1C7E));
	version.insert(std::pair<std::string, int>("1.1.90.859", 0x1700C86));
	version.insert(std::pair<std::string, int>("1.1.91.824", 0x16E0805));
	version.insert(std::pair<std::string, int>("1.1.92.644", 0x16DDE45));
	version.insert(std::pair<std::string, int>("1.1.92.647", 0x16DDE45));
	version.insert(std::pair<std::string, int>("1.1.93.896", 0x16F2405));
	version.insert(std::pair<std::string, int>("1.1.94.864", 0x16F5305));
	version.insert(std::pair<std::string, int>("1.1.94.870", 0x16FA305));
	version.insert(std::pair<std::string, int>("1.1.94.872", 0x16F5505));
	version.insert(std::pair<std::string, int>("1.1.95.889", 0x16F5505));
	version.insert(std::pair<std::string, int>("1.1.95.893", 0x1706F86));
	version.insert(std::pair<std::string, int>("1.1.96.783", 0x171D2FE));
	version.insert(std::pair<std::string, int>("1.1.96.785", 0x17152FE));
	version.insert(std::pair<std::string, int>("1.1.97.956", 0x171A8BE));
	version.insert(std::pair<std::string, int>("1.1.97.962", 0x17148BE));
	version.insert(std::pair<std::string, int>("1.1.98.683", 0x170FA7E));
	version.insert(std::pair<std::string, int>("1.1.98.691", 0x173CA7E));

	std::unordered_map<std::string, int>::iterator xe = version.find(x);
	return ((xe != version.end()) ? xe->second : 0);
}

void clear_screen()
{
	// code excerpt from:
	// https://social.msdn.microsoft.com/Forums/en-US/1e493962-663e-404c-b951-5e9507cbb279/problems-with-switch?forum=Vsexpressvc

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { 0, 0 };
	DWORD count;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hStdOut, &csbi);
	FillConsoleOutputCharacterA(hStdOut, ' ', csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
	SetConsoleCursorPosition(hStdOut, coord);
}

HANDLE GetProcessInfo(const char* processName, char** processPath)
{
	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(entry);
	char exePath[MAX_PATH];

	do {
		if (!strcmp(entry.szExeFile, processName)) {
			DWORD targetID = entry.th32ProcessID;
			HANDLE targetProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_ALL_ACCESS, false, targetID);
			if (targetProcess != NULL) {
				if (GetModuleFileNameExA(targetProcess, NULL, exePath, MAX_PATH) != 0) {
					*processPath = _strdup(exePath);
					CloseHandle(handle);
					return targetProcess;
				} else {
					CloseHandle(targetProcess);
				}
			}
		}
	} while (Process32Next(handle, &entry));

	CloseHandle(handle);
	return 0;
}

char* GetProcessVersion(char* processPath)
{
	DWORD dwHandle, dwLen;
	UINT bufLen;
	LPSTR lpData = NULL, lpBuffer = NULL, libName;
	VS_FIXEDFILEINFO* pFileInfo;

	if (processPath == NULL) {
		// Beep Boop Beep, Wrong Turn, Return To Safety Before Armageddon
		return NULL;
	}
	libName = processPath;
	dwLen = GetFileVersionInfoSizeA(libName, &dwHandle);
	if (dwLen) {
		lpData = (LPSTR)malloc(dwLen);
		if (lpData) {
			if (GetFileVersionInfoA(libName, dwHandle, dwLen, lpData)) {
				if (VerQueryValueA(lpData, "\\", (LPVOID*)&pFileInfo, (PUINT)&bufLen)) {
					lpBuffer = (LPSTR)malloc(3 * sizeof(DWORD));
					sprintf_s(lpBuffer, (3 * sizeof(DWORD)), "%d.%d.%d.%d", HIWORD(pFileInfo->dwFileVersionMS), LOWORD(pFileInfo->dwFileVersionMS), HIWORD(pFileInfo->dwFileVersionLS), LOWORD(pFileInfo->dwFileVersionLS));
				}
			}
		}
	}
	free(lpData);
	return lpBuffer;
}