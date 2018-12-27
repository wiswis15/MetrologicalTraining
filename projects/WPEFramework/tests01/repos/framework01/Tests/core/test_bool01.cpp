#include <gtest/gtest.h>

#include <core/bool01.h>

TEST(Core_Bool, TestTrue)
{
   ASSERT_TRUE(getTrue01());
}

TEST(Core_Bool, TestFalse)
{
   ASSERT_FALSE(getFalse01());
}
