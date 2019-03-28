#define _CRT_SECURE_NO_DEPRECATE
#include "MakeSurface.h"
#include "serial.h"
#include "globals.h"

#define COLORBITS 8
#define ALIGNLONG(i) ((i+3)/4*4)

HBITMAP hbmp;
HANDLE GREENLASER;
BYTE* pixels;
FILE* fcolortable;

void MakeSurface(HWND hwnd) {
	// Desired bitmap properties
	struct SIBITMAPINFO
	{
		BITMAPINFOHEADER bmiHeader;
		RGBQUAD bmiColors[256]; //exactly three colors in the palette
	};
	SIBITMAPINFO bmi = { 0 };

	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height; // Order pixels from top to bottom
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = COLORBITS; // last byte not used, 32 bit for alignment
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = (ALIGNLONG((512 * COLORBITS) / 8) * 512);
	bmi.bmiHeader.biXPelsPerMeter = 0;
	bmi.bmiHeader.biYPelsPerMeter = 0;
	bmi.bmiHeader.biClrUsed = 256;
	bmi.bmiHeader.biClrImportant = 256;

	int red, green, blue, i;
	if ((fcolortable = fopen("D:\\Derek\\IxonCameraControl\\shrcol.tbl", "r")) != NULL) {
		i = 0;
		while (fscanf(fcolortable, "%d %d %d\n", &red, &green, &blue) != EOF) {
			bmi.bmiColors[i].rgbRed = red;
			bmi.bmiColors[i].rgbBlue = blue;
			bmi.bmiColors[i].rgbGreen = green;
			bmi.bmiColors[i].rgbReserved = 0;
			i++;
		}
		for (; i<255; i++) {
			bmi.bmiColors[i].rgbRed = 255;
			bmi.bmiColors[i].rgbBlue = 255;
			bmi.bmiColors[i].rgbGreen = 255;
			bmi.bmiColors[i].rgbReserved = 0;
		}
		fclose(fcolortable);
	}
	else {
		MessageBox(hwnd, "Color Table file 'shrcol.tbl' not found", "", MB_OK);
		for (i = 0; i<255; i++) {
			bmi.bmiColors[i].rgbRed = i;
			bmi.bmiColors[i].rgbBlue = i;
			bmi.bmiColors[i].rgbGreen = i;
			bmi.bmiColors[i].rgbReserved = 0;
		}
	}

	HDC hdc = GetDC(hwnd);

	// Create DIB section to always give direct access to pixels
	hbmp = CreateDIBSection(hdc, (BITMAPINFO*)&bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
	DeleteDC(hdc);

	//WriteComPort(greenCOM, greenOFF, greenBAUD);	      // turn off green
	//WriteComPort(greenCOM, greenDelayOFF, greenBAUD);	  // turn off 5 second delay for green
	GREENLASER = OpenComPort(greenCOM, greenBAUD);
	GREENLASER = WriteONLYComPort(greenCOM, GREENLASER, greenDelayOFF, greenBAUD);
	GREENLASER = WriteONLYComPort(greenCOM, GREENLASER, greenOFF, greenBAUD);

	WriteComPort(redCOM, redDelayOFF, redBAUD);	      // turn off 5 second delay for red
	WriteComPort(redCOM, redOFF, redBAUD);              // turn off red
}