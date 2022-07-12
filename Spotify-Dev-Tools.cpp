/*
  @author: Exhigh <exhigh01@gmail.com>
*/

#include "Spotify-dev-tools.h"

int main()
{
	HANDLE process;
	std::string dummy, spotifyVersion;
	uintptr_t address;
	int fail = 0;
	char* processVersion, *spotifyPath = NULL;
	do {
		std::cout << "[!] This program works only for Spotify ^_^ " << std::endl;
		process = GetProcessInfo("Spotify.exe", &spotifyPath);
		processVersion = GetProcessVersion(*&spotifyPath);
		if (!process || (processVersion == NULL)) {
			fail++;
			std::cerr << "[-] Can't hook up to process (Is Spotify Open ?)" << std::endl;
			std::cout << "[!] Open Spotify And Hit Any Key To Continue" << std::endl << std::endl;
			std::cout << "[!] Number of Times, Dev Tools Failed To Be Activated: " << fail << std::endl;
			std::getline(std::cin, dummy);
			clear_screen();
		}
	} while (!process);

	spotifyVersion = (char*)malloc(sizeof(processVersion) + 1);
	spotifyVersion = processVersion;
	std::cout << "Current Spotify Version - " << spotifyVersion << std::endl;

	address = SpotifyVersionMap(spotifyVersion);

	int targetValue = 255;
	int result = WriteProcessMemory(process, (LPVOID*)address, &targetValue, (DWORD)sizeof(targetValue), NULL);
	if (result == 0x00 || address == 0) {
		std::cerr << "[-] There was some error, try again later or open an issue, with the Spotify Version!" << std::endl;
	}
	else {
		std::cout << "[+] Developer mode has been successfully enabled." << std::endl;
	}
	std::cout << "[!] Press Any Key To Exit" << std::endl;
	std::getline(std::cin, dummy);
	return 0;
}