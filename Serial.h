#pragma once
#include <atlstr.h> // For CString

/*Red Laser settings for WriteComPort
* Serial commands are found on page 5-20 of the Coherent Cube Manual
*/
extern char redCOM[];
extern char redON[];
extern char redOFF[];
extern char redDelayON[];
extern char redDelayOFF[];
extern int redBAUD;
/*Green Laser settings for WriteComPort
* Serial commands were discovered using Free Serial Port Monitor to look at the Flex control software.
*/
extern char greenCOM[];
extern char greenON[];
extern char greenOFF[];
extern char greenDelayON[];
extern char greenDelayOFF[];
extern char greenPowerQuery[];
extern int greenBAUD;
extern int bytesRead;
/*Pump settings for WriteComPort
* Serial commands are found on page 47 of the pump manual.
*/
extern char pumpCOM[];
extern char pumpON[];
extern char pumpOFF[];
extern int pumpBAUD;

bool WriteComPort(CString PortSpecifier, char data[], int baud);
char QueryComPort(CString PortSpecifier, char data[], int MsgLength, int baud);
HANDLE OpenComPort(CString PortSpecifier, int baud);
HANDLE WriteONLYComPort(CString PortSpecifier, HANDLE hPort, char data[], int baud);
bool CloseComPort(CString PortSpecifier, HANDLE hPort, int baud);

