#ifndef USER_PREFERENCES_H
#define USER_PREFERENCES_H

#include "stdafx.h"

// struct that contains all the users preferences
struct UserPreferences
{
	std::string ipAddress;
	std::string name;
	unsigned int armBandPort;
	unsigned int jacketPort;
};

// input/output operators to make things easier
std::ostream& operator<<(std::ostream& os, const UserPreferences& pref);
std::istream& operator>>(std::istream& is, UserPreferences& pref);

#endif

