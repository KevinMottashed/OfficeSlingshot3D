#ifndef TACTILE_ARRAY_H
#define TACTILE_ARRAY_H

#include "mmsystem.h"
#include "math.h"
#include "serialPort.h"

// definition of the array of the tactile device
//////////////////////////////////////////////////////////
//#define ARRAY_SIZE_X 1
//#define ARRAY_SIZE_Y 31
//#define ARRAY_SIZE ARRAY_SIZE_X*ARRAY_SIZE_Y
#define MAX_INTENSITY 15

#define MAX_APPLIED_INTENSITY 12


// input protocol parameters
//////////////////////////////////////////////////////////

// header bit
#define MASK_HEADER					0x80 //0b10000000
#define MASK_DATA					0x7f //0b01111111
#define HEADER_COMMAND				0x80
#define HEADER_DATA					0x00

// commands
#define COMMAND_ARRAY_DATA			0x01
#define COMMAND_GET_ARRAY_SIZE_X_Y	0x02
#define COMMAND_ARE_YOU_OK			0x03

// payload bit
#define DATA_BIT					7
#define ARRAY_DATA_BIT				4  // for 16 intensities
#define MASK_ARRAY_DATA				0xF000 // from ARRAY_DATA_BIT



class TactileArray
{
public:
	TactileArray(int a_portNum);
	~TactileArray(void);
	bool openSerialPort(int pNo = 1, bool announce = false);
	bool closeSerialPort();
private:
	// array data
	int m_arraySizeX;
	int m_arraySizeY;
	int m_arraySize;
	int * m_pIntensityArray;
	int m_maxIntensity;
	// buffer to avr
	int m_bufferSize;
	unsigned char * m_pBuffer;
	// serial port
	CSerialPort * m_pSerialPort;
	// serial port number
	int m_portNum;
	// thread parameters for test
	DWORD m_dwTestThreadID;
	HANDLE m_hTestThread;

	void assembleBuffer(void);

public:
	bool initialize(void);
	void setPort(int a_portNum);

	// Setting intensity values
	void setIntensity(int * pIntensityArray)
	{
		memcpy(m_pIntensityArray, pIntensityArray, m_arraySize);
	}

	void setIntensity(double * pIntensityArray)
	{
		for(int i=0; i<m_arraySizeX; i++)
			for(int j=0; j<m_arraySizeY; j++)
				setIntensity(i, j, pIntensityArray[i*m_arraySizeX+m_arraySizeY]);
	}

	void setIntensity(int arrayX, int arrayY, int intensity)
	{
		if(intensity >= m_maxIntensity)
			intensity = m_maxIntensity;
		if(intensity <= 0)
			intensity = 0;		
		m_pIntensityArray[arrayX*m_arraySizeX + arrayY] = intensity;
	}

	void setIntensity(int arrayX, int arrayY, double intensity)
	{
		if(intensity >= 1.0)
			intensity = 1.0;
		if(intensity <= 0)
			intensity = 0.0;
		m_pIntensityArray[arrayX*m_arraySizeX + arrayY] = (int)(intensity*(double)m_maxIntensity);
	}

	void setIntensityAll(int intensity = 0)
	{
		if(intensity >= m_maxIntensity)
			intensity = m_maxIntensity;
		if(intensity <= 0)
			intensity = 0;
		for(int i=0; i<m_arraySize; i++)
			m_pIntensityArray[i] = intensity;
	}

	void setIntensityAll(double intensity)
	{
		if(intensity >= 1.0)
			intensity = 1.0;
		if(intensity <= 0)
			intensity = 0.0;
		int scaledIntensity = (int)(intensity*(double)m_maxIntensity);
		for(int i=0; i<m_arraySize; i++)
			m_pIntensityArray[i] = scaledIntensity;
	}

	void setArraySize(int a_arraySizeX, int a_arraySizeY)
	{
		m_arraySizeX = a_arraySizeX;
		m_arraySizeY = a_arraySizeY;
		m_arraySize = m_arraySizeX*m_arraySizeY;
	}
	void actuate(void);
	void test1by1(void);

	int getMaxIntensity(void) const
	{
		return m_maxIntensity;
	}

	int getArraySizeX(void) const
	{
		return m_arraySizeX;
	}

	int getArraySizeY(void) const
	{
		return m_arraySizeY;
	}

	int getArraySize(void) const
	{
		return m_arraySize;
	}
};

#endif

