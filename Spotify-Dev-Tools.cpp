#include <windows.h>
#include <iostream>
#include <conio.h>
#include <tlhelp32.h>
#include <vector.h>
#include <string.h>
HANDLE process;
HWND hWnd;

int main()
{
	hWnd = FindWindowA(0, "Spotify Free");
	if (!hWnd) {
        std::cerr << "hWnd not found." << std::endl;
    } else {
        std::cout << "Found process" << std::endl;
	}

	DWORD pid;
	GetWindowThreadProcessId(hWnd, &pid);
	process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if(!process) {
        std::cerr<<"Process error."<<std::endl;
	} else {
	  // int address = 0x170B73E; version: 1.81
	  //int address = 0x016aac3d; version: 1.83.954
	  int address = 0x016acc3d; // version: 1.83.956
	  int value1 = 255;
	  int result = WriteProcessMemory(process, (LPVOID*)address, &value1, (DWORD)sizeof(value1), NULL);
	  if(result>0x00) {
         std::cout<<"Result: Success."<<std::endl;
      } else {
         std::cerr<<"Result: Error."<<std::endl;
      }
	  CloseHandle(hWnd);
	  return 0;
	} getch();

	return 0;
}