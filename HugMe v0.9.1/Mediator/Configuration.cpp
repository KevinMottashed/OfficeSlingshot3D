#include "Configuration.h"

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
		file >> preferences;
	}
	else
	{
		preferences.setDefaults();		
	}
	writeToFile();
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

