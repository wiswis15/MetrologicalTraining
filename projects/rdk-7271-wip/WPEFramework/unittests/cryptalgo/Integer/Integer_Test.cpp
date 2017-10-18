#include <iostream>
#include <cryptalgo/Integer.h>

using namespace std;
using namespace WPEFramework::Core;
using namespace WPEFramework::Crypto;

template <const unsigned int BITS>
void ShowValue(const Integer<BITS>& integer, const bool full)
{
    unsigned int bytes = (full ? integer.Limbs() : (integer.UsedLimbs() + 7) / 8);
    printf("Value: ");
    for (unsigned int index = 0; index < bytes; index++) {
        if (index < (bytes - 1)) {
            printf("%02X:", integer[bytes - index - 1]);
        }
        else {
            printf("%02X", integer[bytes - index - 1]);
        }
    }
    printf("\n");
}

//int main()
//{
//    Integer<512> rhs(1000);
//    Integer<0> lhs(500);
//
//    printf("Test 1\n");
//    ShowValue(rhs, true);
//    ShowValue(lhs, true);
//    assert(rhs > lhs);
//    assert(rhs >= lhs);
//    assert(rhs != lhs);
//
//    printf("Test 2\n");
//    printf("Used Bits: %d\n", rhs.UsedLimbs());
//    rhs += lhs;
//    printf("Used Bits: %d\n", rhs.UsedLimbs());
//    assert(rhs == Integer<16>(1500));
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == false);
//
//    printf("Test 3\n");
//    rhs = rhs + 500;
//    printf("Used Bits: %d\n", rhs.UsedLimbs());
//    assert(rhs.Compare(Integer<16>(2000)) == 0);
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == false);
//
//    printf("Test 4\n");
//    rhs -= 1000;
//    printf("Used Bits: %d\n", rhs.UsedLimbs());
//    assert(rhs == Integer<16>(1000));
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == false);
//
//    printf("Test 5\n");
//    rhs = rhs - 100;
//    printf("Used Bits: %d\n", rhs.UsedLimbs());
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == false);
//    assert(rhs == Integer<16>(900));
//
//    printf("Test 6\n");
//    rhs = rhs - 100;
//    rhs = 1000;
//    rhs *= 8000;
//    printf("Used Bits: %d\n", rhs.UsedLimbs());
//    assert(rhs == 8000000);
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == false);
//
//    printf("Test 7\n");
//    rhs = rhs * 10000;
//    ShowValue(rhs, true);
//    printf("Used Bits: %d\n", rhs.UsedLimbs());
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == false);
//    assert(rhs == 80000000000);
//
//    printf("Test 8\n");
//    rhs *= 8000;
//    printf("Used Bits: %d\n", rhs.UsedLimbs());
//    assert(rhs == Integer<64>(640000000000000));
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == false);
//
//    // This will result in an overflow, as the LHS is only 8 bits big.
//    // So the result of the multiplication will be tried to fit the
//    //Integr<8> definition, and thus too small..
//    printf("Test 8\n");
//    rhs = Integer<8>(4) * rhs;
//    assert(rhs.HasOverflow() == true);
//
//    printf("Test 9\n");
//    rhs = 50000000000;
//    rhs <<= 2;
//    assert(rhs == 200000000000);
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == false);
//
//    printf("Test 10\n");
//    rhs = rhs << 2;
//    assert(rhs == 800000000000);
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == false);
//
//    printf("Test 11\n");
//    rhs = rhs >> 1;
//    assert(rhs == 400000000000);
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == false);
//
//    printf("Test 12\n");
//    rhs >>= 11;
//    assert(rhs == 195312500);
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == false);
//
//    printf("Test 13\n");
//    Integer<32> shiftTest(0x0FFFFFF0);
//    shiftTest <<= 3;
//    assert(shiftTest == 0x7FFFFF80);
//    assert(shiftTest.HasOverflow() == false);
//    assert(shiftTest.IsNegative() == false);
//    shiftTest >>= 6;
//    assert(shiftTest == 0x01FFFFFE);
//    assert(shiftTest.HasOverflow() == false);
//    assert(shiftTest.IsNegative() == false);
//
//    printf("Test 14\n");
//    shiftTest = 0x00000101;
//    shiftTest <<= 19;
//    assert(shiftTest == 0x08080000);
//    assert(shiftTest.HasOverflow() == false);
//    assert(shiftTest.IsNegative() == false);
//    shiftTest >>= 19;
//    assert(shiftTest == 0x00000101);
//    assert(shiftTest.HasOverflow() == false);
//    assert(shiftTest.IsNegative() == false);
//
//    printf("Test 15\n");
//    rhs = 56088;
//    rhs /= 123;
//    assert(rhs == 456);
//
//    printf("Test 16\n");
//    rhs = 1;
//    rhs /= 3;
//    assert(rhs.IsZero() == true);
//
//    printf("Test 17\n");
//    rhs = 33;
//    Integer<0> result;
//    rhs %= 8;
//    assert(rhs == 1);
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == false);
//
//    printf("Test 18\n");
//    rhs = 0xAAAAAAAA;
//    rhs >>= 1;
//    assert(rhs == 0x55555555);
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == false);
//
//    printf("Test 19\n");
//    rhs |= Integer<64>(0xAAAAAAAA);
//    assert(rhs == Integer<64>(0xFFFFFFFF));
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == false);
//
//    printf("Test 20\n");
//    rhs &= Integer<64>(0x5555AAAA);
//    assert(rhs == Integer<64>(0x5555AAAA));
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == false);
//
//    printf("Test 21\n");
//    rhs ^= Integer<64>(0x55555555);
//    assert(rhs == Integer<64>(0x0000FFFF));
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == false);
//
//    printf("Test 22\n");
//    rhs = ~rhs;
//    assert(rhs.UsedLimbs() == rhs.Bits());
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == false);
//
//    printf("Test 23\n");
//    rhs = +1000000;
//    lhs = -2000000;
//    rhs = rhs - lhs;
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == false);
//    assert(rhs == 3000000);
//
//    printf("Test 24\n");
//    rhs = +1000000;
//    lhs = -2000000;
//    rhs = rhs + lhs;
//    assert(rhs == -1000000);
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == true);
//
//    printf("Test 25\n");
//    rhs = +1000000;
//    lhs = -2000000;
//    rhs = lhs + rhs;
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == true);
//    assert(rhs == -1000000);
//
//    printf("Test 26\n");
//    rhs = +1000000;
//    lhs = -2000000;
//    rhs = lhs - rhs;
//    ShowValue(rhs, true);
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == true);
//    assert(rhs == -3000000);
//
//    printf("Test 27\n");
//    rhs = +1000000;
//    lhs = -2000000;
//    rhs = lhs * rhs;
//    ShowValue(rhs, true);
//    assert(rhs.HasOverflow() == false);
//    assert(rhs.IsNegative() == true);
//    assert(rhs == -2000000000000);
//
//    printf("Test 29\n");
//    Integer<32> test;
//    test = 12345;
//    test.MillerRabin(100);
//}
