#ifndef UNIT_TEST_MACROS_H
#define UNIT_TEST_MACROS_H

#define UNIT_TEST(x)													\
	{																	\
		++totalTests;													\
		setup();														\
		try			 													\
		{																\
			##x();														\
			std::string message = #x;									\
			message += " : SUCCESS";									\
			results.push_back(message);									\
		}																\
		catch (TestFailedException& ex)									\
		{																\
			++failedTests;												\
			std::string message = #x;									\
			message += " : FAILED - ";									\
			message += ex.what();										\
			results.push_back(message);									\
		}																\
		tearDown();														\
	}

#endif