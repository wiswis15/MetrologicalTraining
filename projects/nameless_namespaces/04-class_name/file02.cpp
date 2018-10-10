#include <iostream>
using namespace std;

static class Class02 {
public:
   Class02() {
      cout << "Class02 ctor: " << __FILE__ << endl;
   }
} staticInstance;

