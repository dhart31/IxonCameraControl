#include "WndProc.h"
#include "serial.h"
#include "globals.h"
#include "MakeSurface.h"
#include "ATMCD32D.H"
#include "Film.h"
BOOL toggle1 = TRUE; // Toggle  for red laser
BOOL toggle2 = TRUE; // Toggle  for green laser
BOOL toggle3 = TRUE; // Toggle  for pump
BOOL isGreenON = FALSE; // Toggle for turning on both lasers while filming
BOOL isRedON = FALSE; // Toggle for turning off red laser while filming
BOOL isPumpON = FALSE;   // Toggle for turning on pump while filming
HANDLE hTickThread;

LRESULT CALLBACK WndProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	HDC hdc = GetDC(hwnd);
	char text[20];

	switch (msg) {
	case WM_CREATE:
	{
		MakeSurface(hwnd);
		if (!Initialize_Ixon())
		hTickThread = CreateThread(NULL, NULL, &tickThreadProc, NULL, NULL, NULL);
	}
	break;

	case WM_KEYDOWN:

		switch ((int)wParam)
		{
		// '1', red channel toggle
		case 49:
		{
			if (toggle1)
			{
				WriteComPort(redCOM, redON, redBAUD); //turn on red
				toggle1 = FALSE;
			}
			else
			{
				WriteComPort(redCOM, redOFF, redBAUD); //turn off red
				toggle1 = TRUE;
			}
		}
		break;

		// '2', green channel toggle
		case 50:
		{
			if (toggle2)
			{
				GREENLASER = WriteONLYComPort(greenCOM, GREENLASER, greenON, greenBAUD);
				//WriteComPort(greenCOM, greenON, greenBAUD); //turn on green
				toggle2 = FALSE;
			}
			else
			{
				GREENLASER = WriteONLYComPort(greenCOM, GREENLASER, greenOFF, greenBAUD);
				//WriteComPort(greenCOM, greenOFF, greenBAUD); //turn off green
				toggle2 = TRUE;
			}
		}
		break;

		// '3', pump channel toggle
		case 51:
		{
			if (toggle3)
			{
				WriteComPort(pumpCOM, pumpON, pumpBAUD); //turn off pump
				toggle3 = FALSE;
			}
			else
			{
				WriteComPort(pumpCOM, pumpOFF, pumpBAUD); //turn off pump
				toggle3 = TRUE;
			}
		}
		break;

		// space bar, start/stop film loop recording
		case 32:
			if (!filming) StartFilm();
			else
			{
				StopFilm();
				GREENLASER = WriteONLYComPort(greenCOM, GREENLASER, greenOFF, greenBAUD);
				//WriteComPort(greenCOM, greenOFF, greenBAUD);	// turn off green
				WriteComPort(redCOM, redOFF, redBAUD);	        // turn off red
				WriteComPort(pumpCOM, pumpOFF, pumpBAUD);	    // turn off pump
				isGreenON = FALSE;
				isRedON = FALSE;
				isPumpON = FALSE;
			}
			break;

		// 'o', open port
		case 79:
			GREENLASER = OpenComPort(greenCOM, greenBAUD);
			break;

		// 'f', close port
		case 70:
			CloseComPort(greenCOM, GREENLASER, greenBAUD);

			break;
		}
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// Draw pixels to window when window needs repainting
		BitBlt(hdc, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);


		EndPaint(hwnd, &ps);
	}
	break;
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
	}
	break;
	case WM_DESTROY:
	{
		GREENLASER = WriteONLYComPort(greenCOM, GREENLASER, greenDelayON, greenBAUD);
		GREENLASER = WriteONLYComPort(greenCOM, GREENLASER, greenOFF, greenBAUD);
		CloseComPort(greenCOM, GREENLASER, greenBAUD);
		WriteComPort(redCOM, redDelayON, redBAUD);	        // turn on 5 second delay for red
															//WriteComPort(greenCOM, greenDelayON, greenBAUD);	// turn on 5 second delay for green
															//WriteComPort(greenCOM, greenOFF, greenBAUD);	    // turn off green
		WriteComPort(redCOM, redOFF, redBAUD);	            // turn off red
		WriteComPort(pumpCOM, pumpOFF, pumpBAUD);	        // turn off pump


		AbortAcquisition();
		CoolerOFF();
		ShutDown();

		TerminateThread(hTickThread, 0);
		PostQuitMessage(0);
	}
	break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}