#include <stdio.h>
#include <unistd.h>

#include <core/core.h>
#include <iostream>

using namespace WPEFramework;
using namespace std;

Core::CriticalSection g_section1, g_section2;

void other04()
{
   cerr << "About to dead-lock in other thread" << endl;
   g_section2.Lock();
}

void other03()
{
   g_section1.Lock();
}

void other02()
{
   other03();
}

void* other01(void* ignored)
{
   other02();
   other04();
   return NULL;
}

int main()
{
   g_section2.Lock();
   pthread_t otherThread;
   pthread_create(&otherThread, nullptr, other01, nullptr);

   sleep(1);
   cerr << "About to dead-lock in main thread" << endl;
   g_section1.Lock();
}
