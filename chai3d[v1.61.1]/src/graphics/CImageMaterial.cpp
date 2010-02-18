//===========================================================================
/*
    This file is part of the CHAI 3D visualization and haptics libraries.
    Copyright (C) 2003-2004 by CHAI 3D. All rights reserved.

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License("GPL") version 2
    as published by the Free Software Foundation.

    For using the CHAI 3D libraries with software that can not be combined
    with the GNU GPL, and for taking advantage of the additional benefits
    of our support services, please contact CHAI 3D about acquiring a
    Professional Edition License.

    \author:    <http://www.chai3d.org>
    \author:    Francois Conti
    \version    1.1
    \date       01/2004
    
*/
//===========================================================================

//---------------------------------------------------------------------------
#include "CImageMaterial.h"
#include "CMaths.h"
#include "CMacrosGL.h"

//---------------------------------------------------------------------------
cImageMaterial::~cImageMaterial()
{
	if(m_hapticData)
		delete m_hapticData;
	if(m_tempHapticData)
		delete m_tempHapticData;
	if(m_bumpData)
		delete m_bumpData;
	if(m_tempBumpData)
		delete m_tempBumpData;
}
//===========================================================================
/*!
    Set the transparency level (by setting the alpha value for all color properties)

    \fn     void cImageMaterial::setTransparencyLevel(const float a_levelTransparency)
    \param  a_levelTransparency  Level of transparency.
*/
//===========================================================================
void cImageMaterial::setTransparencyLevel(float a_levelTransparency)
{
    // make sur value is in range [0.0 - 1.0]
    float level = cClamp(a_levelTransparency, 0.0f, 1.0f);

    // apply new value
    m_ambient.setA(level);
    m_diffuse.setA(level);
    m_specular.setA(level);
    m_emission.setA(level);
}


//===========================================================================
/*!
    Set the level of shininess. Value are clamped to range from 0 --> 128

    \fn     void cImageMaterial::setShininess(const GLuint a_shininess)
    \param  a_shininess  Level of shininess
*/
//===========================================================================
void cImageMaterial::setShininess(GLuint a_shininess)
{
    m_shininess = cClamp(a_shininess, (GLuint)0, (GLuint)128);
}


//===========================================================================
/*!
    Set the level of stiffness. Clamped to be a non-negative value.

    \fn     void cImageMaterial::setStiffness(const double a_stiffness)
    \param  a_stiffness  Level of stiffness
*/
//===========================================================================
void cImageMaterial::setStiffness(double a_stiffness)
{
    m_stiffness = cClamp0(a_stiffness);
}


//===========================================================================
/*!
    Set the level of static friction. Clamped to be a non-negative value.

    \fn     void cImageMaterial::setStaticFriction(const double a_friction)
    \param  a_friction  Level of friction.
*/
//===========================================================================
void cImageMaterial::setStaticFriction(double a_friction)
{
    m_static_friction = cClamp0(a_friction);
}


//===========================================================================
/*!
    Set the level of dynamic friction. Clamped to be a non-negative value.

    \fn     void cImageMaterial::setDynamicFriction(const double a_friction)
    \param  a_friction  Level of friction.
*/
//===========================================================================
void cImageMaterial::setDynamicFriction(double a_friction)
{
    m_dynamic_friction = cClamp0(a_friction);
}


//===========================================================================
/*!
    Render this material in OpenGL.

    \fn     void cImageMaterial::render()
*/
//===========================================================================
void cImageMaterial::render()
{
    glDisable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (const float *)&m_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (const float *)&m_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (const float *)&m_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, (const float *)&m_emission);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);
}


void cImageMaterial::print() const
{
  CHAI_DEBUG_PRINT("A %0.2f,%0.2f,%0.2f,%0.2f, D %0.2f,%0.2f,%0.2f,%0.2f S %0.2f,%0.2f,%0.2f,%0.2f E %0.2f,%0.2f,%0.2f,%0.2f \n",
  m_ambient[0],m_ambient[1],m_ambient[2],m_ambient[3],
  m_diffuse[0],m_diffuse[1],m_diffuse[2],m_diffuse[3],
  m_specular[0],m_specular[1],m_specular[2],m_specular[3],
  m_emission[0],m_emission[1],m_emission[2],m_emission[3]
  );
}


void cImageMaterial::setImageData(cImageLoader *a_image)
{
	unsigned char *imageData;

	if(a_image)
		imageData = (unsigned char *)a_image->getData();

	setImageData(imageData);

	return;
}

void cImageMaterial::setImageData(cImageLoader *a_hapticImage, cImageLoader *a_bumpImage)
{
	unsigned char *hapticData = NULL;
	unsigned char *bumpData = NULL;

	if(a_hapticImage)
		hapticData = (unsigned char *)a_hapticImage->getData();
	if(a_bumpImage)
		bumpData = (unsigned char *)a_bumpImage->getData();

	setImageData(hapticData, bumpData);

	return;
}

void cImageMaterial::setImageData(unsigned char * a_imageData)
{
	if(a_imageData) {
		setHapticData(a_imageData);
		setBumpData(a_imageData);
	}
}

void cImageMaterial::setImageData(unsigned char * a_hapticData, unsigned char *a_bumpData)
{
	if(a_hapticData)
		setHapticData(a_hapticData);
	if(a_bumpData)
		setBumpData(a_bumpData);
}

void cImageMaterial::setHapticData(unsigned char * a_hapticData)
{
	if(a_hapticData == NULL)
		return;

	memcpy(m_tempHapticData, a_hapticData, m_hapticTotalChannel*m_hapticImageSizeX*m_hapticImageSizeY*sizeof(unsigned char));
	
	m_bIsHapticData = true;

	m_bIsUpdate = true;
	// Above, when updating the bump values, the haptic loop can reference the bump values
	// ('memcpy' takes very short time, but 'for' takes longer time.)
	// So, I make temporary image container and update it first.
	// Then, the actual image is updated with 'memcpy' in updateImage() function.
}

void cImageMaterial::setBumpData(unsigned char *a_bumpData)
{
	if(a_bumpData == NULL)
		return;

	memcpy(m_tempBumpData, a_bumpData, m_bumpTotalChannel*m_bumpImageSizeX*m_bumpImageSizeY*sizeof(unsigned char));
	
	m_bIsBumpData = true;

	m_bIsUpdate = true;
	// Above, when updating the bump values, the haptic loop can reference the bump values
	// ('memcpy' takes very short time, but 'for' takes longer time.)
	// So, I make temporary image container and update it first.
	// Then, the actual image is updated with 'memcpy' in updateImage() function.
}

bool cImageMaterial::updateImage()
{
	if(m_bIsUpdate) {
		if(m_hapticData)
			memcpy(m_hapticData, m_tempHapticData, m_hapticTotalChannel*m_hapticImageSizeX*m_hapticImageSizeY*sizeof(unsigned char));
		if(m_bumpData)
			memcpy(m_bumpData, m_tempBumpData, m_bumpTotalChannel*m_bumpImageSizeX*m_bumpImageSizeY*sizeof(unsigned char));
		
		m_bIsUpdate = false;
		return true;
	}
	else
		return false;
}

void cImageMaterial::setHapticImageProperties(unsigned int a_sizeX, unsigned int a_sizeY,
											unsigned char a_totalChannel, unsigned char a_channelNum)
{
	if( (m_hapticImageSizeX != a_sizeX) || (m_hapticImageSizeY != a_sizeY) ||
		(m_hapticTotalChannel != a_totalChannel) ) {
		m_hapticTotalChannel = a_totalChannel;
		m_hapticChannelNum = a_channelNum;
		m_hapticImageSizeX = a_sizeX;
		m_hapticImageSizeY = a_sizeY;

		if(m_hapticData)
			free(m_hapticData);
		if(m_tempHapticData)
			free(m_tempHapticData);

		m_hapticData = new unsigned char[m_hapticTotalChannel*m_hapticImageSizeX*m_hapticImageSizeY*sizeof(unsigned char)];
		memset(m_hapticData, 0, m_hapticTotalChannel*m_hapticImageSizeX*m_hapticImageSizeY*sizeof(unsigned char));
		m_tempHapticData = new unsigned char[m_hapticTotalChannel*m_hapticImageSizeX*m_hapticImageSizeY*sizeof(unsigned char)];
		memset(m_tempHapticData, 0, m_hapticTotalChannel*m_hapticImageSizeX*m_hapticImageSizeY*sizeof(unsigned char));
	}
}

void cImageMaterial::setBumpImageProperties(unsigned int a_sizeX, unsigned int a_sizeY,
											unsigned char a_totalChannel, unsigned char a_channelNum)
{
	if( (m_bumpImageSizeX != a_sizeX) || (m_bumpImageSizeY != a_sizeY) ||
		(m_bumpTotalChannel != a_totalChannel) ) {
		m_bumpImageSizeX = a_sizeX;
		m_bumpImageSizeY = a_sizeY;
		m_bumpTotalChannel = a_totalChannel;
		m_bumpChannelNum = a_channelNum;

		if(m_bumpData)
			free(m_bumpData);
		if(m_tempBumpData)
			free(m_tempBumpData);

		m_bumpData = new unsigned char[m_bumpTotalChannel*m_bumpImageSizeX*m_bumpImageSizeY*sizeof(unsigned char)];
		memset(m_bumpData, 0, m_bumpTotalChannel*m_bumpImageSizeX*m_bumpImageSizeY*sizeof(unsigned char));
		m_tempBumpData = new unsigned char[m_bumpTotalChannel*m_bumpImageSizeX*m_bumpImageSizeY*sizeof(unsigned char)];
		memset(m_tempBumpData, 0, m_bumpTotalChannel*m_bumpImageSizeX*m_bumpImageSizeY*sizeof(unsigned char));
	}
}

//! get stiffness level [N/m]
double cImageMaterial::getStiffness(double a_x, double a_y)
{
	double fx = a_x*m_hapticImageSizeX;
	double fy = a_y*m_hapticImageSizeY;
	int x = (int)fx;
	int y = (int)fy;
	if(m_bIsHapticData) {
		double result = (double)m_hapticData[m_hapticTotalChannel*(m_hapticImageSizeX*y + x) + m_hapticChannelNum];
		result = (1.0-result/255.0)*m_minStiffness + result/255.0*m_maxStiffness;
		return result;
	}
	else
		return m_stiffness;
}

//! get static friction level
double cImageMaterial::getStaticFriction(double a_x, double a_y)
{
	double fx = a_x*m_hapticImageSizeX;
	double fy = a_y*m_hapticImageSizeY;
	int x = (int)fx;
	int y = (int)fy;
	if(m_bIsHapticData) {
		double result = (double)m_hapticData[m_hapticTotalChannel*(m_hapticImageSizeX*y + x) + m_hapticChannelNum + 1];
		result = (1.0-result/255.0)*m_minFriction + result/255.0*m_maxFriction;
		return result;
	}
	else
		return m_static_friction;
}
//! get dynamic friction level
double cImageMaterial::getDynamicFriction(double a_x, double a_y)
{
	double fx = a_x*m_hapticImageSizeX;
	double fy = a_y*m_hapticImageSizeY;
	int x = (int)fx;
	int y = (int)fy;
	if(m_bIsHapticData) {
		double result = (double)m_hapticData[m_hapticTotalChannel*(m_hapticImageSizeX*y + x) + m_hapticChannelNum + 2];
		result = (1.0-result/255.0)*m_minFriction + result/255.0*m_maxFriction;
		return result;
	}
	else
		return m_static_friction;
}
//! get bump level
double cImageMaterial::getBump(double a_x, double a_y)
{
	double result = 0.0;
	double fx = a_x*m_bumpImageSizeX;
	double fy = a_y*m_bumpImageSizeY;
	int x = (int)fx;
	int y = (int)fy;
	if(m_bIsBumpData) {
		double bumpValue = (double)m_bumpData[m_bumpTotalChannel*(m_bumpImageSizeX*y + x)+m_bumpChannelNum];
		result = (1.0-bumpValue/255.0)*m_minBump + bumpValue/255.0*m_maxBump;
	}
	return result;
}
//