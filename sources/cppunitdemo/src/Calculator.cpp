#include "Calculator.h"
#include <math.h>


Calculator::Calculator () {
}

Calculator::~Calculator () {
}

int Calculator::Addition (int arg1, int arg2) {
	return (arg1 + arg2);
}

int Calculator::Subtract (int arg1, int arg2) {
	return (arg1 - arg2);
}

int Calculator::Multiply (int arg1, int arg2) {
	return (arg1 * arg2);
}

int Calculator::Divide (int arg1, int arg2) {
	return (arg1 / arg2);
}

int Calculator::Power (int arg1, int arg2) {
	return static_cast<int> (pow (static_cast<double> (arg1), arg2));
}
