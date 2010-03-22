// Projectile.h: interface for the Projectile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJECTILE_H__ED91DB1D_0FFA_4DA6_A8C2_F936C5838618__INCLUDED_)
#define AFX_PROJECTILE_H__ED91DB1D_0FFA_4DA6_A8C2_F936C5838618__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "chai3d.h"

class Projectile  
{
public:
	Projectile(cVector3d position, cVector3d speed);
	virtual ~Projectile();

	cVector3d position;
	cVector3d speed;
};

#endif // !defined(AFX_PROJECTILE_H__ED91DB1D_0FFA_4DA6_A8C2_F936C5838618__INCLUDED_)
