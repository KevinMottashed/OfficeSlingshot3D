#include "CommonTestSuite.h"

#include "UserPreferences.h"

using namespace std;

CommonTestSuite::CommonTestSuite(const std::string& name) :
	TestSuite(name)
{
}

CommonTestSuite::~CommonTestSuite()
{
}

void CommonTestSuite::run()
{
	UNIT_TEST(testGoodPreferences);
	UNIT_TEST(testMalformedPreferences);
	UNIT_TEST(testMissingPreferences);
}

void CommonTestSuite::testGoodPreferences()
{
	const string prefString = "IpAddress=127.0.0.1\nName=Player3\nArmBandPort=3\nJacketPort=67\nMuted=0\nVolume=1\n";

	// create the good preferences
	stringstream s;
	s << prefString;

	// read them from the stream
	UserPreferences pref;
	s >> pref;

	// make sure they are what they should be
	assert_equals(pref.ipAddress, "127.0.0.1");
	assert_equals(pref.name, "Player3");
	assert_equals(pref.armBandPort, 3);
	assert_equals(pref.jacketPort, 67);
	assert_equals(pref.muted, false);
	assert_equals(pref.volume, 1.0f);

	// write the preferences to a stream
	ostringstream os;
	os << pref;

	// make sure they are what they were originally
	assert_equals(os.str(), prefString);
}

void CommonTestSuite::testMalformedPreferences()
{
	const string prefString = "IpAddress=127.0.0.1\nName=Player18\nArmBandPort=abc\nJacketPort=67\nMuted=0\nVolume=1\n";

	// create the malformed preferences
	stringstream s;
	s << prefString;

	// read them from the stream
	UserPreferences pref;
	s >> pref;

	// the default preferences
	UserPreferences defaults;
	defaults.setDefaults();

	// make sure they have been defaulted as the armBandPort field is a string
	assert_equals(pref.ipAddress, defaults.ipAddress);
	assert_equals(pref.name, defaults.name);
	assert_equals(pref.armBandPort, defaults.armBandPort);
	assert_equals(pref.jacketPort, defaults.jacketPort);
	assert_equals(pref.muted, defaults.muted);
	assert_equals(pref.volume, defaults.volume);

	// write the preferences to streams
	ostringstream actualStream;
	actualStream << pref;

	ostringstream defaultStream;
	defaultStream << defaults;

	// make sure they have been defaulted
	assert_equals(actualStream.str(), defaultStream.str());
}

void CommonTestSuite::testMissingPreferences()
{
	const string prefString = "Name=Player18\nArmBandPort=abc\nJacketPort=67\nMuted=0\nVolume=1\n";

	// create the preferences with a missing field (IpAddress)
	stringstream s;
	s << prefString;

	// read them from the stream
	UserPreferences pref;
	s >> pref;

	// the default preferences
	UserPreferences defaults;
	defaults.setDefaults();

	// make sure they have been defaulted as the IpAddress field is missing
	assert_equals(pref.ipAddress, defaults.ipAddress);
	assert_equals(pref.name, defaults.name);
	assert_equals(pref.armBandPort, defaults.armBandPort);
	assert_equals(pref.jacketPort, defaults.jacketPort);
	assert_equals(pref.muted, defaults.muted);
	assert_equals(pref.volume, defaults.volume);

	// write the preferences to streams
	ostringstream actualStream;
	actualStream << pref;

	ostringstream defaultStream;
	defaultStream << defaults;

	// make sure they have been defaulted
	assert_equals(actualStream.str(), defaultStream.str());
}
