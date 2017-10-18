#include <gtest/gtest.h>
#include <cryptalgo/cryptalgo.h>

namespace WPEFramework {
namespace Core {
namespace Test {

class HashTest  : public ::testing::Test {
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

TEST_F(HashTest, SHA224)
{
    WPEFramework::Crypto::SHA224 sha224;
    sha224.Input(reinterpret_cast<const uint8*>(_T("")), 0);

    std::string output;
    ToHexString(sha224.Result(), sha224.Length, output);

    // 0xd14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f
    std::string expected("d14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f");
    EXPECT_EQ(expected, output);
}

TEST_F(HashTest, SHA256Test)
{
    WPEFramework::Crypto::SHA256 sha256;
    sha256.Input(reinterpret_cast<const uint8*>(_T("")), 0);

    std::string output;
    ToHexString(sha256.Result(), sha256.Length, output);

    // 0xe3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
    std::string expected("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    EXPECT_EQ(expected, output);
}
    
TEST_F(HashTest, SHA384Test)
{
    WPEFramework::Crypto::SHA384 sha384;
    sha384.Input(reinterpret_cast<const uint8*>(_T("")), 0);

    std::string output;
    ToHexString(sha384.Result(), sha384.Length, output);

    // 0x38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b
    std::string expected("38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b");
    EXPECT_EQ(expected, output);
}
    
TEST_F(HashTest, SHA512Test)
{
    WPEFramework::Crypto::SHA512 sha512;
    sha512.Input(reinterpret_cast<const uint8*>(_T("")), 0);

    std::string output;
    ToHexString(sha512.Result(), sha512.Length, output);

    // 0xcf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e
    std::string expected("cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e");
    EXPECT_EQ(expected, output);
}

} // namespace Test
} // namespace Core
} // namespace WPEFramework
