#include <core/core.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <iostream>
#include <fstream>
#include <openssl/modes.h>

using namespace std;

// TODO: is the ESN always the same length?
const uint32_t g_esnSize = 42;

const uint8_t g_key[16] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 
                            0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x11 };

// TODO: m_ prefix
struct NetflixData
{
    uint8_t salt[16];
    uint8_t kpe[16];
    uint8_t kph[32];
    uint8_t esn[g_esnSize];
} __attribute__((packed));

// TODO: check buffer overrun
void DecodeBase64(const char input[], uint8_t output[], uint32_t outputSize)
{
	BIO *bio, *b64;

	int inputLength = strlen(input);

	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new_mem_buf(input, inputLength);
	bio = BIO_push(b64, bio);
	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
	BIO_read(bio, output, inputLength);

	BIO_free_all(bio);
}

void AesEncrypt(const uint8_t input[], uint8_t output[], uint32_t bufferSize, uint8_t iv[16])
{
   uint8_t localIv[16];
   memcpy(localIv, iv, 16);
   AES_KEY aesKey;
   AES_set_encrypt_key(g_key, 128, &aesKey);
   AES_cbc_encrypt(input, output, bufferSize, &aesKey, localIv, AES_ENCRYPT);
}

void runAes(const uint8_t input[], uint8_t output[], uint32_t bufferSize, const uint8_t iv[16])
{
   unsigned char localIv[AES_BLOCK_SIZE];
   memcpy(localIv, iv, 16);

   unsigned char ecount[AES_BLOCK_SIZE];
   memset(ecount, 0, 16); 

   unsigned int num = 0;

   AES_KEY aesKey;
   AES_set_encrypt_key(g_key, 128, &aesKey);
   CRYPTO_ctr128_encrypt(input, output, bufferSize, &aesKey, localIv, ecount, &num, reinterpret_cast<block128_f>(AES_encrypt));
}

/*
void AesDecrypt(const uint8_t input[], uint8_t output[], uint32_t bufferSize, uint8_t iv[16])
{
   uint8_t localIv[16];
   memcpy(localIv, iv, 16);
   AES_KEY aesKey;
   AES_set_decrypt_key(g_key, 128, &aesKey);
   AES_cbc_encrypt(input, output, bufferSize, &aesKey, localIv, AES_DECRYPT);
}
*/

// TODO: argv as array
// TODO: better error return codes (standard linux)
int main(int argc, const char ** argv)
{
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " [input file] [output file]" << endl;
        cerr << "Example:" << endl;
        cerr << "   " << argv[0] << " idfile netflix-vault.bin" << endl;
        return 1;
    }

    // AES needs 
    //uint32_t plainBufferSize = sizeof(NetflixData);
    //if ()

    NetflixData netflixData;

    srand(time(nullptr));
    uint8_t iv[16];
    for (int i = 0; i < 16; i++) {
        iv[i] = rand() % 256;
        netflixData.salt[i] = rand() % 256;
    }

    ifstream inFile(argv[1]);
    if (!inFile) {
        cerr << "Failed to open " << argv[1] << " for reading" << endl;
        return 2;
    }

    string line;

    std::getline(inFile, line);
    if (line.length() != g_esnSize) {
        cerr << "Expected ESN to be " << g_esnSize << " chars long, got " << line.length() << " instead." << endl;
        return 3;
    }

    strncpy(reinterpret_cast<char *>(netflixData.esn), line.c_str(), g_esnSize);

    const uint32_t expectedKpeB64Length = 24;
    std::getline(inFile, line);
    if (line.length() != expectedKpeB64Length) {
        cerr << "Expected KPE to be " << expectedKpeB64Length << " chars long, got " << line.length() << " instead." << endl;
        return 4;
    }

    DecodeBase64(line.c_str(), netflixData.kpe, sizeof(netflixData.kpe));

    const uint32_t expectedKphB64Length = 44;
    std::getline(inFile, line);
    if (line.length() != expectedKphB64Length) {
        cerr << "Expected KPH to be " << expectedKphB64Length << " chars long, got " << line.length() << " instead." << endl;
        return 5;
    }

    DecodeBase64(line.c_str(), netflixData.kph, sizeof(netflixData.kph));

    uint8_t encryptBuffer[sizeof(NetflixData)];
    memset(encryptBuffer, 0, sizeof(encryptBuffer));
    //AesEncrypt(reinterpret_cast<uint8_t *>(&netflixData), encryptBuffer, sizeof(NetflixData), iv);
    runAes(reinterpret_cast<uint8_t *>(&netflixData), encryptBuffer, sizeof(NetflixData), iv);

    ofstream outFile(argv[2], ofstream::binary);
    if (!outFile) {
        cerr << "Failed to open " << argv[2] << " for writing" << endl;
        return 6;
    }

    outFile.write(reinterpret_cast<const char *>(iv), sizeof(iv));
    outFile.write(reinterpret_cast<const char *>(&netflixData), sizeof(netflixData));

    outFile.close();
    inFile.close();

    cout << "SUCCESS: Written netflix fault to " << argv[2] << endl;
    return 0;
}
