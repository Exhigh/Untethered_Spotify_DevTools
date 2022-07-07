/*	
  @initial author: ririxidev <dev@ririxi.dev>
  @author: Exhigh <exhigh01@gmail.com> 
*/
#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <conio.h>
#include <tlhelp32.h>
#include <vector.h>
#include <string.h>
#include <tchar.h>
#include <map.h>

#pragma comment(lib, "psapi.lib")

int SpotifyVersionMapSearch(std::string x)
{
	std::map<std::string, int> version;
	
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
	
	return version.find(x)->second;
}

HANDLE GetProcessInfo(char* processName, char **processPath)
{
	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(entry);
	char exePath[MAX_PATH];

	do {
		if (!strcmp(entry.szExeFile, processName)) {
			DWORD targetID = entry.th32ProcessID;
			CloseHandle(handle);
			HANDLE TargetProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_ALL_ACCESS, false, targetID);
			if (TargetProcess != NULL) {
				if (GetModuleFileNameEx(TargetProcess, NULL, exePath, MAX_PATH) != 0) {
					*processPath = strdup(exePath);
					return TargetProcess;
				} else {
					CloseHandle(TargetProcess);
				}
			}
		}
	}
	while (Process32Next(handle, &entry));

	return 0;
}

char* GetProcessVersion (char* processPath)
{
	DWORD dwHandle, dwLen;
	UINT BufLen;
	LPTSTR lpData, lpBuffer = NULL, LibName;
	VS_FIXEDFILEINFO* pFileInfo;
	
	if (processPath == NULL) {
		// Beep Boop Beep, Wrong Turn, Return To Safety Before Armageddon
		return NULL;
	}
	LibName = processPath;
	dwLen = GetFileVersionInfoSize (LibName, &dwHandle);
	if (dwLen) {
		lpData = (LPTSTR) malloc (dwLen);
		if (lpData) {
			if (GetFileVersionInfo (LibName, dwHandle, dwLen, lpData)) {
				if (VerQueryValue (lpData, "\\", (LPVOID*) &pFileInfo, (PUINT) &BufLen)) {
					lpBuffer = (LPTSTR) malloc(sizeof(pFileInfo));
					sprintf(lpBuffer,"%d.%d.%d.%d", HIWORD(pFileInfo->dwFileVersionMS),LOWORD(pFileInfo->dwFileVersionMS),HIWORD(pFileInfo->dwFileVersionLS),LOWORD(pFileInfo->dwFileVersionLS));
				}
			}
		}
	}
	free (lpData);
	return lpBuffer;
}

int main()
{

	HANDLE process;
	std::string dummy, spotifyVersion;
	int address = 0, fail = 0;
	char *processVersion = NULL, *spotifyPath = NULL;
	Retry:
	std::cout << "[!] This program works only for Spotify ^_^ "<<std::endl;
	process = GetProcessInfo("Spotify.exe",&spotifyPath);
	if(!process) {
		fail++;
		std::cerr << "[-] Can't hook up to process (Is Spotify open?)." << std::endl;
		std::cout<<"[!] Open Spotify And Hit Any Key To Continue" << std::endl;
		std::cout<<"\nNumber of Times, Dev Tools Failed To Be Activated: "<<fail<<std::endl;
		std::getline(std::cin, dummy);
		clrscr();
		goto Retry;
	}
	processVersion = GetProcessVersion(*&spotifyPath);
	if (processVersion == NULL) {
		// Clearly This Wouldn't Have Ended Well, Let's Retry
		clrscr();
		goto Retry;
	}
	spotifyVersion = (char*) malloc(sizeof(processVersion) + 1); 
	spotifyVersion = processVersion;
	std::cout<<"Current Spotify Version - "<<spotifyVersion<<std::endl;

	address = SpotifyVersionMapSearch(spotifyVersion);

	int targetValue = 255;
	int result = WriteProcessMemory(process, (LPVOID*)address, &targetValue, (DWORD)sizeof(targetValue), NULL);
	if (result == 0x00 || address == 0) {
		std::cerr << "[-] There was some error, try again later or open an issue, with the Spotify Version!" << std::endl;
	} else {
		std::cout<< "[+] Developer mode has been successfully enabled."<<std::endl;
	}
	std::cout<<"[!] Press Any Key To Exit"<<std::endl;
	std::getline(std::cin, dummy);
	return 0;
}