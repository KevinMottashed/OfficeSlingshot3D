#ifndef CONNECTION_TEST_SUITE_H
#define CONNECTION_TEST_SUITE_H

#include "stdafx.h"
#include "TestSuite.h"
#include "NetworkProxy.h"

/**
 * This test suite tests the network module.
 */
class ConnectionTestSuite : public TestSuite
{
public:
	ConnectionTestSuite(const std::string& name = "");
	~ConnectionTestSuite();

	// run all tests
	virtual void run();

private:

	// Why is there only one test case here?
	// Good question.
	// From what I've observed it is not possible for a process to listen for connections
	// and then to connect to itself. I couldn't find this in any documentation but I have
	// observed it to be true. It might be possible for a process to connect to itself 
	// but I don't know how. For that reason, all test requiring more then 1 peer cannot 
	// be tested without creating a test process. Creating this test process would require 
	// a lot of effort so for now it is not worth the effort seeing as how deadlines are approaching.

	/** Scenario:
	 *  1. Player A Listens
	 *  2. Player A Disconnects
	 */	
	void testListen();

};

#endif
