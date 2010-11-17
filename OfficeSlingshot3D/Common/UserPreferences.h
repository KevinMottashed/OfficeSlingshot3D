#ifndef USER_PREFERENCES_H
#define USER_PREFERENCES_H

#include "stdafx.h"
#include "boost.h"

/**
 * @ingroup Common
 * @b public
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

	bool muted; /**< True if the audio should be muted. */
	float volume; /**< The volume on a scale of 0.0 to 1.0. */

	static const std::string default_ip_address; /**< Default IP Address. */
	static const std::string default_name; /**< Default user name. */
	static const unsigned int default_arm_band_port; /**< Default blue tooth port for the arm band. */
	static const unsigned int default_jacket_port; /**< Default blue tooth port for the jacket. */
	static const bool default_muted; /**< Default for if the application is muted or not. */
	static const float default_volume; /**< Default volume level. */

	/**
	 * Reset all the preferences to the defaults.
	 */
	void setDefaults();

private:
	// the boost serialization library requires access to the serialize function
	friend class boost::serialization::access;

	/**
	 * Serialize or deserialize the user preferences.
	 * This function is used by the boost library and should not be called directly.
	 * @param[in,out] ar The archive to serialize to or deserialize from.
	 * @param[in] version The archive version.
	 */
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version);
};

/**
 * Output operator.
 * The output is saved as one field per line in the .ini style.
 * @param[out] os The output stream.
 * @param[in] pref The preferences that will saved to the file.
 * @return The output stream.
 */
std::ostream& operator<<(std::ostream& os, const UserPreferences& pref);

/**
 * Input operator.
 * The input is assumed to come in as one field per line in the .ini style.
 * If a field is missing or the parameter is not correct (ex: string instead of number)
 * Then the defaults will be used.
 * @param[in,out] is The input stream.
 * @param[in] pref The preferences that will read from the file.
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
	ar & muted;
	ar & volume;
	return;
}

#endif

