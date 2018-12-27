#include <gtest/gtest.h>

#include <core/vector01.h>

using namespace std;

TEST(Core_Vector, CheckContents01)
{
   vector<int> theVector = getVector01();

   ASSERT_EQ(theVector.size(), 4);

   for (int i = 0; i < theVector.size(); i++) {
      EXPECT_EQ(theVector[i], i) << "The vector is incorrect at index " << i << ": " << theVector[i];
   }

}
