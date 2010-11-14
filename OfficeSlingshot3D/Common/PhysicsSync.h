#ifndef PHYSICS_SYNC_H
#define PHYSICS_SYNC_H

#include "stdafx.h"
#include "boost.h"

/**
 * This data structure is used to synchronize 2 virtual environments.
 * This class contains the age of the physics simulation and the current 
 * position and velocity of each projectile.
 */
class PhysicsSync
{
public:
	/**< Constructor. */
	PhysicsSync();

	/**< Destructor. */
	~PhysicsSync();

	/**
	 * Get the age of the game in frames
	 * @return the age of the game in frames
	 */
	unsigned long age() const;

	/**
	 * Set the age of the game in frames
	 * @param age the age of the game in frames
	 */
	void age(unsigned long age);

private:
	// the boost serialization library requires access to the serialize function
	friend class boost::serialization::access;

	/**
	 * Serialize/Deserialize a physics sync object
	 * This function is used by boost for transmiting and saving physics sync objects
	 * @param ar The archive where the sync object will be serialized to or deserialized from
	 * @param version The archive's version
	 */
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version);

	// the age of the game in frames
	unsigned long _age;
};

// output operator
std::ostream& operator<<(std::ostream& os, const PhysicsSync& sync);

//--------------------------------------------
// TEMPLATE IMPLEMENTATIONS
//--------------------------------------------

template <typename Archive>
void PhysicsSync::serialize(Archive& ar, const unsigned int version)
{
	ar & _age;
	return;
}

#endif