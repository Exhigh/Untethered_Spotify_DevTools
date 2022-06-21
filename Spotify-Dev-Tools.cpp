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

#pragma comment(lib, "psapi.lib")

//(TODO) Find A Better Way To Store These ?
std::string version15 = "1.1.88.612";
std::string version14 = "1.1.88.595";
std::string version13 = "1.1.87.612";
std::string version12 = "1.1.86.857";
std::string version11 = "1.1.85.895";
std::string version10 = "1.1.85.884";
std::string version9 = "1.1.84.716";
std::string version8 = "1.1.83.956";
std::string version7 = "1.1.83.954";
std::string version6 = "1.1.83.944";
std::string version5 = "1.1.82.760";
std::string version4 = "1.1.82.758";
std::string version3 = "1.1.82.754";
std::string version2 = "1.1.81.604";
std::string version1 = "1.1.81.598";
std::string version = "1.1.80.699";


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
	
	if (spotifyVersion == version15) {
		address = 0x17131C5; // version: 1.1.88.612
	} else if (spotifyVersion == version14) {
		address = 0x17121C5; // version: 1.1.88.595
	} else if (spotifyVersion == version13) {
		address = 0x1700ABD; // version: 1.1.87.612
	} else if (spotifyVersion == version12) {
		address = 0x16EFC3D; // version: 1.1.86.857
	} else if (spotifyVersion == version11) {
		address = 0x16B7C7C; // version: 1.1.85.895 
	} else if (spotifyVersion == version10) {
		address = 0x16ACCBC; // version: 1.1.85.884 
	} else if (spotifyVersion == version9) {
		address = 0x16A597C; // version: 1.1.84.716
	} else if (spotifyVersion == version8) {
		address = 0x16ACC3D; // version: 1.1.83.956
	} else if (spotifyVersion == version6 || spotifyVersion == version7) {
		address = 0x16AAC3D; // version: 1.1.83.954 & 1.1.83.944
	} else if (spotifyVersion == version5) {
		address = 0x170BAFE; // version: 1.1.82.760
	} else if (spotifyVersion == version4) {
		address = 0x16E4A7E; // version: 1.1.82.758
	} else if (spotifyVersion == version3) {
		address = 0x16E5A7E; // version: 1.1.82.754
	} else if (spotifyVersion == version2) {
		address = 0x170B73E; // version: 1.1.81.604
	} else if (spotifyVersion == version1) {
		address = 0x170273E; // version: 1.1.81.598
	} else if (spotifyVersion == version) {
		address = 0x16525FD; // version: 1.1.80.699
	}

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