#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "stdafx.h"
#include "UserPreferences.h"

// manages the software configuration and
// stores it in a file
class Configuration
{
public:
	Configuration(const std::string& fileName);
	~Configuration();

	UserPreferences getUserPreferences() const;
	void setUserPreferences(const UserPreferences& pref);
private:
	void readFromFile();
	void writeToFile();

	// default preferences
	static const std::string default_ip_address;
	static const std::string default_name;
	static const unsigned int default_arm_band_port = 3;
	static const unsigned int default_jacket_port = 97;

	UserPreferences preferences;
	std::string fileName;
};

#endif

