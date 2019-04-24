#include <core/core.h>

#include <iostream>

using namespace std;
using namespace WPEFramework;

class TestThread01 : public Core::Thread
{
public:
   TestThread01()
   {
      cerr << "In Thread01 Constructor: " << __FILE__ << ":" << __LINE__ << endl;

      // Perform required initialization here
   }

   ~TestThread01()
   {
      cerr << "In Thread01 Destructor: " << __FILE__ << ":" << __LINE__ << endl;

      Stop();

      cerr << "In Thread01 Destructor: " << __FILE__ << ":" << __LINE__ << endl;

      Wait(STOPPED, Core::infinite);
      cerr << "In Thread01 Destructor: " << __FILE__ << ":" << __LINE__ << endl;

      // Perform required deinitialization here
   }

protected:
   virtual uint32_t Worker()
   {
      cerr << "In Thread01 Worker: " << __FILE__ << ":" << __LINE__ << endl;

      // "Sleeping" for one second
      Block();
      return 1000;
   }
};

int main()
{
   TestThread01 * t01 = new TestThread01;

   cerr << __FILE__ << ":" << __LINE__ << endl;
   t01->Run();
   cerr << __FILE__ << ":" << __LINE__ << endl;

   sleep(5);
   cerr << __FILE__ << ":" << __LINE__ << endl;

   delete t01;
   cerr << __FILE__ << ":" << __LINE__ << endl;

   Core::Singleton::Dispose();

   cerr << __FILE__ << ":" << __LINE__ << endl;

   return 0;
}
