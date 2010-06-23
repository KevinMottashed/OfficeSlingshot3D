#ifndef USER_PREFERENCES_H
#define USER_PREFERENCES_H

#include "stdafx.h"
#include "boost.h"

// struct that contains all the users preferences
struct UserPreferences
{
public:
	std::string ipAddress;
	std::string name;
	unsigned int armBandPort;
	unsigned int jacketPort;

private:
	// the boost serialization library requires access to the serialize function
	friend class boost::serialization::access;

	// serialization function that the boost library will use for both
	// serializaing and deserializing this data type
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version);
};

// input/output operators to make things easier
std::ostream& operator<<(std::ostream& os, const UserPreferences& pref);
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

