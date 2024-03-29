#ifndef TERM_H
#define TERM_H

#include "register_common.h"

#ifdef WIN_TERM
#include <windows.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

class Term {
    public:
        Term()
		#ifdef WIN_TERM
		:hStdOut(GetStdHandle(STD_OUTPUT_HANDLE))
		#endif
		{}
		
        void initiate() {
			#ifdef WIN_TERM
			if(!getenv("TERM")) {
				HANDLE hNewBuff;
				hNewBuff = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 
					FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
				SetConsoleActiveScreenBuffer(hNewBuff);
				SetStdHandle(STD_OUTPUT_HANDLE, hNewBuff);
				freopen("CONOUT$", "w", stdout);
			}
			else {
			#endif
				cout << "\0337\033[?47h\033[1;1H";
			#ifdef WIN_TERM
			}
			#endif
			clear();
		}
		
        void clear() {
			#ifdef WIN_TERM
			if(!getenv("TERM")) {
				HANDLE hCurOut;
				CONSOLE_SCREEN_BUFFER_INFO csbi;
				DWORD count, cellCount;
				COORD homeCoords = {0, 0};
				hCurOut = CreateFile("CONOUT$", GENERIC_READ | GENERIC_WRITE, 
					FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
				GetConsoleScreenBufferInfo(hCurOut, &csbi);
				cellCount = csbi.dwSize.X * csbi.dwSize.Y;
				FillConsoleOutputCharacter(hCurOut, (TCHAR)' ', cellCount, homeCoords, &count);
				FillConsoleOutputAttribute(hCurOut, csbi.wAttributes, cellCount, homeCoords, &count);
				SetConsoleCursorPosition(hCurOut, homeCoords);
			}
			else {
			#endif
				cout << "\033[2J\033[1;1H";
			#ifdef WIN_TERM
			}
			#endif
		}
		
        void restore(){
			clear();
			#ifdef WIN_TERM

			if(!getenv("TERM")) {
				HANDLE hCurOut;
				hCurOut = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleActiveScreenBuffer(hStdOut);
				SetStdHandle(STD_OUTPUT_HANDLE, hStdOut);
				CloseHandle(hCurOut);
				freopen("CONOUT$", "w", stdout);
			}
			else {
			#endif
				cout << "\0338\033[?47l";
			#ifdef WIN_TERM
			}
			#endif
		}
		
        void pause() {
			string temp;
			cout << "\nHit ENTER to continue... \n";
			getline(cin, temp);
		}

    private:
        #ifdef WIN_TERM
        HANDLE hStdOut;
        #endif
};

#endif