#ifndef PERSPECTIVE_MATH_TEST_SUITE_H
#define PERSPECTIVE_MATH_TEST_SUITE_H

#include "stdafx.h"
#include "TestSuite.h"
#include "PerspectiveMath.h"

/**
 * Tests the perspective math utility class
 */
class PerspectiveMathTestSuite : public TestSuite
{
public:
	/**
	 * Constructor.
	 * @param name The name of the test suite as it will appear in the test results
	 */
	PerspectiveMathTestSuite(const std::string& name = "");

	/**
	 * Destructor
	 */
	~PerspectiveMathTestSuite();

	/**
	 * Run all tests.
	 */
	virtual void run();

private:

	/**
	 * Tests the invert 2D coordinate function
	 */
	void testInvert2DCoordinate();

	/**
	 * Tests the invert 3D coordinate function
	 */
	void testInvert3DCoordinate();

	/**
	 * Tests the invert 3D projectile function
	 */
	void testInvert3DProjectile();

	/**
	 * Tests the convert orientation XYZ to YZX function
	 */
	void testConvertOrientationXYZtoYZX();

	/**
	 * Tests the convert box to box function
	 */
	void testConvertBoxToBox_n1();

	/**
	 * Tests the convert box to box function
	 */
	void testConvertBoxToBox_n2();

	/**
	 * Tests the convert box to box function
	 */
	void testConvertBoxToBox_n3();
};

#endif
