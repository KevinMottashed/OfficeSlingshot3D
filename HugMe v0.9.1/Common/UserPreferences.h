#ifndef USER_PREFERENCES_H
#define USER_PREFERENCES_H

#include "stdafx.h"
#include "boost.h"

/**
 * Represents the user preferences.
 * The user preferencese consist of the following:
 * An ip address which is the default ip to connect to.
 * The user name.
 * The blue tooth port for the arm band.
 * The blue tooth port fot the jacket.
 */
struct UserPreferences
{
public:
	/**
 	 * The default ip address to connect to.
	 */
	std::string ipAddress;

	/**
	 * The user name.
	 */
	std::string name;

	/**
	 * The bluetooth port for the arm band.
	 */
	unsigned int armBandPort;

	/**
	 * The bluetooth port for the jacket.
	 */
	unsigned int jacketPort;

	static const std::string default_ip_address; /**< Default IP Address */
	static const std::string default_name; /**< Default user name */
	static const unsigned int default_arm_band_port; /**< Default blue tooth port for the arm band */
	static const unsigned int default_jacket_port; /**< Default blue tooth port for the jacket. */

	/**
	 * Reset all the preferences to the defaults
	 */
	void setDefaults();

private:
	// the boost serialization library requires access to the serialize function
	friend class boost::serialization::access;

	/**
	 * Serialize or deserialize the user preferences.
	 * This function is used by the boost library and should not be called directly.
	 * @param ar The archive to serialize to or deserialize from.
	 * @param version The archive version.
	 */
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version);
};

/**
 * Output operator.
 * The output is saved as one field per line in the .ini style.
 * IpAddress=<ip>
 * Name=<name>
 * ArmBandPort=<port#>
 * JacketPort=<port#>
 * @param os The output stream.
 * @param pref The preferences that will saved to the file.
 * @return The output stream.
 */
std::ostream& operator<<(std::ostream& os, const UserPreferences& pref);

/**
 * Input operator.
 * The input is assumed to come in as one field per line in the .ini style.
 * IpAddress=<ip>
 * Name=<name>
 * ArmBandPort=<port#>
 * JacketPort=<port#>
 * If a field is missing or the parameter is not correct (ex: string instead of number)
 * Then the defaults will be used.
 * @param is The input stream.
 * @param pref The preferences that will read from the file.
 * @return The input stream.
 */
std::istream& operator>>(std::istream& is, UserPreferences& pref);

//--------------------------------------------
// TEMPLATE IMPLEMENTATIONS
//--------------------------------------------

template <typename Archive>
void UserPreferences::serialize(Archive& ar, const unsigned int version)
{
	ar & ipAddress;
	ar & name;
	ar & armBandPort;
	ar & jacketPort;
	return;
}

#endif

