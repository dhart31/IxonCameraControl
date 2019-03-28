#pragma once
#ifndef WINDOWS_H
#define WINDOWS_H
#include <Windows.h>
#endif

extern HANDLE hTickThread;

extern BOOL toggle1; //= TRUE; // Toggle  for red laser
extern BOOL toggle2; // = TRUE; // Toggle  for green laser
extern BOOL toggle3; // = TRUE; // Toggle  for pump
extern BOOL isGreenON; // = FALSE; // Toggle for turning on both lasers while filming
extern BOOL isRedON; // = FALSE; // Toggle for turning off red laser while filming
extern BOOL isPumpON; // = FALSE;   // Toggle for turning on pump while filming

LRESULT CALLBACK WndProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);

DWORD WINAPI tickThreadProc(HANDLE handle);

BOOL Initialize_Ixon();

void MakeSurface(HWND hwnd);