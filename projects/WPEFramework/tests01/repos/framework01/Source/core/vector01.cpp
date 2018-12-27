#include "vector01.h"

using namespace std;

vector<int> getVector01()
{
	vector<int> output({0, 1, 2, 3});

	// This line will should a failing EXPECT_* (instead of ASSERT)
	//output[2] = 5;

	return output;
}
