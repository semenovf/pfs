#pragma once
#include <pfs/byte_string.hpp>
#include <pfs/binary_istream.hpp>
#include <pfs/io/buffer.hpp>

typedef pfs::byte_string byte_string;

struct byte_string_idevice
{
    byte_string_idevice (byte_string & buffer)
        : _buffer(buffer)
        , _cursor(0)
    {}

    ssize_t read (char * s, size_t n)
    {
        _buffer.copy(reinterpret_cast<byte_string::pointer>(s), n, _cursor);
        _cursor += n;
        return static_cast<ssize_t>(n);
    }

private:
    byte_string & _buffer;
    byte_string::size_type _cursor;
};

template <typename InputDevice>
void test_binary_istream (InputDevice & dev)
{
    typedef pfs::binary_istream<InputDevice> binary_istream;

    ADD_TESTS(11);

    pfs::endian order = pfs::endian::network_order();
    binary_istream bis(dev, order);

    bool t;
    bool f;
    char c;
    int8_t i8;
    uint8_t u8;
    int16_t i16;
    uint16_t u16;
    int32_t i32;
    uint32_t u32;
#if PFS_HAVE_INT64
    int64_t i64;
    uint64_t u64;
#endif

    real32_t f32;

#if PFS_HAVE_INT64
    real64_t f64;
#endif

    bis >> t >> f
            >> c >> i8 >> u8
            >> i16 >> u16
            >> i32 >> u32
#if PFS_HAVE_INT64
            >> i64 >> u64
#endif
            >> f32
#if PFS_HAVE_INT64
            >> f64
#endif
            ;

    TEST_OK(t == true);
    TEST_OK(f == false);
    TEST_OK(c == 'A');
    TEST_OK(i8 == static_cast<int8_t>(0xDE));
    TEST_OK(u8 == 0xAD);
    TEST_OK(i16 == static_cast<int16_t>(0xDEAD));
    TEST_OK(u16 == 0xBEEF);
    TEST_OK(i32 == static_cast<int32_t>(0xDEADBEEF));
    TEST_OK(u32 == 0xABADBABE);

#if PFS_HAVE_INT64
    ADD_TESTS(2)
    TEST_OK(i64 == static_cast<int64_t>(0xDEADBEEFABADBABE));
    TEST_OK(u64 == 0xABADBABEDEADBEEF);
#endif

    TEST_OK(f32 == static_cast<real32_t>(0x4E9D3A75));

#if PFS_HAVE_INT64
    TEST_OK(f64 == static_cast<real64_t>(0x43D0F43D0F43D0F4));
#endif
}

void test_binary_istream ()
{
    uint8_t bytes[] = {
          0x01, 0x00
        , 0x41, 0xDE, 0xAD
        , 0xDE, 0xAD, 0xBE, 0xEF
        , 0xDE, 0xAD, 0xBE, 0xEF, 0xAB, 0xAD, 0xBA, 0xBE
#if PFS_HAVE_INT64
        , 0xDE, 0xAD, 0xBE, 0xEF, 0xAB, 0xAD, 0xBA, 0xBE
        , 0xAB, 0xAD, 0xBA, 0xBE, 0xDE, 0xAD, 0xBE, 0xEF
#endif
        , 0x4E, 0x9D, 0x3A, 0x75

#if PFS_HAVE_INT64
        , 0x43, 0xD0, 0xF4, 0x3D, 0x0F, 0x43, 0xD0, 0xF4
#endif
    };

    byte_string buffer(bytes, sizeof(bytes)/sizeof(bytes[0]));

    byte_string_idevice dev1(buffer);
    pfs::io::device dev2 = pfs::io::open_device(pfs::io::open_params<pfs::io::buffer>(buffer));

    test_binary_istream<byte_string_idevice>(dev1);
    test_binary_istream<pfs::io::device>(dev2);
}
