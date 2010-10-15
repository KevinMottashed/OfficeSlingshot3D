#ifndef TACTILE_ARRAY_H
#define TACTILE_ARRAY_H

#include "mmsystem.h"
#include "math.h"
#include "serialPort.h"

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


/**
 * Interface class for any smart clothing
 * hardware
 */
class TactileArray
{
public:
	/**
	 * Constructor
	 * @param a_portNum The port number
	 */
	TactileArray(int a_portNum);

	/**
	 * Destructor
	 */
	~TactileArray(void);

	/**
	 * Open's the array's serial port
	 * @param pNo The port number. 1 by default
	 * @param announce If true, sends a message when successful. False by default
	 * @return true if successful
	 */
	bool openSerialPort(int pNo = 1, bool announce = false);

	/**
	 * Closes the serial port
	 */
	bool closeSerialPort();

	bool initialize(void);
	void setPort(int a_portNum);
	/**
	 * Setting intensity values
	 * @param pIntensityArray The intensity array
	 */
	void setIntensity(int * pIntensityArray)
	{
		memcpy(m_pIntensityArray, pIntensityArray, m_arraySize);
	}

	/**
	 * Setting intensity values
	 * @param pIntensityArray The intensity array
	 */
	void setIntensity(double * pIntensityArray)
	{
		for(int i=0; i<m_arraySizeX; i++)
			for(int j=0; j<m_arraySizeY; j++)
				setIntensity(i, j, pIntensityArray[i*m_arraySizeX+m_arraySizeY]);
	}

	/**
	 * Setting intensity values
	 * @param arrayX The x value of the array
	 * @param arrayY The y value of the array
	 * @param intensity The intensity to be set
	 */
	void setIntensity(int arrayX, int arrayY, int intensity)
	{
		if(intensity >= m_maxIntensity)
			intensity = m_maxIntensity;
		if(intensity <= 0)
			intensity = 0;		
		m_pIntensityArray[arrayX*m_arraySizeX + arrayY] = intensity;
	}

	/**
	 * Setting intensity values
	 * @param arrayX The x value of the array
	 * @param arrayY The y value of the array
	 * @param intensity The intensity to be set
	 */
	void setIntensity(int arrayX, int arrayY, double intensity)
	{
		if(intensity >= 1.0)
			intensity = 1.0;
		if(intensity <= 0)
			intensity = 0.0;
		m_pIntensityArray[arrayX*m_arraySizeX + arrayY] = (int)(intensity*(double)m_maxIntensity);
	}

	/**
	 * Setting intensity values for all the array
	 * @param intensity The intensity to be set. 0 by default
	 */
	void setIntensityAll(int intensity = 0)
	{
		if(intensity >= m_maxIntensity)
			intensity = m_maxIntensity;
		if(intensity <= 0)
			intensity = 0;
		for(int i=0; i<m_arraySize; i++)
			m_pIntensityArray[i] = intensity;
	}

	/**
	 * Setting intensity values for all the array
	 * @param intensity The intensity to be set
	 */
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

	/**
	 * Sets the array size
	 * @param a_arraySizeX The x value
	 * @param a_arraySizeY The y value
	 */
	void setArraySize(int a_arraySizeX, int a_arraySizeY)
	{
		m_arraySizeX = a_arraySizeX;
		m_arraySizeY = a_arraySizeY;
		m_arraySize = m_arraySizeX*m_arraySizeY;
	}

	/**
	 * Actuates the array
	 */
	void actuate(void);

	/**
	 * Retreive the intensity
	 * @return the intensity
	 */
	int getMaxIntensity(void) const
	{
		return m_maxIntensity;
	}

	/**
	 * Retreive the array size in x
	 * @return array size in x
	 */
	int getArraySizeX(void) const
	{
		return m_arraySizeX;
	}

	/**
	 * Retreive the array size in y
	 * @return array size in y
	 */
	int getArraySizeY(void) const
	{
		return m_arraySizeY;
	}

	/**
	 * Retreive the array size
	 * @return array size
	 */
	int getArraySize(void) const
	{
		return m_arraySize;
	}

private:
	/**
	 * Array's X size
	 */
	int m_arraySizeX;

	/**
	 * Array's Y size
	 */
	int m_arraySizeY;

	/**
	 * Array's size
	 */
	int m_arraySize;

	/**
	 * Reference to the array's intensity
	 */
	int * m_pIntensityArray;

	/**
	 * The maximum possible intensity
	 */
	int m_maxIntensity;

	/**
	 * Buffer to avr
	 */
	unsigned char * m_pBuffer;

	/**
	 * Avr buffer's size
	 */
	int m_bufferSize;
	/**
	 * Serial port
	 */
	CSerialPort * m_pSerialPort;

	/**
	 * Serial port number
	 */
	int m_portNum;

	// thread parameters for test
	DWORD m_dwTestThreadID;
	HANDLE m_hTestThread;

	/**
	 * Assembles the buffer
	 */
	void assembleBuffer(void);
};

#endif

