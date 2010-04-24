#include "Configuration.h"

const std::string Configuration::default_ip_address = "127.0.0.1";
const std::string Configuration::default_name = "Player";

Configuration::Configuration(const std::string& fileName) : fileName(fileName)
{
	readFromFile();
}

Configuration::~Configuration()
{
}

void Configuration::readFromFile()
{
	std::ifstream file (fileName.c_str());
	if (file.is_open()) 
	{
		// grab the preferences from the file
		// assume that no errors will occurr
		file >> preferences;
	}
	else
	{
		// fill in default preferences if no file is present
		preferences.ipAddress = default_ip_address;
		preferences.name = default_name;
		preferences.armBandPort = default_arm_band_port;
		preferences.jacketPort = default_jacket_port;

		// write the defaults to the file
		writeToFile();
	}
	return;
}

void Configuration::writeToFile()
{
	std::ofstream file (fileName.c_str());
	if (file.is_open()) 
	{
		file << preferences;
	}
	return;
}

UserPreferences Configuration::getUserPreferences() const
{
	return preferences;
}

void Configuration::setUserPreferences(const UserPreferences& pref)
{
	preferences = pref;
	writeToFile();
	return;
}