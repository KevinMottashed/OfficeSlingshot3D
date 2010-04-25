#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "chai3d.h"
#include "stdafx.h"

class Projectile  
{
public:
	Projectile();
	Projectile(cVector3d position, cVector3d speed);
	virtual ~Projectile();

	// getter and setter for the position
	cVector3d getPosition() const;
	void setPosition(double x, double y, double z);

	// getter and setter for the speed
	cVector3d getSpeed() const;
	void setSpeed(double x, double y, double z);	

private:
	cVector3d position;
	cVector3d speed;	
};

// output operator to make things easier
std::ostream& operator<<(std::ostream& os, const Projectile& projectile);

#endif
 
