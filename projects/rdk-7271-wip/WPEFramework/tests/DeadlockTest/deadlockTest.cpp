#include "../../generics/generics.h"

#include <iostream>

using namespace WPEFramework::Core;

CriticalSection g_mutex1;
CriticalSection g_mutex2;

class Thread1 : public Thread {
public:
    Thread1()
        : m_runned(false)
    {
    }

    uint32 Worker()
    {
        if (m_runned) {
            return WPEFramework::Core::infinite;
        }

        g_mutex1.Lock();
        sleep(1);
        g_mutex2.Lock();

        std::cout << "In worker of thread 1" << std::endl;

        m_runned = true;

        return WPEFramework::Core::infinite;
    }

public:
    bool m_runned;
};

class Thread2 : public Thread {
public:
    Thread2()
        : m_runned(false)
    {
    }

    uint32 Worker()
    {
        if (m_runned) {
            return WPEFramework::Core::infinite;
        }

        g_mutex2.Lock();
        sleep(10000);
        //g_mutex1.Lock();

        std::cout << "In worker of thread 2" << std::endl;

        m_runned = true;

        return WPEFramework::Core::infinite;
    }

public:
    bool m_runned;
};

int main()
{
    Thread1 thread1;
    Thread2 thread2;

    thread1.Run();
    thread2.Run();

    //thread1.Wait(Thread::STOPPED, WPEFramework::Core::infinite);
    //sleep(3);
    ::SleepMs(300);

    std::cout << thread1.IsRunning() << std::endl;

    thread1.Block();
    thread2.Block();

    return 0;
}
