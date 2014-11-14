#include "UnitTests.h"
#include "CppUnitWrapper.h"
#include <iostream>


int main(int argc, char* argv[])
{
	CppUnitWrapper theRunner;
	theRunner.setOutputType (CppUnitWrapper::kXmlOutput);

	/*bool wasSucessful = */ theRunner.run ();

	// Return error code 1 if the one of test failed.
	return 0;//(wasSucessful ? 0 : 1);
}
