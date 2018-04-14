#pragma once
#include <pfs/v2/byte_string.hpp>
#include <pfs/v2/binary_ostream.hpp>

typedef pfs::byte_string byte_string;

struct byte_string_odevice
{
    byte_string_odevice (byte_string & buffer)
        : _buffer(buffer)
    {}

    ssize_t write (char const * s, size_t n)
    {
        _buffer.append(s, n);
    }

private:
    byte_string & _buffer;
};

typedef pfs::binary_ostream<byte_string_odevice> binary_ostream;

void test_binary_ostream ()
{
    ADD_TESTS(21);

    pfs::endian order = pfs::endian::network_order();
    byte_string buffer;
    byte_string_odevice dev(buffer);
    binary_ostream bos(dev, order);

    int i = 0;

    bos << bool(true)
        << bool(false);

    TEST_OK(buffer[i++] == 1);
    TEST_OK(buffer[i++] == 0);

    bos << 'A'
        << static_cast<int8_t>(0xDE)
        << static_cast<uint8_t>(0xAD);

    TEST_OK(buffer[i++] == 'A');
    TEST_OK(buffer[i++] == 0xDE);
    TEST_OK(buffer[i++] == 0xAD);

    bos << static_cast<int16_t>(0xDEAD)
        << static_cast<uint16_t>(0xBEEF);

    TEST_OK(buffer[i++] == 0xDE);
    TEST_OK(buffer[i++] == 0xAD);
    TEST_OK(buffer[i++] == 0xBE);
    TEST_OK(buffer[i++] == 0xEF);

    bos << static_cast<int32_t>(0xDEADBEEF)
        << static_cast<uint32_t>(0xABADBABE);

    TEST_OK(buffer[i++]  == 0xDE);
    TEST_OK(buffer[i++] == 0xAD);
    TEST_OK(buffer[i++] == 0xBE);
    TEST_OK(buffer[i++] == 0xEF);

    TEST_OK(buffer[i++] == 0xAB);
    TEST_OK(buffer[i++] == 0xAD);
    TEST_OK(buffer[i++] == 0xBA);
    TEST_OK(buffer[i++] == 0xBE);

#if PFS_HAVE_INT64
    ADD_TESTS(16)

    bos << static_cast<int64_t>(0xDEADBEEFABADBABE)
        << static_cast<uint64_t>(0xABADBABEDEADBEEF);
    TEST_OK(buffer[i++] == 0xDE);
    TEST_OK(buffer[i++] == 0xAD);
    TEST_OK(buffer[i++] == 0xBE);
    TEST_OK(buffer[i++] == 0xEF);
    TEST_OK(buffer[i++] == 0xAB);
    TEST_OK(buffer[i++] == 0xAD);
    TEST_OK(buffer[i++] == 0xBA);
    TEST_OK(buffer[i++] == 0xBE);

    TEST_OK(buffer[i++] == 0xAB);
    TEST_OK(buffer[i++] == 0xAD);
    TEST_OK(buffer[i++] == 0xBA);
    TEST_OK(buffer[i++] == 0xBE);
    TEST_OK(buffer[i++] == 0xDE);
    TEST_OK(buffer[i++] == 0xAD);
    TEST_OK(buffer[i++] == 0xBE);
    TEST_OK(buffer[i++] == 0xEF);
#endif

    bos << static_cast<real32_t>(0x4E9D3A75);

    TEST_OK(buffer[i++] == 0x4E);
    TEST_OK(buffer[i++] == 0x9D);
    TEST_OK(buffer[i++] == 0x3A);
    TEST_OK(buffer[i++] == 0x75);

#if PFS_HAVE_INT64
    ADD_TESTS(8);

    bos << static_cast<real64_t>(0x43D0F43D0F43D0F4);

    TEST_OK(buffer[i++] == 0x43);
    TEST_OK(buffer[i++] == 0xD0);
    TEST_OK(buffer[i++] == 0xF4);
    TEST_OK(buffer[i++] == 0x3D);
    TEST_OK(buffer[i++] == 0x0F);
    TEST_OK(buffer[i++] == 0x43);
    TEST_OK(buffer[i++] == 0xD0);
    TEST_OK(buffer[i++] == 0xF4);
#endif
}
