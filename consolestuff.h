#pragma once

#include <windows.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>

const int objectsize = 40;
const int boardsize = 200;

//screen size
#define WIDTH 30
#define HEIGHT 30


//Create Handles
HANDLE writehandle;
HANDLE readhandle;

//cursor info struct
CONSOLE_CURSOR_INFO cursor = {1, false};


//input event buffer
INPUT_RECORD irBuffer[128];
CHAR_INFO buffy[WIDTH * HEIGHT];

//Screen Size and Write area structures
SMALL_RECT screenSize = { 0, 0, WIDTH -1 , HEIGHT -1 };
SMALL_RECT writearea = { 0, 0, WIDTH -1, HEIGHT -1 };

COORD buffersize = { WIDTH, HEIGHT };
COORD startlocation = { 0, 0 };

//TCHAR title[6];


//Setup Console buffer and handle etc.
void setupscreen() {

	TCHAR title[] = {'T', 'E', 'T', 'R', 'I', 'S', '\0'};
	 
	SetConsoleTitle(title);

	//Get standard input/output handles
	readhandle = GetStdHandle(STD_INPUT_HANDLE);
	writehandle = GetStdHandle(-11); // Dword equates to -11 "STD_OUTPUT_HANDLE"


	SetConsoleWindowInfo(writehandle, TRUE, &writearea);
	SetConsoleScreenBufferSize(writehandle, buffersize);
	SetConsoleMode(readhandle, ENABLE_WINDOW_INPUT);
	SetConsoleCursorInfo(writehandle, &cursor);

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 25;
	cfi.dwFontSize.Y = 25;
	cfi.FontFamily = FF_DONTCARE; //required for font size/width changes. 
	cfi.FontWeight = FW_NORMAL;

	SetCurrentConsoleFontEx(writehandle, TRUE, &cfi);


	//fill screen buffer with initial values
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {

			buffy[x + WIDTH * y].Char.AsciiChar = 219;
			buffy[x + WIDTH * y].Attributes = 11;

		}
	}
}


//Draw Background
void background() {

	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {

			if ( x < 15) {
				buffy[x + WIDTH * y].Char.AsciiChar = 219;
				buffy[x + WIDTH * y].Attributes = 8;
			}
			else {
			buffy[x + WIDTH * y].Char.AsciiChar = 219;
			buffy[x + WIDTH * y].Attributes = 7;
			}
		}
	}
}

void draw() {
	
	WriteConsoleOutputA(writehandle, buffy, buffersize, startlocation, &writearea);

}

char checkInput() {

	DWORD nEvents = 0;
	char output;

	GetNumberOfConsoleInputEvents(readhandle, &nEvents);

	if (nEvents > 0) {
		ReadConsoleInput(readhandle, irBuffer, 128, &nEvents);

		for (int i = 0; i < nEvents; i++) {
		
			if (irBuffer[i].Event.KeyEvent.bKeyDown)
			{
				output = irBuffer[i].Event.KeyEvent.uChar.AsciiChar;
				return output;
			}
		}

	}

	return 0;
}