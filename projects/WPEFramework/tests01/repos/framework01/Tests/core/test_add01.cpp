#include <gtest/gtest.h>

#include <core/add01.h>

TEST(Core_Add, AddTwoPositive)
{
   int a = 12;
   int b = 20;

   int sum = add01(a, b);

   ASSERT_EQ(sum, 32);
}

TEST(Core_Add, AddPositiveNegative)
{
   int a = 8;
   int b = -18;

   int sum = add01(a, b);

   ASSERT_EQ(sum, -10);
}

TEST(Core_Add, AddTwoNegative)
{
   int a = -8;
   int b = -10;

   int sum = add01(a, b);

   ASSERT_EQ(sum, -18);
}

// This test would show the possibility of printing a custom string
/*
TEST(Core_Add, AddTwoPositiveFail)
{
   int a = 12;
   int b = 20;

   int sum = add01(a, b);

   ASSERT_EQ(sum, 33) << "This is a string from test_add01.cpp file";
}
*/

TEST(Core_Add, AddTwoPositive_NE)
{
   int a = 12;
   int b = 20;

   int sum = add01(a, b);

   ASSERT_NE(sum, 33);
}

// Less than
TEST(Core_Add, AddTwoPositive_LT)
{
   int a = 12;
   int b = 20;

   int sum = add01(a, b);

   ASSERT_LT(sum, 33);
}

// Less than / equal
TEST(Core_Add, AddTwoPositive_LE)
{
   int a = 12;
   int b = 20;

   int sum = add01(a, b);

   ASSERT_LE(sum, 33);
}

// Greater than
TEST(Core_Add, AddTwoPositive_GT)
{
   int a = 12;
   int b = 20;

   int sum = add01(a, b);

   ASSERT_GT(sum, 31);
}

// Greater than / equal
TEST(Core_Add, AddTwoPositive_GE)
{
   int a = 12;
   int b = 20;

   int sum = add01(a, b);

   ASSERT_GE(sum, 31);
}
