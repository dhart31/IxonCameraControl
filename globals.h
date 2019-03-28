#pragma once
#ifndef WINDOWS_H
#define WINDOWS_H
#include <Windows.h>
#endif
#include <stdio.h>


extern bool filming;
extern const short width;
extern const short height;
extern int sb;
extern int pb;
extern unsigned long frames;
extern int totaltime;
extern float exposure, accumulate, cycletime;
extern float actualtime;
extern BYTE* pixels;
extern unsigned short *buffer;

extern HWND hwnd;
extern HDC hdcMem;
extern HBITMAP hbmp;
extern HANDLE GREENLASER;
