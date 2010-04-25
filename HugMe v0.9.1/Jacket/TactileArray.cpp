#include "stdafx.h"
#include "TactileArray.h"

#pragma warning(disable : 4996)


TactileArray::TactileArray(int a_portNum)
{
	// init the serial port
	m_pSerialPort = NULL;
	m_pSerialPort = new CSerialPort();

	// setting array size
	m_arraySizeX = 0;
	m_arraySizeY = 0;
	m_arraySize = 0;

	// maximum intensity
	m_maxIntensity = MAX_INTENSITY;

	m_pIntensityArray = NULL;
	m_portNum = a_portNum;

	m_bufferSize = 0;
	m_pBuffer = NULL;
}

TactileArray::~TactileArray(void)
{
	closeSerialPort();
	delete m_pSerialPort;
	
	if(m_pIntensityArray)
		delete [] m_pIntensityArray;

	// Error occurs here. But I don't know the reason.
	// Just comment out
	//if(m_pBuffer)
	//	delete [] m_pBuffer;
}

bool TactileArray::openSerialPort(int pNo, bool announce)
{
	if( m_pSerialPort == NULL )
		m_pSerialPort= new CSerialPort();

	if ( m_pSerialPort->getConnected() ) {
		m_pSerialPort->RestartMonitoring();
		if (announce)
			AfxMessageBox("Port already connected");
		return true;
	}

	if( !m_pSerialPort->InitPort(pNo, 57600) ) {
		if(announce) {
			char *msg = new char [64];
			sprintf(msg, "COM%d is not a valid port", pNo);
			AfxMessageBox(msg);
		}
		return false;
	}

	if (m_pSerialPort->StartMonitoring()) {
		Sleep(50);
		/* to do
		getArraySize();
		*/
		return true;
	}
	return false;
}

bool TactileArray::closeSerialPort()
{
	if (m_pSerialPort!=NULL) {
		m_pSerialPort->StopMonitoring();
		m_pSerialPort->ClosePort();
	}
	return true;
}

void TactileArray::assembleBuffer(void)
{
	m_pBuffer[0] = 1<<DATA_BIT | COMMAND_ARRAY_DATA;
	int buffer_num = 1;
	unsigned short queue = 0;
	int queue_num=0;
	int array_num=0;
	while(array_num < m_arraySize) {
		m_pBuffer[buffer_num] = 0x00;
		while(queue_num/DATA_BIT < 1) {
			unsigned short tmp = (unsigned short)m_pIntensityArray[array_num];
			tmp <<= (16-ARRAY_DATA_BIT-queue_num);
			queue |= tmp;
			queue_num += ARRAY_DATA_BIT;
			array_num++;
		}
		m_pBuffer[buffer_num] |= (unsigned char)(queue>>(16-DATA_BIT));
		queue <<= DATA_BIT;
		queue_num -= DATA_BIT;
		buffer_num++;
	}
	if(queue_num > 0) {
		m_pBuffer[buffer_num] = 0<<DATA_BIT | queue>>(queue_num+8);
	}
}

void TactileArray::actuate(void)
{
	// actaully write
	assembleBuffer();
	if(m_pSerialPort && m_pSerialPort->getConnected())
		m_pSerialPort->WriteToPort(m_pBuffer, m_bufferSize);
}

DWORD WINAPI test1by1Thread(LPVOID lp)
{
	TactileArray* ta = (TactileArray*)lp;
	/*
	for(int i=0; i<ta->getArraySizeX(); i++) 
		for(int j=0; j<ta->getArraySizeY(); j++){
		ta->setIntensityAll(0);
		ta->setIntensity(i, j, ta->getMaxIntensity());
		ta->actuate();
		Sleep(100);
	}*/
	
	ta->setIntensityAll(0);
	ta->setIntensity(0,0, 10);
	ta->actuate();
	Sleep(1000);
	ta->setIntensityAll(0);
	ta->setIntensity(0,0, 10);
	ta->actuate();
	Sleep(1000);
	ta->setIntensityAll(0);
	ta->actuate();

 	ExitThread(0);
	return 0;
}

void TactileArray::test1by1(void)
{
	m_hTestThread = ::CreateThread(NULL, NULL, test1by1Thread, this, NULL, &m_dwTestThreadID);
}

void TactileArray::initialize(void)
{
	if(!m_pIntensityArray)
		delete [] m_pIntensityArray;

	m_pIntensityArray = new int[m_arraySize];

	m_bufferSize = 2+(int)((double)m_arraySize*(double)ARRAY_DATA_BIT/(double)DATA_BIT); // 2: header bit + remainder

	if(!m_pBuffer)
		delete [] m_pBuffer;

	m_pBuffer = new unsigned char[m_bufferSize];

	openSerialPort(m_portNum);
}

