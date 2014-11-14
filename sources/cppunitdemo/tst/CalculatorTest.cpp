#include "CalculatorTest.h"
#include "Calculator.h"


CalculatorTest::CalculatorTest () : mCalc (0)
{
}

CalculatorTest::~CalculatorTest () {
    if (mCalc != 0) {
		delete mCalc;
		mCalc = 0;
	}
}

void CalculatorTest::setUp () {
	mCalc = new Calculator ();
	CPPUNIT_ASSERT (mCalc != 0);
}

void CalculatorTest::tearDown () {
	delete mCalc;
	mCalc = 0;
}

void CalculatorTest::AdditionTest () {
	int result = mCalc->Addition (2, 3);
	CPPUNIT_ASSERT_EQUAL (5, result);
}

void CalculatorTest::SubtractTest () {
	int result = mCalc->Subtract (2, 3);
	CPPUNIT_ASSERT_EQUAL (-1, result);
}

void CalculatorTest::MultiplyTest () {
	int result = mCalc->Multiply (2, 3);
	CPPUNIT_ASSERT_EQUAL (6, result);
}

void CalculatorTest::DivideTest () {
	int result = mCalc->Divide (2, 3);
	CPPUNIT_ASSERT_EQUAL_MESSAGE ("Too bad results are not the same!", 1, result);
}

void CalculatorTest::PowerTest () {
	int result = mCalc->Power (2, 3);
	CPPUNIT_ASSERT_EQUAL_MESSAGE ("Too bad results are not the same!", 1, result);
}
