#include "../../Source/core/core.h"

#include <iostream>
using namespace std;
using namespace WPEFramework::Core;

// When an instance of this class goes out of scope, it will call Dispose on Singleton.
class SingletonTrigger
{
public:
   ~SingletonTrigger() {
      Singleton::Dispose();
   }
};

int main(int argc, const char ** argv)
{
   // Make sure Singleton::Dispose() will be called when main is exited. 
   SingletonTrigger st;

   if (argc != 2) {
      cerr << "Usage: " << argv[0] << " [mapped file path]" << endl;
      cerr << "For example: " << argv[0] << " /tmp/testbuffer01" << endl;
      return 1;
   }
   
   const char * bufferPath = argv[1];
   
   CyclicBuffer buffer(bufferPath, 0, false);
   if (buffer.IsValid() == false) {
      cerr << "Failed to open Memory Mapped File, error: " << buffer.ErrorCode() << endl;
      return 1;
   }
   
   uint32 used = buffer.Used();
   
   cout << "Size: " << buffer.Size() << endl;
   cout << "Used: " << used << endl;
   cout << "Free: " << buffer.Free() << endl;
   cout << "Head: " << buffer._administration->_head.load() << endl;
   cout << "Tail: " << buffer._administration->_tail.load() << endl;

   uint8 contents[used + 1];
   uint32 result = buffer.Peek(contents, used);
   contents[result] = '\0';

   if (result != used) {
      cout << "Warning: expected " << used << " chars, but got " << result << endl;
   }

   cout << "Contents:" << endl;
   cout << contents << endl;

   return 0;
}

