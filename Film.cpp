#define _CRT_SECURE_NO_DEPRECATE
#include "Film.h"
#ifndef WINDOWS_H
#define WINDOWS_H
#include <Windows.h>
#endif
#include <atlstr.h> // For CString
#include "globals.h"

const short width = 512;
const short height = 512;
int sb = 2;
int pb = 2;
int totaltime = 5;
float exposure = 0.020;
float accumulate = 0;
float cycletime = 0;

unsigned long frames = 1;
unsigned short *buffer;
float actualtime;
bool filming = false;

HWND hwnd;
LARGE_INTEGER timerFreq_;
LARGE_INTEGER counterAtStart_;
FILE* fout;

void SaveAFrame()
{
fwrite(&frames, sizeof(short), 1, fout);
fwrite(buffer, sizeof(short), int(width / sb)*int(height / pb), fout);
};

void StartFilm()
{
	char text[20];
	short temp;
	int i = 1;
	char buffer[2];
	char fname[] = _T("ixon01_100ms.pma");

	BOOL exist = PathFileExists(fname);
	while (exist == TRUE)
	{
		i++;
		_itoa(i, buffer, 10);
		if (i < 10)
		{
			buffer[1] = buffer[0];
			buffer[0] = '0';
		}
		fname[4] = buffer[0];
		fname[5] = buffer[1];
		exist = PathFileExists(fname);
	}

	sprintf(text, fname);

	if ((fout = fopen(text, "wb")) != NULL) {
		frames = 0;
		filming = true;
		temp = width / sb;
		fwrite(&temp, sizeof(short), 1, fout);
		temp = height / pb;
		fwrite(&temp, sizeof(short), 1, fout);
		QueryPerformanceFrequency(&timerFreq_);
		QueryPerformanceCounter(&counterAtStart_);
	}
	else {
		MessageBox(hwnd, text, "File Open Failed", MB_OK);
	}

}

void StopFilm()
{
	filming = false;
	LARGE_INTEGER c;
	QueryPerformanceCounter(&c);
	actualtime = (float)(c.QuadPart - counterAtStart_.QuadPart)/ timerFreq_.QuadPart;
	if (fout != NULL) {
		fclose(fout);
	}
}

void onFrame(unsigned short* buffer) {
	// This is where takes place

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			BYTE h = buffer[(y / pb) * width / sb + (x / sb)] >> 4;
			pixels[y * width + x] = h;
		}
	}
}