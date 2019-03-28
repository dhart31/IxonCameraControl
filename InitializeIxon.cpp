#define _CRT_SECURE_NO_DEPRECATE
#include "ATMCD32D.H"
#include "InitializeIxon.h"
#include "globals.h"
#pragma comment (lib, "D:\\camera strobe\\atmcd32m.lib")

BOOL Initialize_Ixon()
{
	char 				aBuffer[256];
	int					errorValue;
	BOOL				errorFlag = FALSE;
	int 				test, test2;
	int gblXPixels, gblYPixels;
	GetCurrentDirectory(256, aBuffer);// Look in current working directory
									  // for driver files
	errorValue = Initialize(aBuffer);  // Initialize driver in current directory

	if (errorValue != DRV_SUCCESS) {
		MessageBox(hwnd, "Initialize error", "", MB_OK);
		errorFlag = TRUE;
	}

	// Get detector information
	errorValue = GetDetector(&gblXPixels, &gblYPixels);
	if (errorValue != DRV_SUCCESS) {
		MessageBox(hwnd, "Get detector information error", "", MB_OK);
		errorFlag = TRUE;
	}
	// "Run Till Abort" acquisition mode for Frame Transfer
	// The camera continually acquires data until the AbortAcquisition function is called
	errorValue = SetAcquisitionMode(5);
	if (errorValue != DRV_SUCCESS) {
		MessageBox(hwnd, "Set acquisition mode error", "", MB_OK);
		errorFlag = TRUE;
	}

	errorValue = SetImage(sb, pb, (gblXPixels - width) / 2 + 1, (gblYPixels + width) / 2, (gblXPixels - height) / 2 + 1, (gblYPixels + height) / 2);
	if (errorValue != DRV_SUCCESS) {
		MessageBox(hwnd, "Set image error", "", MB_OK);
		errorFlag = TRUE;
	}

	errorValue = SetReadMode(4);
	if (errorValue != DRV_SUCCESS) {
		MessageBox(hwnd, "Set read mode error", "", MB_OK);
		errorFlag = TRUE;
	}

	errorValue = SetExposureTime(exposure);
	if (errorValue != DRV_SUCCESS) {
		MessageBox(hwnd, "Set exposure time error", "", MB_OK);
		errorFlag = TRUE;
	}

	/*errorValue = SetAccumulationCycleTime(accumulate);
	if (errorValue != DRV_SUCCESS) {
		MessageBox(hwnd, "Set accumulation cycle time error", "", MB_OK);
		errorFlag = TRUE;
	}*/

	errorValue = SetKineticCycleTime(cycletime);
	if (errorValue != DRV_SUCCESS) {
		MessageBox(hwnd, "Set kinetic cycle time error", "", MB_OK);
		errorFlag = TRUE;
	}
	errorValue = SetFrameTransferMode(1);
	if (errorValue != DRV_SUCCESS) {
		MessageBox(hwnd, "Set frame transfer mode error", "", MB_OK);
		errorFlag = TRUE;
	}
	errorValue = CoolerON();
	if (errorValue != DRV_SUCCESS) {
		MessageBox(hwnd, "Switch CoolerON error", "", MB_OK);
		errorFlag = TRUE;
	}

	// Sets the desired CCD temperature in hundredths of degrees Celsius
	errorValue = SetTemperature(-50);
	if (errorValue != DRV_SUCCESS) {
		MessageBox(hwnd, "Set temperature error", "", MB_OK);
		errorFlag = TRUE;
	}
	
	//errorValue = SetADChannel(2);
	//if (errorValue != DRV_SUCCESS) {
	//	MessageBox(hwnd, "Set ADChannel error", "", MB_OK);
	//	errorFlag = TRUE;
	//}

	//Turns on access to higher EM gain levels
	errorValue = SetEMAdvanced(1);
	if (errorValue != DRV_SUCCESS) {
		MessageBox(hwnd, "Set EMAdvanced error", "", MB_OK);
		errorFlag = TRUE;
	}

	//Set gain value
	errorValue = SetEMCCDGain(500);
	if (errorValue != DRV_SUCCESS) {
		MessageBox(hwnd, "Set EMCCDGain error", "", MB_OK);
		errorFlag = TRUE;
	}
	
	//1st parameter sets the max number of images per DMA
	//2nd parameter sets the minimum amount of time between interrupts
	errorValue = SetDMAParameters(0, 0);
	if (errorValue != DRV_SUCCESS) {
		MessageBox(hwnd, "SetDMAParameters", "Error!", MB_OK | MB_ICONEXCLAMATION);
		errorFlag = TRUE;
	}

	//1st param sets TTL high signal to open shutter
	//2nd param sets camera shutter to auto
	//3rd param sets shutter closing time (milliseconds)
	//4th param sets shutter opening time (milliseconds)
	errorValue = SetShutter(1, 0, 0, 0);
	if (errorValue != DRV_SUCCESS) {
		MessageBox(hwnd, "Set shutter error", "Error!", MB_OK | MB_ICONEXCLAMATION);
		errorFlag = TRUE;
	}
	return errorFlag;
}
