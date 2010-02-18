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
#ifndef CImageMaterialH
#define CImageMaterialH
//---------------------------------------------------------------------------
#include "CColor.h"
#include "CImageLoader.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include "GL/gl.h"
//---------------------------------------------------------------------------

//===========================================================================
/*!
      \file       CMaterial.h
      \class      cMaterial
      \brief      cMaterial describes the graphic and haptic properties of a
                  solid.
                  
                  Graphic properties include the OpenGL favorites:
                  
                  ambient color, diffuse color, specular color, emissive color, and shininess

                  Haptic properties currently include stiffness, dynamic friction, and static friction
*/
//===========================================================================
struct cImageMaterial
{
public:
	// by gaecha
	// depth image information
	unsigned char * m_hapticData;
	unsigned char * m_tempHapticData;
	unsigned char m_hapticTotalChannel;
	unsigned char m_hapticChannelNum;
	bool m_bIsHapticData;
	bool m_bIsUpdate;

	unsigned int m_hapticImageSizeX;
	unsigned int m_hapticImageSizeY;

	unsigned char * m_bumpData;
	unsigned char * m_tempBumpData;
	unsigned char m_bumpTotalChannel;
	unsigned char m_bumpChannelNum;
	bool m_bIsBumpData;

	unsigned int m_bumpImageSizeX;
	unsigned int m_bumpImageSizeY;
	
	double m_maxStiffness;
	double m_minStiffness;
	double m_maxFriction;
	double m_minFriction;
	double m_maxBump;
	double m_minBump;

	void setImageData(cImageLoader *a_hapticImage, cImageLoader *a_bumpImage);
	void setImageData(cImageLoader *a_hapticImage);
	void setImageData(unsigned char * a_hapticData, unsigned char * a_bumpData);
	void setImageData(unsigned char * a_imageData);
	void setHapticData(unsigned char * a_hapticData);
	void setBumpData(unsigned char *a_bumpData);
	bool updateImage();
	void setHapticImageProperties(unsigned int a_sizeX, unsigned int a_sizeY, unsigned char a_totalChannel, unsigned char a_channelNum);
	void setBumpImageProperties(unsigned int a_sizeX, unsigned int a_sizeY, unsigned char a_totalChannel, unsigned char a_channelNum);

	//! get stiffness level [N/m]
	double getStiffness(double a_x, double a_y);

	//! get static friction level
	double getStaticFriction(double a_x, double a_y);
	//! get dynamic friction level
	double getDynamicFriction(double a_x, double a_y);
	double getBump(double a_x, double a_y);
	//

	// CONSTRUCTOR & DESTRUCTOR:
	//! Constructor of cImageMaterial.
	cImageMaterial() : m_ambient(0.3f, 0.3f, 0.3f, 1.0f), m_diffuse(0.7f, 0.7f, 0.7f, 1.0f),
		m_specular(1.0f, 1.0f, 1.0f, 1.0f), m_emission(0.0f, 0.0f, 0.0f, 1.0f), m_shininess(64),
		m_stiffness(100.0), m_static_friction(0.0), m_dynamic_friction(0.0),
		m_hapticImageSizeX(0), m_hapticImageSizeY(0), m_bumpImageSizeX(0), m_bumpImageSizeY(0),
		m_bIsHapticData(false), m_bIsBumpData(false), m_bIsUpdate(false), 
		m_hapticData(NULL), m_tempHapticData(NULL), m_bumpData(NULL), m_tempBumpData(NULL),
		m_maxBump(-5.0), m_minBump(0.0),
		m_maxStiffness(5.0), m_minStiffness(0.001), m_maxFriction(1.0), m_minFriction(0.001) { };
	//! Destructor of cImageMaterial.
	~cImageMaterial();

	//! Render the material in OpenGL
	virtual void render();


	// METHODS - GRAPHIC PROPERTIES:

	//! Set shininess (the exponent used for specular lighting)
	void setShininess(GLuint a_shininess);
	//! Get shininess
	GLuint getShininess() { return (m_shininess); }

	//! set transparency level (sets the alpha value for all color properties)
	void setTransparencyLevel(float a_levelTransparency);


	// METHODS - HAPTIC PROPERTIES:

	//! set stiffness level [N/m]
	void setStiffness(double a_stiffness);
	//! get stiffness level [N/m]
	inline double getStiffness() const { return (m_stiffness); }

	//! set static friction level
	void setStaticFriction(double a_friction);
	//! set dynamic friction level
	void setDynamicFriction(double a_friction);
	//! get static friction level
	inline double getStaticFriction() const { return (m_static_friction); }
	//! get dynamic friction level
	inline double getDynamicFriction() const { return (m_dynamic_friction); }

	//! tells you whether this ImageMaterial includes partial transparency
	inline bool isTransparent() const {
	  return (m_ambient[4] < 1.0 || m_diffuse[4] < 1.0 || m_specular[4] < 1.0 || m_emission[4]);
	}

	//! For debugging: prints the colors contained in this material
	void print() const;

	// MEMBERS:

	//! Ambient color
	cColorf m_ambient;
	//! Diffuse color
	cColorf m_diffuse;
	//! Specular color
	cColorf m_specular;
	//! Emissive color
	cColorf m_emission;
	//! OpenGL shininess
	GLuint m_shininess;

protected:

	//! Stiffness [netwons per meter]
	double m_stiffness;
	//! Static friction constant
	double m_static_friction;
	//! Dynamic friction constant
	double m_dynamic_friction;

};

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------

