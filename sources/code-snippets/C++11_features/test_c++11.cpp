#include <cstdio>
#include "test_c++11.h"

int main(int argc, char * argv[])
{
    B b;
    printf("B foo: %d\n", b.foo());
    printf("B bar: %d\n", b.bar());

    C c;
    printf("C foo: %d\n", c.foo());
    printf("C bar: %d\n", c.bar());
    return 0;
}