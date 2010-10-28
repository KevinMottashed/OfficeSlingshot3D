/*
**	FILENAME			CSerialPort.h
**
**	PURPOSE				This class can read, write and watch one serial port.
**						It sends messages to its owner when something happends on the port
**						The class creates a thread for reading and writing so the main
**						program is not blocked.
**
**	CREATION DATE		15-09-1997
**	LAST MODIFICATION	12-11-1997
**
**	AUTHOR				Remon Spekreijse
**
**
*/


#ifndef SERIALPORT_H
#define SERIALPORT_H

#define WM_COMM_BREAK_DETECTED		WM_USER+1	// A break was detected on input.
#define WM_COMM_CTS_DETECTED		WM_USER+2	// The CTS (clear-to-send) signal changed state. 
#define WM_COMM_DSR_DETECTED		WM_USER+3	// The DSR (data-set-ready) signal changed state. 
#define WM_COMM_ERR_DETECTED		WM_USER+4	// A line-status error occurred. Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY. 
#define WM_COMM_RING_DETECTED		WM_USER+5	// A ring indicator was detected. 
#define WM_COMM_RLSD_DETECTED		WM_USER+6	// The RLSD (receive-line-signal-detect) signal changed state. 
#define WM_COMM_RXCHAR				WM_USER+7	// A character was received and placed in the input buffer. 
#define WM_COMM_RXFLAG_DETECTED		WM_USER+8	// The event character was received and placed in the input buffer.  
#define WM_COMM_TXEMPTY_DETECTED	WM_USER+9	// The last character in the output buffer was sent.  

// small buffer for input - should be ok for this app, not enough for many others
// beware
#define IN_BUFFER_SIZE				64

class CSerialPort
{														 
public:
	// contruction and destruction
	
	CSerialPort();
	virtual		~CSerialPort();

	// port initialisation											
//	BOOL		InitPort(UINT portnr = 1, UINT baud = 57600, char parity = 'N', UINT databits = 8, UINT stopsbits = 1, DWORD dwCommEvents = EV_RXCHAR | EV_CTS, UINT nBufferSize = 512);
	BOOL		InitPort(UINT portnr = 5, UINT baud = 115000, char parity = 'N', UINT databits = 8, UINT stopsbits = 1, DWORD dwCommEvents = EV_RXCHAR | EV_CTS, UINT nBufferSize = 512);
	void		ClosePort();

	// start/stop comm watching
	BOOL		StartMonitoring();
	BOOL		RestartMonitoring();
	BOOL		StopMonitoring();

	DWORD		GetWriteBufferSize();
	DWORD		GetCommEvents();
	DCB			GetDCB();

	void		WriteToPort(unsigned char* string, int len);
	void		WriteToPort(unsigned char chr);


	// whether we have sent the current packet
	bool				sentPacket;

	void writeInBuffer(unsigned char in)
		{
		inBuffer[bufferWrite] = in;
		if (bufferWrite<IN_BUFFER_SIZE-1)
			bufferWrite++; // this algorithm just means that when we reach the end of our buffer, we just keep writing over the last byte
						   // hopefully won't come up!
		}

	unsigned char readInBuffer()
		{
		if (bufferWrite>0 && bufferRead!=bufferWrite)
			{
			int temp = inBuffer[bufferRead];
			bufferRead++;
			if (bufferRead==bufferWrite)
				{
				bufferWrite=0;
				bufferRead=0;
				}
			return temp;
			}

		if (bufferRead==bufferWrite)
			{
			bufferWrite=0;
			bufferRead=0;
			}
		return 0;
		}

	bool inBufferData() const
		{return !(bufferRead==bufferWrite);}

	bool getConnected() const
		{return connected;}

protected:
	// protected memberfunctions
	void		ProcessErrorMessage(char* ErrorText);
	static UINT	CommThread( LPVOID pParam );
	static void	ReceiveChar(CSerialPort* port, COMSTAT comstat);
	static void	WriteChar(CSerialPort* port);

	// thread
	CWinThread*			m_Thread;

	// synchronisation objects
	CRITICAL_SECTION	m_csCommunicationSync;
	BOOL				m_bThreadAlive;

	// handles
	HANDLE				m_hShutdownEvent;
	HANDLE				m_hComm;
	HANDLE				m_hWriteEvent;

	// Event array. 
	// One element is used for each event. There are two event handles for each port.
	// A Write event and a receive character event which is located in the overlapped structure (m_ov.hEvent).
	// There is a general shutdown when the port is closed. 
	HANDLE				m_hEventArray[3];

	// structures
	OVERLAPPED			m_ov;
	COMMTIMEOUTS		m_CommTimeouts;
	DCB					m_dcb;

	// misc
	UINT				m_nPortNr;
	unsigned char*		m_szWriteBuffer;
	unsigned short		m_WriteBufferSize;
	DWORD				m_dwCommEvents;
	DWORD				m_nWriteBufferSize;

	// bool for connection
	bool				connected;

	// we have a small internal buffer which we upload characters into 
	// we then read them with the same timer that commands the write scehdule
	// simplifies programming for subsequent developers
	unsigned char 		inBuffer[IN_BUFFER_SIZE];
	int					bufferWrite;
	int					bufferRead;
};

#endif
 
