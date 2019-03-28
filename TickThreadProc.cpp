#define _CRT_SECURE_NO_DEPRECATE
#include "TickThreadProc.h"
#include "Film.h"
#include "globals.h"
#include "ATMCD32D.H"
#include "serial.h"
#include "WndProc.h"

HDC hdcMem;

DWORD WINAPI tickThreadProc(HANDLE handle)
{
	// Give plenty of time for main thread to finish setting up
	Sleep(50);
	ShowWindow(hwnd, SW_SHOW);
	// Retrieve the window's DC
	HDC hdc = GetDC(hwnd);

	char text[20];
	char textt[20];// for temperature

	// Create DC with shared pixels to variable 'pixels'

	hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmp);

	SetTextColor(hdc, RGB(0, 0, 0));

	HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	SetDriverEvent(hEvent);
	unsigned long number = int(width / sb)*int(height / pb);
	buffer = (unsigned short*)malloc(int(width / sb)*int(height / pb) * sizeof(unsigned short));

	int temp;
	while(GetTemperature(&temp) != DRV_TEMP_STABILIZED)
	{
	sprintf(textt,"Temp:%d\n", temp);
	TextOut(hdc,width, 0,textt, strlen(textt));
	}
	float true_exposure, true_accumulate, true_kinetic;
	GetAcquisitionTimings(&true_exposure, &true_accumulate, &true_kinetic);
	int total = int(totaltime/true_kinetic); // total number of frames.
	StartAcquisition();
	unsigned int status;
	while (true) {
		WaitForSingleObject(hEvent, INFINITE);
		ResetEvent(hEvent);
		char text_exposure[20];
		if ((status = GetMostRecentImage16(buffer, number)) == DRV_SUCCESS)
		{
			// convert 16 bit to 8bit
			onFrame(buffer);
			// Draw pixels to window
			BitBlt(hdc, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);
			if (filming)
			{
				if (frames <= total)
				{	//Turn on both lasers, red laser should be very weak to distinguish blinking
					//if (frames == 1 || frames == total - 45)
					//{
					//	if (!isGreenON)
					//	{
					//		GREENLASER = WriteONLYComPort(greenCOM, GREENLASER, greenON, greenBAUD);
					//		//WriteComPort(greenCOM, greenON, greenBAUD);
					//		isGreenON = TRUE;
					//	}
					//	if (!isRedON)
					//	{
					//		WriteComPort(redCOM, redON, redBAUD);
					//		isRedON = TRUE;
					//	}
					//}
					//if (frames == 21) //|| frames == total-19
					//{
					//	if (isRedON)
					//	{
					//		WriteComPort(redCOM, redOFF, redBAUD);
					//		isRedON = FALSE;
					//	}
					//}
					//if (frames % 100 == 0)
					//{
					//	if (!isGreenON)
					//	{
					//		GREENLASER = WriteONLYComPort(greenCOM, GREENLASER, greenON, greenBAUD);
					//		isGreenON = TRUE;
					//	}
					//}
					SaveAFrame();
					sprintf(text, "frame #:%d\n", frames);
					TextOut(hdc, width, 60, text, strlen(text));
					frames++;
				}
				else
				{
					StopFilm();
					if (isGreenON || isRedON)
					{
						GREENLASER = WriteONLYComPort(greenCOM, GREENLASER, greenOFF, greenBAUD);
						//WriteComPort(greenCOM, greenOFF, greenBAUD);	// turn off green
						WriteComPort(redCOM, redOFF, redBAUD);	        // turn off red
						WriteComPort(pumpCOM, pumpOFF, pumpBAUD);	    // turn off pump
						isGreenON = FALSE;
						isRedON = FALSE;
						isPumpON = FALSE;
					}
				}
			}
			sprintf(text, "Exposure: %f\n", true_exposure);
			TextOut(hdc, width, 0, text, strlen(text));
			sprintf(text, "Cycle Time:%f\n", true_kinetic);
			TextOut(hdc, width, 15, text, strlen(text));
			sprintf(text, "Input time: %d\n", totaltime);
			TextOut(hdc, width, 30, text, strlen(text));
			sprintf(text, "Actual time:%f\n", actualtime);
			TextOut(hdc, width, 45, text, strlen(text));
		}
	}
	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdc);
}

