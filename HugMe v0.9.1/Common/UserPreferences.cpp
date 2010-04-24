#include "UserPreferences.h"

std::ostream& operator<<(std::ostream& os, const UserPreferences& pref)
{
	os << pref.ipAddress << std::endl;
	os << pref.name << std::endl;
	os << pref.armBandPort << std::endl;
	os << pref.jacketPort << std::endl;
	return os;
}

std::istream& operator>>(std::istream& is, UserPreferences& pref)
{
	is >> pref.ipAddress;
	is >> pref.name;
	is >> pref.armBandPort;
	is >> pref.jacketPort;
	return is;
}