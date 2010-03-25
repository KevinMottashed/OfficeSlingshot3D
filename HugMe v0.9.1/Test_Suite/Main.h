// Main.h: interface for the Main class.
//
//////////////////////////////////////////////////////////////////////

#include "cxxtest/TestSuite.h"
class Main : public CxxTest::TestSuite
{
	public:
		void testAddition( void )
		{
			TS_ASSERT( 1 + 1 > 1 );
			TS_ASSERT_EQUALS( 1 + 1, 2 );
		}
		void testJacket( void )
		{
			TS_ASSERT( 1 + 1 > 1 );
		}
};