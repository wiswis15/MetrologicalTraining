#include <gtest/gtest.h>
#include <generics/Singleton.h>

namespace WPEFramework {
namespace Core {
namespace Test {

class TestSingleton {
public:
    TestSingleton()
    : _wasCreated(false){};

    bool _wasCreated;

private:
};

class SingletonTest : public ::testing::Test {
protected:
    virtual void SetUp() {}

    virtual void TearDown() {}
};

TEST_F(SingletonTest, Creation)
{
    static TestSingleton& single = WPEFramework::Core::SingletonType<TestSingleton>::Instance();
    EXPECT_FALSE(single._wasCreated);
}

} // namespace Test
} // namespace Core
} // namespace WPEFramework
