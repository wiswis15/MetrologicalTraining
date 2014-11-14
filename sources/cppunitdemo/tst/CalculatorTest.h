#ifndef	CALCULATOR_TESTER_H
#define	CALCULATOR_TESTER_H

#include "cppunit/extensions/HelperMacros.h"


class Calculator;

class CalculatorTest : public CPPUNIT_NS::TestFixture
{
public:
	CalculatorTest ();
	virtual ~CalculatorTest ();

public:
	virtual void setUp ();
	virtual void tearDown ();

public:
    // Declare test suite and cases
	CPPUNIT_TEST_SUITE (CalculatorTest);
	CPPUNIT_TEST (AdditionTest);
	CPPUNIT_TEST (SubtractTest);
	CPPUNIT_TEST (MultiplyTest);
	CPPUNIT_TEST (DivideTest);
	//CPPUNIT_TEST (PowerTest);
	CPPUNIT_TEST_SUITE_END ();

private:
	void AdditionTest (void);
	void SubtractTest (void);
	void MultiplyTest (void);
	void DivideTest   (void);
	void PowerTest    (void);

private:
	Calculator* mCalc;
};

#endif	// CALCULATOR_TESTER_H
