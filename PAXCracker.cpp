/*
 *  Copyright (C) 2013 Daniel Correa, http://www.sinfocol.org/
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */  

#include "stdafx.h"
#include "windows.h"
#include "conio.h"
#include "io.h"
#include "PAXCracker.h"

bool bruteforce_end = 0;
char password[128] = {0};

DWORD WINAPI show_password(LPVOID lpParam);
DWORD WINAPI save_session(LPVOID lpParam);

int main(int argc, char* argv[]) {
	unsigned int width = 0, height = 0, cracked = 0, length = 0;
	HANDLE hThreadPassword = NULL, hThreadSession = NULL, paxFile   = NULL;
	HGLOBAL result   = NULL;
	DWORD lpThreadIdPassword = NULL, lpThreadIdSession = NULL;
	FILE *wordlist   = NULL;

	if (argc < 3) {
		fprintf(stdout, "Usage: %s <.PAX file> <wordlist>\n", argv[0]);
		return EXIT_SUCCESS;
	}

	_is5_Initialize("PAXCracker");
	paxFile = _is5_OpenFileSource(argv[1]);
	
	if (paxFile == NULL) {
		fprintf(stderr, "Error while reading .pax file\n");
		return EXIT_FAILURE;
	}

	if (_is5_GuessFileType(paxFile) != 9) {
		fprintf(stderr, "Resource is not PAX file\n");
		return EXIT_FAILURE;
	}

	wordlist = fopen(argv[2], "r");

	if (wordlist == NULL) {
		fprintf(stderr, "Error while reading wordlist file\n");
		return EXIT_FAILURE;
	}

	hThreadPassword = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) show_password, NULL, NULL, &lpThreadIdPassword);
	hThreadSession  = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) save_session, NULL, NULL, &lpThreadIdSession);
	fprintf(stdout, "Press any key to view the status of the bruteforcing\n");

	while (fgets(password, 128, wordlist) != NULL) {
		length = strlen(password) - 1;
		if(password[length] == '\n') {
			password[length] = 0;
		}
		
		_is5_Seek(paxFile, 0,0);

		__try {
			result = _is5_ReadPAX(paxFile, &width, &height, 24, 0, password, 0);
			if (result != 0) {
				cracked = 1;
				break;
			}
		} __except(EXCEPTION_EXECUTE_HANDLER) {
			fprintf(stderr, "Error calling ReadPAX function using \"%s\" password.\n", password);
		}
	}

	bruteforce_end = 1;
	WaitForSingleObject(hThreadPassword, INFINITE);
	WaitForSingleObject(hThreadSession, INFINITE);
	
	if (cracked) {
		fprintf(stdout, "Password found: %s\n", password);
	} else {
		fprintf(stdout, "Password not found\n");
	}

	_is5_CloseSource(paxFile);
	_is5_Exit();
	fclose(wordlist);

	CloseHandle(hThreadPassword);
	CloseHandle(hThreadSession);

    return EXIT_SUCCESS;
}

DWORD WINAPI show_password(LPVOID lpParam) {
	while (!bruteforce_end) {
		if (_kbhit()) {
			fflush(stdout);
			getch();
			fprintf(stdout, "Actual password: %s\n", password);
		}

		Sleep(1000);
	}

	return 0;
}

DWORD WINAPI save_session(LPVOID lpParam) {
	FILE *session;
	session = fopen(".pax_session", "w");

	while (!bruteforce_end) {
		_chsize(fileno(session), 0);
		fseek(session, 0, SEEK_SET);
		fprintf(session, "%s", password);
		Sleep(10000);
	}

	fclose(session);

	return 0;
}