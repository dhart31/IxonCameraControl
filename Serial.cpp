// This function was taken directly from http://members.ee.net/brey/Serial.pdf with modification only to data and data length.
#include "serial.h"

/*Red Laser settings for WriteComPort
* Serial commands are found on page 5-20 of the Coherent Cube Manual
*/
char redCOM[] = "COM4";														//The name of red laser's COM port found in device manager.
char redON[] = { 0x4C, 0x3D, 0x31, 0x0D, 0x0A };							//ASCII for L=1\r\n    \r (return carriage) and \n (linefeed) are necessary
char redOFF[] = { 0x4C, 0x3D, 0x30, 0x0D, 0x0A };							//ASCII for L=0\r\n
char redDelayON[] = { 0x43, 0x44, 0x52, 0x48, 0x3D, 0x31, 0x0D, 0x0A };		//ASCII for CDRH=1\r\n
char redDelayOFF[] = { 0x43, 0x44, 0x52, 0x48, 0x3D, 0x30, 0x0D, 0x0A };	//ASCII for CDRH=0\r\n
int redBAUD = 19200;														//baud rate of 19200

/*Green Laser settings for WriteComPort
* Serial commands were discovered using Free Serial Port Monitor to look at the Flex control software.
*/
char greenCOM[] = "COM1";													//The name of green laser's COM port found in device manager.
char greenON[] = { 0x24, 0x4C, 0x31, 0x0D, 0x0A };							//ASCII for $L1\r\n     \r (return carriage) and \n (linefeed) are necessary
char greenOFF[] = { 0x24, 0x4C, 0x30, 0x0D, 0x0A };							//ASCII for $L0\r\n     \r (return carriage) and \n (linefeed) are necessary
char greenDelayON[] = { 0x24, 0x57, 0x31, 0x0D, 0x0A };						//ASCII for $W1\r\n     \r (return carriage) and \n (linefeed) are necessary
char greenDelayOFF[] = { 0x24, 0x57, 0x30, 0x0D, 0x0A };					//ASCII for $W0\r\n     \r (return carriage) and \n (linefeed) are necessary
char greenPowerQuery[] = { 0x3f, 0x47, 0x0d, 0x0a };						//ASCII for ?G\r\n 
int greenBAUD = 115200;														//baud rate of 115200
int bytesRead = 12;															//any number higher than this crashes the program

/*Pump settings for WriteComPort
* Serial commands are found on page 47 of the pump manual.
*/
char pumpCOM[] = "COM5";													//The name of pump's COM port found in device manager.
char pumpON[] = { 0x52, 0x55, 0x4E, 0x0D, 0x0A };							//ASCII for RUN\r\n    \r (return carriage) and \n (linefeed) are necessary
char pumpOFF[] = { 0x53, 0x54, 0x50, 0x0D, 0x0A };							//ASCII for STP\r\n    \r (return carriage) and \n (linefeed) are necessary
int pumpBAUD = 19200;														//baud rate of 19200

bool WriteComPort(CString PortSpecifier, char data[], int baud)
{
	DCB dcb;
	DWORD byteswritten;
	HANDLE hPort = CreateFile(PortSpecifier, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL); //Opens port

	if (!GetCommState(hPort, &dcb)) //Checks that the port was opened correctly
		return false;

	switch (baud) {
	case 19200:
	{
		dcb.BaudRate = CBR_19200; //19200 Baud
	}
	break;

	case 115200:
	{
		dcb.BaudRate = CBR_115200; //115200 Baud
	}
	break;
	}
	dcb.ByteSize = 8; //8 data bits
	dcb.Parity = NOPARITY; //no parity
	dcb.StopBits = ONESTOPBIT; //1 stop

	bool retVal = WriteFile(hPort, data, strlen(data), &byteswritten, NULL); //Writes to port
	CloseHandle(hPort); //closes the port
	return retVal;
}


char QueryComPort(CString PortSpecifier, char data[], int MsgLength, int baud)
{
	DCB dcb;
	char retVal;
	BYTE Byte;
	DWORD byteswritten;
	DWORD dwBytesTransferred;
	DWORD dwCommModemStatus;


	HANDLE hPort = CreateFile(PortSpecifier, (GENERIC_READ | GENERIC_WRITE), 0, NULL, OPEN_EXISTING, 0, NULL);


	if (!GetCommState(hPort, &dcb)) //Returns an error if the port was opened incorrectly
		return 0;

	switch (baud) {
	case 19200:
	{
		dcb.BaudRate = CBR_19200; //19200 Baud
	}
	break;

	case 115200:
	{
		dcb.BaudRate = CBR_115200; //115200 Baud
	}
	break;
	}
	dcb.ByteSize = 8; //8 data bits 
	dcb.Parity = NOPARITY; //no parity 
	dcb.StopBits = ONESTOPBIT; //1 stop 

	WriteFile(hPort, data, strlen(data), &byteswritten, NULL);
	ReadFile(hPort, &Byte, MsgLength, &dwBytesTransferred, 0);
	ReadFile(hPort, &Byte, MsgLength, &dwBytesTransferred, 0);
	ReadFile(hPort, &Byte, MsgLength, &dwBytesTransferred, 0);
	CloseHandle(hPort);
	return 0;
}

HANDLE OpenComPort(CString PortSpecifier, int baud)
{
	DCB dcb;
	DWORD byteswritten;
	HANDLE hPort = CreateFile(PortSpecifier, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL); //Opens port

	if (!GetCommState(hPort, &dcb)) //Checks that the port was opened correctly
		return false;

	switch (baud) {
	case 9600:
	{
		dcb.BaudRate = CBR_9600; //19200 Baud
	}
	break;
	case 19200:
	{
		dcb.BaudRate = CBR_19200; //19200 Baud
	}
	break;

	case 115200:
	{
		dcb.BaudRate = CBR_115200; //115200 Baud
	}
	break;
	}
	dcb.ByteSize = 8; //8 data bits
	dcb.Parity = NOPARITY; //no parity
	dcb.StopBits = ONESTOPBIT; //1 stop

							   //bool retVal = WriteFile(hPort,data,strlen(data),&byteswritten,NULL); //Writes to port
							   //CloseHandle(hPort); //closes the port
	return hPort;
}

HANDLE WriteONLYComPort(CString PortSpecifier, HANDLE hPort, char data[], int baud)
{
	DCB dcb;
	DWORD byteswritten;
	//HANDLE hPort = CreateFile(PortSpecifier, GENERIC_WRITE, 0, NULL, OPEN_EXISTING,	0, NULL); //Opens port

	if (!GetCommState(hPort, &dcb)) //Checks that the port was opened correctly
		return false;

	switch (baud) {
	case 9600:
	{
		dcb.BaudRate = CBR_9600; //19200 Baud
	}
	break;
	case 19200:
	{
		dcb.BaudRate = CBR_19200; //19200 Baud
	}
	break;

	case 115200:
	{
		dcb.BaudRate = CBR_115200; //115200 Baud
	}
	break;
	}
	dcb.ByteSize = 8; //8 data bits
	dcb.Parity = NOPARITY; //no parity
	dcb.StopBits = ONESTOPBIT; //1 stop

	WriteFile(hPort, data, strlen(data), &byteswritten, NULL); //Writes to port
															   //CloseHandle(hPort); //closes the port
	return hPort;
}
bool CloseComPort(CString PortSpecifier, HANDLE hPort, int baud)
{
	DCB dcb;
	DWORD byteswritten;
	//HANDLE hPort = CreateFile(PortSpecifier, GENERIC_WRITE, 0, NULL, OPEN_EXISTING,	0, NULL); //Opens port

	if (!GetCommState(hPort, &dcb)) //Checks that the port was opened correctly
		return false;

	switch (baud) {
	case 9600:
	{
		dcb.BaudRate = CBR_9600; //19200 Baud
	}
	break;
	case 19200:
	{
		dcb.BaudRate = CBR_19200; //19200 Baud
	}
	break;

	case 115200:
	{
		dcb.BaudRate = CBR_115200; //115200 Baud
	}
	break;
	}
	dcb.ByteSize = 8; //8 data bits
	dcb.Parity = NOPARITY; //no parity
	dcb.StopBits = ONESTOPBIT; //1 stop

							   //bool retVal = WriteFile(hPort,data,strlen(data),&byteswritten,NULL); //Writes to port
	CloseHandle(hPort); //closes the port
						//return retVal;
}