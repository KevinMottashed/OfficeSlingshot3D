#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "boost.h"
#include "chai3d.h"

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
	 * @param[in] position The position for this projectile
	 * @param[in] force The force applied to this projectile
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
	 * @param[in] x The x coordinate
	 * @param[in] y The y coordinate
	 * @param[in] z The z coordinate
	 */
	void position(double x, double y, double z);

	/**
	 * Set the position.
	 * @param[in] v The new position.
	 */
	void position(const cVector3d& v);

	/**
	 * Get the force
	 * @return The projectile's force
	 */
	cVector3d force() const;

	/**
	 * Set the force.
	 * @param[in] x The x coordinate
	 * @param[in] y The y coordinate
	 * @param[in] z The z coordinate
	 */
	void force(double x, double y, double z);

	/**
	 * Set the force
	 * @param[in] v The new force
	 */
	void force(const cVector3d& v);

	/**
	 * Compare 2 projectiles for equality.
	 * A projectile's position and force is expressed using floating point numbers
	 * so the epsilon paramater is provided to allow for an error margin.
	 * @param[in] other The other projectile.
	 * @param[in] epsilon The error margin allowed in the floating point comparisons.
	 * @return true if the projectiles are equal.
	 */
	bool equals(const Projectile& other, float epsilon = 0.0f);

private:
	// the boost serialization library requires access to the serialize function
	friend class boost::serialization::access;

	/**
	 * Serialize/Deserialize a projectile
	 * This function is used by boost for transmiting and saving projectiles
	 * @param[in,out] ar The archive where the projectile will be serialized to or deserialized from
	 * @param[in] version The archive's version
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

/**
 * Output operator.
 * @param[out] os Output stream to add to.
 * @param[in] projectile Object to write to stream.
 * @return Returns the param os.
 */
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
 
