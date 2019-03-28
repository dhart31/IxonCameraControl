// Creating an window
#ifndef WINDOWS_H
#define WINDOWS_H
#include<Windows.h>
#endif
#include "globals.h"
#include "WndProc.h"

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{
	WNDCLASSEX wc;
	MSG msg;

	// Init wc
	wc.cbClsExtra = 0; // Amount of extra data allocated for this class in memory. Usually 0.
	wc.cbWndExtra = 0; // Amount of extra data allocated in memory per window of this type. Usually 0.
	wc.cbSize = sizeof(WNDCLASSEX); // The size of the structure.
	wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0)); // Background Brush to set the color of our window.
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Cursor that will be displayed over our window.
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Large (usually 32x32) icon shown when the user presses Alt+Tab.
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // Small (usually 16x16) icon to show in the taskbar and in the top left corner of the window.
	wc.hInstance = hInstance; // Handle to application instance (that we got in the first parameter of WinMain()).
	wc.lpfnWndProc = WndProc; // Pointer to the window procedure for this window class.
	wc.lpszClassName = "animation_class"; // Name to identify the class with.
	wc.lpszMenuName = NULL; // Name of a menu resource to use for the windows with this class.
	wc.style = 0; // Class Styles (CS_*), not to be confused with Window Styles (WS_*) This can usually be set to 0.

				  // Register wc
	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Failed to register window class.", "Error", MB_OK);
		return 0;
	}

	// Make window
	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		"animation_class",
		"Ixon+",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		400, 100, width + 190, height,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nShowCmd);

	UpdateWindow(hwnd);

	while (GetMessage(&msg, 0, 0, NULL) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}