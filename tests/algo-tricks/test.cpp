#include <pfs/test.hpp>
#include <pfs/types.hpp>

//
// Source:
// Book "Hacker's Delight" by Henry S. Warren, Jr
// "Алгоритмические трюки для программистов", Генри Уоррен-мл., 2014, глава 5, раздел  5.1
//
size_t calculate_bits_1 (byte_t b)
{
    if (b == 0) return 0;

    byte_t x = b;

    x = (x & 0x5555) + ((x >> 1) & 0x5555);
    x = (x & 0x3333) + ((x >> 2) & 0x3333);
    x = (x & 0x0F0F) + ((x >> 4) & 0x0F0F);
    x = (x & 0x00FF) + ((x >> 8) & 0x00FF);

    return size_t(x);
}


int main ()
{
    BEGIN_TESTS(9);

    TEST_OK(calculate_bits_1(byte_t(0)) == 0);
    TEST_OK(calculate_bits_1(byte_t(1)) == 1);
    TEST_OK(calculate_bits_1(byte_t(2)) == 1);
    TEST_OK(calculate_bits_1(byte_t(3)) == 2);
    TEST_OK(calculate_bits_1(byte_t(4)) == 1);
    TEST_OK(calculate_bits_1(byte_t(5)) == 2);
    TEST_OK(calculate_bits_1(byte_t(6)) == 2);
    TEST_OK(calculate_bits_1(byte_t(7)) == 3);
    TEST_OK(calculate_bits_1(byte_t(8)) == 1);

    return END_TESTS;
}


