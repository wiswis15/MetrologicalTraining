#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <openssl/aes.h>
#include <dirent.h>
#include <linux/limits.h>
#include <stdlib.h>

// compile: gcc main.c -o parser -lcrypto
uint8_t _SecStoreAesKey[16] = { 0xd8, 0x95, 0x7a, 0x33, 0x14, 0x4d, 0xe9, 0x30,
                                0x02, 0xcd, 0x32, 0x89, 0x6e, 0x57, 0xdf, 0xfc }; 
                                
void processFile(const char filePath[], FILE * outFile)
{
   uint8_t iv[16];
   memset(iv, 0, sizeof(iv));

   FILE * inFile = fopen(filePath, "rb");
   fseek(inFile, 0, SEEK_END);
   long fileSize = ftell(inFile);
   const int dataOffset = 44;
   fseek(inFile, dataOffset, SEEK_SET);
   int inputDataSize = fileSize - dataOffset;
   uint8_t * encryptedData = malloc(inputDataSize);
   fread(encryptedData, inputDataSize, 1, inFile);
   fclose(inFile);

   uint8_t * decryptedData = malloc(2 * inputDataSize);
   memset(decryptedData, 0, 2 * inputDataSize);
   int len = inputDataSize;

   AES_KEY LocalKey;
   AES_set_decrypt_key(_SecStoreAesKey, 128, &LocalKey);
   AES_cbc_encrypt(encryptedData, decryptedData, inputDataSize, &LocalKey, iv, AES_DECRYPT);
   
   const int esnStart = 0x78;
   const int esnLength = 42;
   char esnBuffer[esnLength + 1];
   memcpy(esnBuffer, decryptedData + esnStart, esnLength);
   esnBuffer[esnLength] = '\0';
   fprintf(outFile, "%s\t%s\n", filePath, esnBuffer);
   fflush(outFile);

   free(decryptedData);
   free(encryptedData);
}

int main(int argc, const char * argv[])
{
   if (argc != 3) {
      fprintf(stderr, "Usage: %s [input dir] [output file]\n", argv[0]);
      return 1;
   }
   
   const char * inputDir = argv[1];
   const char * outputFileName = argv[2];
   
   printf("Counting files in directory...\n");
   int fileCount = 0;

   DIR *d;
   struct dirent *dir;
   d = opendir(inputDir);
   if (d) {
     while ((dir = readdir(d)) != NULL) {
       if (dir->d_type == DT_REG) {
         fileCount++;
       }
     }
     closedir(d);
   }
   
   printf("Found %d files.\n", fileCount);
   
   int fileIndex = 1;
   FILE * outFile = fopen(outputFileName, "w");
   
   d = opendir(inputDir);
   if (d) {
     while ((dir = readdir(d)) != NULL) {
       if (dir->d_type == DT_REG) {
         printf("[%d/%d] %s\n", fileIndex, fileCount, dir->d_name);
         fileIndex++;
         
         char fullPathBuffer[PATH_MAX];
         sprintf(fullPathBuffer, "%s/%s", inputDir, dir->d_name);
         processFile(fullPathBuffer, outFile);
       }
     }
     closedir(d);
   }

   fclose(outFile);
}
