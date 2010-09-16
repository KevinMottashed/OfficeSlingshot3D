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
	bool failed = false;

	if (file.is_open()) 
	{
		// grab the preferences from the file
		file >> preferences;
		if (file.fail())
		{
			failed = true;
		}
	}
	else
	{
		failed = true;
	}

	if (failed)
	{
		// fill in default preferences if we fail to retrieve from the file
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

