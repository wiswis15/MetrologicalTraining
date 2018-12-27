#include <gtest/gtest.h>

#include <core/string01.h>

#include <string>

TEST(Core_String, TestEqual)
{
   const char a[] = "Hello ";
   const char b[] = "World";
   char out[64];

   appendStrings01(a, b, out);
   ASSERT_STREQ(out, "Hello World");
}

TEST(Core_String, TestNotEqual)
{
   const char a[] = "Hello ";
   const char b[] = "World";
   char out[64];

   appendStrings01(a, b, out);
   ASSERT_STRNE(out, "Hello World2");
}

TEST(Core_String, TestEqualCase)
{
   const char a[] = "Hello ";
   const char b[] = "World";
   char out[64];

   appendStrings01(a, b, out);
   ASSERT_STRCASEEQ(out, "hello world");
}

TEST(Core_String, TestNotEqualCase)
{
   const char a[] = "Hello ";
   const char b[] = "World";
   char out[64];

   appendStrings01(a, b, out);
   ASSERT_STRCASENE(out, "hello world2");
}

TEST(Core_String, TestEqualCpp)
{
   const char a[] = "Hello ";
   const char b[] = "World";
   char out[64];

   appendStrings01(a, b, out);

   std::string strOut(out);
   const std::string expected("Hello World");

   ASSERT_EQ(strOut, expected);
}
