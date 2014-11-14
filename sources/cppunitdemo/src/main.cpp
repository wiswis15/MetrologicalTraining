#include "Calculator.h"
#include <iostream>


int main(int argc, char* argv[])
{
	Calculator calc;

	int addition = calc.Addition (2, 3);
	std::cout << "2 + 3 = " << addition << std::endl;
	return 0;
}
