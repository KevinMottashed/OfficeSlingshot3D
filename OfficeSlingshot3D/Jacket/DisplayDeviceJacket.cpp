#include "stdafx.h"
#include "Chat.h"
#include "DisplayDeviceJacket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DisplayDeviceJacket::DisplayDeviceJacket(int a_portNum) : TactileArray(a_portNum)
{
	int i, j;
	for(i=0; i<CHEST_ARRAY_SIZE_X; i++)
		for(j=0; j<CHEST_ARRAY_SIZE_Y; j++)
			m_chestArray[i][j] = j*CHEST_ARRAY_SIZE_X + i;
	for(i=0; i<ARM_ARRAY_SIZE_X; i++)
		for(j=0; j<ARM_ARRAY_SIZE_Y; j++)
			m_armArray[i][j] = j*ARM_ARRAY_SIZE_X + i + CHEST_ARRAY_SIZE_X*CHEST_ARRAY_SIZE_Y;
	for(i=0; i<NECK_ARRAY_SIZE_X; i++)
		for(j=0; j<NECK_ARRAY_SIZE_Y; j++)
			m_neckArray[i][j] = j*NECK_ARRAY_SIZE_X + i + CHEST_ARRAY_SIZE_X*CHEST_ARRAY_SIZE_Y
			+ ARM_ARRAY_SIZE_X*ARM_ARRAY_SIZE_Y;
	
	int totalSize = CHEST_ARRAY_SIZE_X*CHEST_ARRAY_SIZE_Y + ARM_ARRAY_SIZE_X*ARM_ARRAY_SIZE_Y + 
		NECK_ARRAY_SIZE_X*NECK_ARRAY_SIZE_Y;

	setArraySize(1, totalSize+2);
}

DisplayDeviceJacket::~DisplayDeviceJacket()
{

}

void DisplayDeviceJacket::setActChest(double a_actX, double a_actY)
{
	double coordX = a_actX * (double)(CHEST_ARRAY_SIZE_X-1);
	double coordY = a_actY * (double)(CHEST_ARRAY_SIZE_Y-1);
	double coordXF = floor(coordX);
	double coordYF = floor(coordY);

	setIntensityAll(0);
	if( coordX == coordXF && coordY == coordYF ) {
		int actIndex = m_chestArray[(int)coordX][(int)coordY];
		int actInt = MAX_INTENSITY;
		setIntensity(0, actIndex, actInt);
	} else if(coordX == coordXF) {
		int actIndex[2], actInt[2];
		actIndex[0] = m_chestArray[(int)coordX][(int)coordYF];
		actInt[0] = (int)floor( (1.0 + coordYF - coordY)*(double)MAX_INTENSITY );
		actIndex[1] = m_chestArray[(int)coordX][(int)coordYF+1];
		actInt[1] = (int)floor( (coordY - coordYF)*(double)MAX_INTENSITY );
		setIntensity(0, actIndex[0], actInt[0]);
		setIntensity(0, actIndex[1], actInt[1]);
	} else if(coordY == coordYF) {
		int actIndex[2], actInt[2];
		actIndex[0] = m_chestArray[(int)coordXF][(int)coordY];
		actInt[0] = (int)floor( (1.0 + coordXF - coordX)*(double)MAX_INTENSITY );
		actIndex[1] = m_chestArray[(int)coordXF+1][(int)coordY];
		actInt[1] = (int)floor( (coordX - coordXF )*(double)MAX_INTENSITY );
		setIntensity(0, actIndex[0], actInt[0]);
		setIntensity(0, actIndex[1], actInt[1]);
	} else {
		int actIndex[4], actInt[4];
		actIndex[0] = m_chestArray[(int)coordXF][(int)coordYF];
		actInt[0] = (int)floor( ( 2.0 + coordXF - coordX+ coordYF - coordY )*MAX_INTENSITY*0.5);
		actIndex[1] = m_chestArray[(int)coordXF+1][(int)coordYF];
		actInt[1] = (int)floor( ( 1.0 + coordX - coordXF + coordYF - coordY)*MAX_INTENSITY*0.5);
		actIndex[2] = m_chestArray[(int)coordXF+1][(int)coordYF+1];
		actInt[2] = (int)floor( ( coordX - coordXF + coordY - coordYF)*MAX_INTENSITY*0.5);
		actIndex[3] = m_chestArray[(int)coordXF][(int)coordYF+1];
		actInt[3] = (int)floor( ( 1.0 + coordXF - coordX + coordY - coordYF)*MAX_INTENSITY*0.5);		

		setIntensity(0, actIndex[0], actInt[0]);
		setIntensity(0, actIndex[1], actInt[1]);
		setIntensity(0, actIndex[2], actInt[2]);
		setIntensity(0, actIndex[3], actInt[3]);
	}
}

void DisplayDeviceJacket::setActUpperArm(double a_actX, double a_actY)
{
	double coordX = a_actX * (double)(ARM_ARRAY_SIZE_X-1);
	double coordY = a_actY * (double)(ARM_ARRAY_SIZE_Y-1);
	double coordXF = floor(coordX);
	double coordYF = floor(coordY);

	setIntensityAll(0);
	if( coordX == coordXF && coordY == coordYF ) {
		int actIndex = m_armArray[(int)coordX][(int)coordY];
		int actInt = MAX_INTENSITY;
		setIntensity(0, actIndex, actInt);
	} else if(coordX == coordXF) {
		int actIndex[2], actInt[2];
		actIndex[0] = m_armArray[(int)coordX][(int)coordYF];
		actInt[0] = (int)floor( (1.0 + coordYF - coordY)*(double)MAX_INTENSITY );
		actIndex[1] = m_armArray[(int)coordX][(int)coordYF+1];
		actInt[1] = (int)floor( (coordY - coordYF)*(double)MAX_INTENSITY );
		setIntensity(0, actIndex[0], actInt[0]);
		setIntensity(0, actIndex[1], actInt[1]);
	} else if(coordY == coordYF) {
		int actIndex[2], actInt[2];
		actIndex[0] = m_armArray[(int)coordXF][(int)coordY];
		actInt[0] = (int)floor( (1.0 + coordXF - coordX)*(double)MAX_INTENSITY );
		actIndex[1] = m_armArray[(int)coordXF+1][(int)coordY];
		actInt[1] = (int)floor( (coordX - coordXF )*(double)MAX_INTENSITY );
		setIntensity(0, actIndex[0], actInt[0]);
		setIntensity(0, actIndex[1], actInt[1]);
	} else {
		int actIndex[4], actInt[4];
		actIndex[0] = m_armArray[(int)coordXF][(int)coordYF];
		actInt[0] = (int)floor( ( 2.0 + coordXF - coordX+ coordYF - coordY )*MAX_INTENSITY*0.5);
		actIndex[1] = m_armArray[(int)coordXF+1][(int)coordYF];
		actInt[1] = (int)floor( ( 1.0 + coordX - coordXF + coordYF - coordY)*MAX_INTENSITY*0.5);
		actIndex[2] = m_armArray[(int)coordXF+1][(int)coordYF+1];
		actInt[2] = (int)floor( ( coordX - coordXF + coordY - coordYF)*MAX_INTENSITY*0.5);
		actIndex[3] = m_armArray[(int)coordXF][(int)coordYF+1];
		actInt[3] = (int)floor( ( 1.0 + coordXF - coordX + coordY - coordYF)*MAX_INTENSITY*0.5);		

		setIntensity(0, actIndex[0], actInt[0]);
		setIntensity(0, actIndex[1], actInt[1]);
		setIntensity(0, actIndex[2], actInt[2]);
		setIntensity(0, actIndex[3], actInt[3]);
	}
}

void DisplayDeviceJacket::setActNeck(int a_actX, double a_intensity)
{
	setIntensityAll(0);
	setIntensity(0, m_neckArray[a_actX][0], (int)a_intensity);
}

