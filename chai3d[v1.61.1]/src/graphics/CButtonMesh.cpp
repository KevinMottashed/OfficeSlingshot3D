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

#include "CButtonMesh.h"

//---------------------------------------------------------------------------

//===========================================================================
/*!
    Constructor of cMesh

    \fn       cButtonMesh::cButtonMesh(cWorld* a_world)
    \param    a_parent  Pointer to parent world.
*/
//===========================================================================
cButtonMesh::cButtonMesh(cWorld* a_world) : cMesh(a_world)
{
	m_tag = BUTTON_MESH_TAG;

	m_buttonDirection.set(0.0, 0.0, 1.0);

	m_isClicked = false;

	m_clickForce = 1.0;

	m_releaseForceRatio = 0.5;

	m_releaseForce = m_clickForce*m_releaseForceRatio;
}


//===========================================================================
/*!
    Destructor of cMesh.

    \fn     cButtonMesh::~cButtonMesh()
*/
//===========================================================================
cButtonMesh::~cButtonMesh()
{

}


void cButtonMesh::setButtonDirection(cVector3d a_vector)
{
	m_buttonDirection = a_vector;
	m_buttonDirection.normalize();
}

void cButtonMesh::setClickForce(double a_force)
{
	m_clickForce = a_force;
}

void cButtonMesh::setReleaseForceRatio(double a_ratio)
{
	m_releaseForceRatio = a_ratio;
	m_releaseForce = m_clickForce*m_releaseForceRatio;
}

