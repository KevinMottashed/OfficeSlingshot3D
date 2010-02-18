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
    \author:    Jongeun Cha
    \version    1.0
    \date       05/2005
*/
//===========================================================================

//---------------------------------------------------------------------------
#ifdef _MSVC
#pragma warning (disable : 4786)
#endif
//---------------------------------------------------------------------------
#ifndef CButtonMeshH
#define CButtonMeshH
//---------------------------------------------------------------------------
#include "CMesh.h"
//---------------------------------------------------------------------------

//===========================================================================
/*!
      \class      cButtonMesh
      \brief      cButtonMesh represents a collection of vertices, triangles, materials,
                  and texture properties that can be rendered graphically and
                  haptically.
*/
//===========================================================================
class cButtonMesh : public cMesh
{
  public:
    // CONSTRUCTOR & DESTRUCTOR:

    //! Constructor of cButtonMesh.
    cButtonMesh(cWorld* a_world);
    //! Destructor of cButtonMesh.
    virtual ~cButtonMesh();

    // METHODS - VERTICES
	//!
	void setButtonDirection(cVector3d a_vector);
	//!
	void setClickForce(double a_force);
	//!
	void setReleaseForceRatio(double a_ratio);


    
	//!
	cVector3d m_buttonDirection;
	//!
	bool m_isClicked;
	//!
	double m_clickForce;
	//!
	double m_releaseForce;
	//!
	double m_releaseForceRatio;


  protected:
   
    // METHODS:
};

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------

