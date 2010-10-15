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

bool TactileArray::initialize(void)
{
	if(!m_pIntensityArray)
		delete [] m_pIntensityArray;

	m_pIntensityArray = new int[m_arraySize];

	m_bufferSize = 2+(int)((double)m_arraySize*(double)ARRAY_DATA_BIT/(double)DATA_BIT); // 2: header bit + remainder

	if(!m_pBuffer)
		delete [] m_pBuffer;

	m_pBuffer = new unsigned char[m_bufferSize];

	return openSerialPort(m_portNum);
}


void TactileArray::setPort(int a_portNum)
{
	if (m_portNum != a_portNum)
	{
		m_portNum = a_portNum;
		initialize();
	}
}

void TactileArray::setIntensity(int * pIntensityArray)
{
	memcpy(m_pIntensityArray, pIntensityArray, m_arraySize);
}

void TactileArray::setIntensity(double * pIntensityArray)
{
	for(int i=0; i<m_arraySizeX; i++)
		for(int j=0; j<m_arraySizeY; j++)
			setIntensity(i, j, pIntensityArray[i*m_arraySizeX+m_arraySizeY]);
}

void TactileArray::setIntensity(int arrayX, int arrayY, int intensity)
{
	if(intensity >= m_maxIntensity)
		intensity = m_maxIntensity;
	if(intensity <= 0)
		intensity = 0;		
	m_pIntensityArray[arrayX*m_arraySizeX + arrayY] = intensity;
}

void TactileArray::setIntensity(int arrayX, int arrayY, double intensity)
{
	if(intensity >= 1.0)
		intensity = 1.0;
	if(intensity <= 0)
		intensity = 0.0;
	m_pIntensityArray[arrayX*m_arraySizeX + arrayY] = (int)(intensity*(double)m_maxIntensity);
}

void TactileArray::setIntensityAll(int intensity)
{
	if(intensity >= m_maxIntensity)
		intensity = m_maxIntensity;
	if(intensity <= 0)
		intensity = 0;
	for(int i=0; i<m_arraySize; i++)
		m_pIntensityArray[i] = intensity;
}

void TactileArray::setIntensityAll(double intensity)
{
	if(intensity >= 1.0)
		intensity = 1.0;
	if(intensity <= 0)
		intensity = 0.0;
	int scaledIntensity = (int)(intensity*(double)m_maxIntensity);
	for(int i=0; i<m_arraySize; i++)
		m_pIntensityArray[i] = scaledIntensity;
}

void TactileArray::setArraySize(int a_arraySizeX, int a_arraySizeY)
{
	m_arraySizeX = a_arraySizeX;
	m_arraySizeY = a_arraySizeY;
	m_arraySize = m_arraySizeX*m_arraySizeY;
}

int TactileArray::getMaxIntensity(void) const
{
	return m_maxIntensity;
}

int TactileArray::getArraySizeX(void) const
{
	return m_arraySizeX;
}

int TactileArray::getArraySizeY(void) const
{
	return m_arraySizeY;
}

int TactileArray::getArraySize(void) const
{
	return m_arraySize;
}