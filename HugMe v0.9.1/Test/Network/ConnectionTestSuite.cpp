#include "ConnectionTestSuite.h"

#include "boost.h"

ConnectionTestSuite::ConnectionTestSuite(const std::string& name) :
	TestSuite(name)
{
}

ConnectionTestSuite::~ConnectionTestSuite()
{
}

void ConnectionTestSuite::run()
{
	UNIT_TEST(testListen);
}

void ConnectionTestSuite::testListen()
{
	WinsockNetwork playerA;
	assert_equals(playerA.isListening(), false, "The network should not be listening when initialized");		

	// 1. Player A Listens	
	rc_network error = playerA.listen("Player A");
	assert_equals(error, SUCCESS, "The listen should be successful");
	assert_equals(playerA.isListening(), true, "The network should be listening");

	// 2. Player A disconnects
	error = playerA.disconnect();
	assert_equals(error, SUCCESS, "The disconnect should be successful");
	assert_equals(playerA.isListening(), false, "The network should no longer be listening");

	return;
}


