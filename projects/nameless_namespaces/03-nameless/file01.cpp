#include <iostream>
using namespace std;

namespace {

static class Class01 {
public:
   Class01() {
      cout << "Class01 ctor: " << __FILE__ << endl;
   }
} staticInstance;

}

