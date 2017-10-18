#include <gtest/gtest.h>
#include <generics/Thread.h>
#include <generics/Singleton.h>

namespace WPEFramework {
namespace Core {
namespace Test {

class UnitThread : public WPEFramework::Core::Thread {
public:
    UnitThread(const TCHAR* threadName = NULL)
    : Thread(0, threadName)
    , _delay(WPEFramework::Core::infinite)
    , _counter(0)
    , _hasFired(false)
    {
    }
    virtual ~UnitThread()
    {
    }

    /*virtual*/ uint32 Worker() override
    {
        _hasFired = true;
        _counter++;
        // Block as otherwise we will immediately be called again.
        Block();
        return _delay;
    }

public:
    uint32 _delay;
    uint32 _counter;
    bool _hasFired;
};

class ThreadTest : public ::testing::Test {
protected:
    virtual void SetUp() {}

    virtual void TearDown()
	{
		WPEFramework::Core::Singleton::Dispose();
	}
};

TEST_F(ThreadTest, Creation)
{
    UnitThread thread("unit_thread");

    EXPECT_FALSE(thread._hasFired) << "Thread fired at creation";
}

TEST_F(ThreadTest, StartStop)
{
    UnitThread thread("unit_thread");

    bool isRunning = thread.IsRunning();
    EXPECT_FALSE(isRunning) << "Thread started in running state";

    EXPECT_FALSE(thread._hasFired) << "Thread fired at creation";

    // Set thread in single-shot mode by returning a 'wait-forever' delay
    thread._delay = WPEFramework::Core::infinite;

    thread.Run();
    isRunning = thread.IsRunning();
    // Check if thread start was successful
    EXPECT_TRUE(isRunning);

    // Sleep for 100ms giving thread time to process
    ::SleepMs(100);

    EXPECT_TRUE(thread._hasFired) << "Thread did not fire";
    EXPECT_EQ(static_cast<uint32>(1), thread._counter) << "Thread did not fire once only";

    std::cout << "# Thread fired " << thread._counter << " times" << std::endl;

    thread.Block();
    isRunning = thread.IsRunning();
    // Check if thread stopped
    EXPECT_FALSE(isRunning);
}

TEST_F(ThreadTest, InfLoop)
{
    UnitThread thread("unit_thread");

    bool isRunning = thread.IsRunning();
    EXPECT_FALSE(isRunning) << "Thread started in running state";

    EXPECT_FALSE(thread._hasFired) << "Thread fired at creation";

    // Set thread in loop mode by returning a 'wait-forever' delay
    thread._delay = 0;

    thread.Run();
    isRunning = thread.IsRunning();
    // Check if thread start was successful
    EXPECT_TRUE(isRunning);

    // Sleep for 100ms giving thread time to process
    ::SleepMs(100);

    EXPECT_TRUE(thread._hasFired) << "Thread did not fire";
    EXPECT_GE(thread._counter, uint32{1}) << "Thread did not fire at least once";

    std::cout << "# Thread fired " << thread._counter << " times" << std::endl;

    thread.Block();
    isRunning = thread.IsRunning();
    // Check if thread stopped
    EXPECT_FALSE(isRunning);
}

} // namespace Test
} // namespace Core
} // namespace WPEFramework
