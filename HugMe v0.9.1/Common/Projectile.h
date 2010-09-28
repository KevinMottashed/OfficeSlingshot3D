#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "chai3d.h"
#include "stdafx.h"
#include "boost.h"

/**
 * Represents a projectile
 * Each projectile has a position and a force behind it
 */
class Projectile  
{
public:
	/**
	 * Default constructor.
	 * Initializes the position and force to (0,0,0)
	 */
	Projectile();

	/**
	 * Constructor
	 * @param position The position for this projectile
	 * @param force The force applied to this projectile
	 */
	Projectile(cVector3d position, cVector3d force);
	
	/**
	 * Destructor
	 */
	virtual ~Projectile();

	/**
	 * Get the position
	 * @return The projectile's position
	 */
	cVector3d position() const;

	/**
	 * Set the position
	 * @param x The x coordinate
	 * @param y The y coordinate
	 * @param z The z coordinate
	 */
	void position(double x, double y, double z);

	/**
	 * Set the position
	 * @param v The new position
	 */
	void position(const cVector3d& v);

	/**
	 * Get the force
	 * @return The projectile's force
	 */
	cVector3d force() const;

	/**
	 * Set the force
	 * @param x The x coordinate
	 * @param y The y coordinate
	 * @param z The z coordinate
	 */
	void force(double x, double y, double z);

	/**
	 * Set the force
	 * @param v The new force
	 */
	void force(const cVector3d& v);

private:
	// the boost serialization library requires access to the serialize function
	friend class boost::serialization::access;

	/**
	 * Serialize/Deserialize a projectile
	 * This function is used by boost for transmiting and saving projectiles
	 * @param ar The archive where the projectile will be serialized to or deserialized from
	 * @param version The archive's version
	 */
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version);

	/**
	 * The projectiles position
	 */
	cVector3d _position;

	/**
	 * The projectiles force
	 */
	cVector3d _force;
};

// output operator to make things easier
std::ostream& operator<<(std::ostream& os, const Projectile& projectile);

//--------------------------------------------
// TEMPLATE IMPLEMENTATIONS
//--------------------------------------------

template <typename Archive>
void Projectile::serialize(Archive& ar, const unsigned int version)
{
	ar & _position;
	ar & _force;
	return;
}

#endif
 
