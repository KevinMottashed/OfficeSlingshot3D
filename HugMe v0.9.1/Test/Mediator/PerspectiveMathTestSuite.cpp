#include "PerspectiveMathTestSuite.h"

#include "chai3d.h"

PerspectiveMathTestSuite::PerspectiveMathTestSuite(const std::string& name) :
	TestSuite(name)
{

}

PerspectiveMathTestSuite::~PerspectiveMathTestSuite()
{

}

void PerspectiveMathTestSuite::run()
{
	UNIT_TEST(testInvert2DCoordinate);
	UNIT_TEST(testInvert3DCoordinate);
	UNIT_TEST(testInvert3DProjectile);
	UNIT_TEST(testConvertOrientationXYZtoYZX);
	UNIT_TEST(testConvertBoxToBox_n1);
	UNIT_TEST(testConvertBoxToBox_n2);
	UNIT_TEST(testConvertBoxToBox_n3);
}

void PerspectiveMathTestSuite::testInvert2DCoordinate()
{
	cVector3d actual(0.0, 3.4, 2.3);
	PerspectiveMath::invert2DCoordinate(actual);

	cVector3d expected(0.0, 3.4, -2.3);
	assert_true(actual.equals(expected, 0.001f));
	return;
}

void PerspectiveMathTestSuite::testInvert3DCoordinate()
{
	cVector3d actual(-1.2, 3.4, 2.3);
	PerspectiveMath::invert3DCoordinate(actual);

	cVector3d expected(1.2, 3.4, -2.3);
	assert_true(actual.equals(expected, 0.001f));
	return;
}

void PerspectiveMathTestSuite::testInvert3DProjectile()
{
	Projectile actual(cVector3d(-1.2, 3.4, 2.3), cVector3d(-2.7, 1.4, 9.3));
	PerspectiveMath::invert3DProjectile(actual);

	Projectile expected(cVector3d(1.2, 3.4, -2.3), cVector3d(2.7, 1.4, -9.3));
	assert_true(actual.equals(expected, 0.001f));
	return;
}

void PerspectiveMathTestSuite::testConvertOrientationXYZtoYZX()
{
	cVector3d actual(1.0, 2.0, 3.0);
	PerspectiveMath::convertOrientationXYZtoYZX(actual);

	cVector3d expected(2.0, 3.0, 1.0);
	assert_true(actual.equals(expected, 0.001f));
	return;
}

void PerspectiveMathTestSuite::testConvertBoxToBox_n1()
{
	cVector3d actual(1.0, 4.0, 3.0);
	cCollisionAABBBox origin(cVector3d(0.0, 0.0, 0.0), cVector3d(5.0, 5.0, 5.0));
	cCollisionAABBBox target(cVector3d(0.0, 0.0, 0.0), cVector3d(10.0, 10.0, 10.0));
	PerspectiveMath::convertBoxToBox(actual, origin, target);
	cVector3d expected(2.0, 8.0, 6.0);
	assert_true(actual.equals(expected));
	return;
}

void PerspectiveMathTestSuite::testConvertBoxToBox_n2()
{
	cVector3d actual(1.0, 4.0, 3.0);
	cCollisionAABBBox origin(cVector3d(-5.0, -5.0, -5.0), cVector3d(5.0, 5.0, 5.0));
	cCollisionAABBBox target(cVector3d(0.0, 0.0, 0.0), cVector3d(10.0, 10.0, 10.0));
	PerspectiveMath::convertBoxToBox(actual, origin, target);
	cVector3d expected(6.0, 9.0, 8.0);
	assert_true(actual.equals(expected));
	return;
}

void PerspectiveMathTestSuite::testConvertBoxToBox_n3()
{
	cVector3d actual(1.0, -4.0, 3.0);
	cCollisionAABBBox origin(cVector3d(-5.0, -5.0, -5.0), cVector3d(5.0, 5.0, 5.0));
	cCollisionAABBBox target(cVector3d(-10.0, -10.0, -10.0), cVector3d(10.0, 10.0, 10.0));
	PerspectiveMath::convertBoxToBox(actual, origin, target);
	cVector3d expected(2.0, -8.0, 6.0);
	assert_true(actual.equals(expected));
	return;
}