#include <WPEFramework/core/core.h>
#include <pthread.h>

using namespace WPEFramework;

void func02()
{
   while (true) {}
}

void func01()
{
   func02();
}

void * otherThreadMain(void * args)
{
   func01();
}

int main()
{
   pthread_t threadId;

   pthread_create(&threadId, NULL, otherThreadMain, NULL);

   const int bufferSize = 20;
   void * addresses[bufferSize];

   sleep(1);

   uint32_t usedCount = GetCallStack(threadId, &addresses[0], bufferSize);
fprintf(stderr, __FILE__ ":%u: %u\n", __LINE__, usedCount);

   fprintf(stderr, "-----------------------\n");
   backtrace_symbols_fd(addresses, usedCount, fileno(stderr));
   fprintf(stderr, "-----------------------\n");
   fflush(stderr);

   Core::Singleton::Dispose();

}