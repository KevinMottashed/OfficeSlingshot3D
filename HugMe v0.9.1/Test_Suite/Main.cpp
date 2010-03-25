/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ParenPrinter.h>

int main() {
 return CxxTest::ParenPrinter().run();
}
#include "Main.h"

static Main suite_Main;

static CxxTest::List Tests_Main = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_Main( "Main.h", 6, "Main", suite_Main, Tests_Main );

static class TestDescription_Main_testAddition : public CxxTest::RealTestDescription {
public:
 TestDescription_Main_testAddition() : CxxTest::RealTestDescription( Tests_Main, suiteDescription_Main, 9, "testAddition" ) {}
 void runTest() { suite_Main.testAddition(); }
} testDescription_Main_testAddition;

static class TestDescription_Main_testJacket : public CxxTest::RealTestDescription {
public:
 TestDescription_Main_testJacket() : CxxTest::RealTestDescription( Tests_Main, suiteDescription_Main, 14, "testJacket" ) {}
 void runTest() { suite_Main.testJacket(); }
} testDescription_Main_testJacket;

#include <cxxtest/Root.cpp>
