#include <gtest/gtest.h>
#include <cryptalgo/cryptalgo.h>

namespace WPEFramework {
namespace Core {
namespace Test {

class HMACTest  : public ::testing::Test {
protected:
    virtual void SetUp() {}

    virtual void TearDown() {}
};

static void ToHexString(const uint8 hash[], const uint8 size, std::string& result)
{
    char hexBuffer[3];
    for (uint8 teller = 0; teller < size; teller++) {
        sprintf(hexBuffer, "%02x", hash[teller]);
        result += hexBuffer;
    }
}

TEST_F(HMACTest, HMAC_SHA256)
{
    //SHA512/256("")
    //0xf7bc83f430538424b13298e6aa6fb143ef4d59a14946175997479dbc2d1a3cd8
    std::string expected("f7bc83f430538424b13298e6aa6fb143ef4d59a14946175997479dbc2d1a3cd8");
    const char testText[] = "The quick brown fox jumps over the lazy dog";

    WPEFramework::Crypto::SHA256HMAC hmacSHA256("key");
    hmacSHA256.Input(reinterpret_cast<const uint8*>(testText), static_cast<uint16>(strlen(testText)));

    std::string output;
    ToHexString(hmacSHA256.Result(), hmacSHA256.Length, output);

    EXPECT_EQ(expected, output);
}

// TODO: requires the expected HMAC of the text2 variable to be obtained elsewhere
//TEST_F(HMACTest, HMAC)
//{
//    std::string expected();
//    const char testText[] = "{\"id\":\"test\",\"price\":1,\"summary\":\"With a Tab <\t> and spaces\"}";
//
//    WPEFramework::Crypto::SHAHMAC hmacSHA("key");
//    hmacSHA.Input(reinterpret_cast<const uint8*>(testText), strlen(testText));
//
//    std::string output;
//    ToHexString (hmacSHA.Result(), hmacSHA.Length, output);
//
//    EXPECT_EQ (expected, output);
//}

} // namespace Test
} // namespace Core
} // namespace WPEFramework
