#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "chai3d.h"
#include "stdafx.h"
#include "boost.h"

class Projectile  
{
public:
	Projectile();
	Projectile(cVector3d position, cVector3d speed);
	virtual ~Projectile();

	// getter and setter for the position
	cVector3d getPosition() const;
	void setPosition(double x, double y, double z);
	void setPosition(const cVector3d& v);

	// getter and setter for the speed
	cVector3d getSpeed() const;
	void setSpeed(double x, double y, double z);
	void setSpeed(const cVector3d& v);

private:
	// the boost serialization library requires access to the serialize function
	friend class boost::serialization::access;

	// serialization function that the boost library will use for both
	// serializaing and deserializing this data type
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version);

	cVector3d position;
	cVector3d speed;	
};

// output operator to make things easier
std::ostream& operator<<(std::ostream& os, const Projectile& projectile);

//--------------------------------------------
// TEMPLATE IMPLEMENTATIONS
//--------------------------------------------

template <typename Archive>
void Projectile::serialize(Archive& ar, const unsigned int version)
{
	ar & position;
	ar & speed;
	return;
}

#endif
 
