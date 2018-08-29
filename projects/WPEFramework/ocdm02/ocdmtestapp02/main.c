#include <ocdm-accessor/open_cdm.h>

#include <stdio.h>
#include <stdlib.h>

// To store key ID we get in callback.
uint8_t * g_keyId = NULL;
uint8_t g_keyLength = 0;

int main()
{
   const char keySystem[] = "com.metrological.null";
   OpenCDMError isSupported = opencdm_is_type_supported(keySystem, "");
   fprintf(stderr, "com.metrological.null is supported (should be 0): %u\n", isSupported);

   isSupported = opencdm_is_type_supported("foobar", "");
   fprintf(stderr, "foobar is supported (should be 1): %u\n", isSupported);
   
}

