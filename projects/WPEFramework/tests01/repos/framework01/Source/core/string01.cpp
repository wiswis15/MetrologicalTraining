#include "string01.h"

#include <string.h>

void appendStrings01(const char a[], const char b[], char out[])
{
   // https://stackoverflow.com/questions/5901181/c-string-append
   out[0] = '\0';
   strcat(out, a);
   strcat(out, b);
}
