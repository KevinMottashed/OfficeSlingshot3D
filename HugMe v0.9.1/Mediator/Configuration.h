#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "stdafx.h"
#include "UserPreferences.h"

/**
 * Manages the software's configuration and stores it in a file
 */
class Configuration
{
public:
	/**
	 * Constructor.
	 * @param fileName The file where the configuration is stored
	 */
	Configuration(const std::string& fileName);

	/**
	 * Destructor.
	 */
	~Configuration();

	/**
	 * Get the user preferences.
	 * @return The user preferences.
	 */
	UserPreferences getUserPreferences() const;

	/**
	 * Set the user preferences.
	 * @param pref The new preferences.
	 */
	void setUserPreferences(const UserPreferences& pref);
private:
	/**
	 * Read the preferences from a file.
	 */
	void readFromFile();

	/**
	 * Write the preferences to a file.
	 */
	void writeToFile();

	/**
	 * Default IP address.
	 */
	static const std::string default_ip_address;

	/**
	 * Default user name.
	 */
	static const std::string default_name;

	/**
	 * Default blue tooth port for the arm band.
	 */
	static const unsigned int default_arm_band_port = 3;

	/**
	 * Default blue tooth port for the jacket.
	 */
	static const unsigned int default_jacket_port = 97;

	/**
	 * The user preferences.
	 */
	UserPreferences preferences;

	/**
	 * The file where the preferences are stored.
	 */
	std::string fileName;
};

#endif

