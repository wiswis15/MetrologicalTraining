#include <gtest/gtest.h>

#include <vector>

class VectorTest02 : public testing::Test
{
protected:
   // Watch out with spelling (use "override" to make sure is spelled correctly)
   void SetUp() override
   {
      m_V2.push_back(1);
      m_V2.push_back(2);
   }

// Members cannot be private, need to be at least protected
//private:
   std::vector<int> m_V1;
   std::vector<int> m_V2;
};

TEST_F(VectorTest02, V1IsEmpty)
{
   ASSERT_EQ(m_V1.size(), 0);
}

TEST_F(VectorTest02, V2Contains12)
{
   ASSERT_EQ(m_V2.size(), 2);
   ASSERT_EQ(m_V2[0], 1);
   ASSERT_EQ(m_V2[1], 2);
}
