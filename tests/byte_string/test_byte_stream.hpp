#ifndef __TEST_APPEND_HPP__
#define __TEST_APPEND_HPP__

#include "pfs/compiler.hpp"
#include "pfs/limits.hpp"

#include <iostream>

void test_byte_ostream_integral ()
{
    const char     v01 = '\x20';
    const int8_t   v02 = 0x12;
    const uint8_t  v03 = 0xCD;
    const wchar_t  v04 = L' ';
    const int16_t  v05 = 0x1234;
    const uint16_t v06 = 0xCDEF;
    const int32_t  v07 = 0x12345678;
    const uint32_t v08 = 0xCDEF0123;

#if PFS_HAVE_INT64
    const int64_t  v09 = 0x123456789ABCDEF0;
    const uint64_t v10 = 0xCDEF0123456789AB;
#endif

    // Little endian
    {
        ADD_TESTS(1);

        pfs::byte_string sample("\x20\x12\xCD"
#if PFS_CC_MSC
        "\x20\x00"
#else
        "\x20\x00\x00\x00"
#endif
        "\x34\x12"
        "\xEF\xCD"
        "\x78\x56\x34\x12"
        "\x23\x01\xEF\xCD"
#if PFS_HAVE_INT64
        "\xF0\xDE\xBC\x9A\x78\x56\x34\x12"
        "\xAB\x89\x67\x45\x23\x01\xEF\xCD"
#endif
#if PFS_CC_MSC
#   if PFS_HAVE_INT64
        , 33);
#   else
        , 17);
#   endif
#else
#   if PFS_HAVE_INT64
        , 35);
#   else
        , 19);
#   endif
#endif
        pfs::byte_string buffer;
        pfs::byte_ostream os(buffer, pfs::endian::little_endian);
        os << v01 << v02 << v03 << v04 << v05 << v06 << v07 << v08
#if PFS_HAVE_INT64
                << v09 << v10
#endif
        ;

        TEST_OK2(os.data() == sample
                , "test_byte_ostream_integral (little endian order): "
                  "byte_ostream's internal buffer equals to sample buffer");
    }

    // Big endian
    {
        ADD_TESTS(1);

        pfs::byte_string sample("\x20\x12\xCD"
#if PFS_CC_MSC
        "\x00\x20"
#else
        "\x00\x00\x00\x20"
#endif
        "\x12\x34"
        "\xCD\xEF"
        "\x12\x34\x56\x78"
        "\xCD\xEF\x01\x23"
#if PFS_HAVE_INT64
        "\x12\x34\x56\x78\x9A\xBC\xDE\xF0"
        "\xCD\xEF\x01\x23\x45\x67\x89\xAB"
#endif
#if PFS_CC_MSC
#   if PFS_HAVE_INT64
        , 33);
#   else
        , 17);
#   endif
#else
#   if PFS_HAVE_INT64
        , 35);
#   else
        , 19);
#   endif
#endif
        pfs::byte_string buffer;
        pfs::byte_ostream os(buffer, pfs::endian::big_endian);
        os << v01 << v02 << v03 << v04 << v05 << v06 << v07 << v08
#if PFS_HAVE_INT64
                << v09 << v10
#endif
        ;

        TEST_OK2(os.data() == sample
                , "test_byte_ostream_integral (big endian order): "
                  "byte_ostream's internal buffer equals to sample buffer");
        //std::cout << "os.data().size(): " << os.data().size() << "=" << sample.size() << " :sample.size()";
        //std::cout << std::hex << int(os.data()[3]) << "=" << std::hex << int(sample[3]) << std::endl;
        //std::cout << std::hex << int(os.data()[4]) << "=" << std::hex << int(sample[4]) << std::endl;
        //std::cout << std::hex << int(os.data()[5]) << "=" << std::hex << int(sample[5]) << std::endl;
        //std::cout << std::hex << int(os.data()[6]) << "=" << std::hex << int(sample[6]) << std::endl;
    }

    {
        ADD_TESTS(8);

#if PFS_HAVE_INT64
        ADD_TESTS(2);
#endif
        pfs::byte_string buffer;
        pfs::byte_ostream os(buffer, pfs::endian::little_endian);
        os << v01 << v02 << v03 << v04 << v05 << v06 << v07 << v08
#if PFS_HAVE_INT64
                << v09 << v10
#endif
        ;

        pfs::byte_istream is(os.data().cbegin(), os.data().cend(), pfs::endian::little_endian);

        char     a01;
        int8_t   a02;
        uint8_t  a03;
        wchar_t  a04;
        int16_t  a05;
        uint16_t a06;
        int32_t  a07;
        uint32_t a08;

#if PFS_HAVE_INT64
        int64_t  a09;
        uint64_t a10;
#endif

        try {
            is >> a01 >> a02 >> a03 >> a04 >> a05 >> a06 >> a07 >> a08
#if PFS_HAVE_INT64
                    >> a09 >> a10
#endif
        ;
        } catch (pfs::out_of_range ex) {
            TEST_FAIL2(false, "failed to read data from stream (source too short)");
        }

        TEST_OK(a01 == v01);
        TEST_OK(a02 == v02);
        TEST_OK(a03 == v03);
        TEST_OK(a04 == v04);
        TEST_OK(a05 == v05);
        TEST_OK(a06 == v06);
        TEST_OK(a07 == v07);
        TEST_OK(a08 == v08);
#if PFS_HAVE_INT64
        TEST_OK(a09 == v09);
        TEST_OK(a10 == v10);
#endif
    }

}

void test_byte_ostream_real ()
{
    const real32_t v1 = 0x12345680;
    const real64_t v2 = 0x123456789ABCDF00;

    {
        ADD_TESTS(1);

        pfs::byte_string sample("\xB4\xA2\x91\x4D"
                "\xDF\xBC\x9A\x78\x56\x34\xB2\x43", 12);

        pfs::byte_string buffer;
        pfs::byte_ostream os(buffer, pfs::endian::little_endian);
        os << v1 << v2;

        TEST_OK2(os.data() == sample
                , "test_byte_ostream_real (little endian order): "
                  "byte_ostream's internal buffer equals to sample buffer");
    }

    {
        ADD_TESTS(1);

        pfs::byte_string sample("\x4D\x91\xA2\xB4"
                "\x43\xB2\x34\x56\x78\x9A\xBC\xDF", 12);

        pfs::byte_string buffer;
        pfs::byte_ostream os(buffer, pfs::endian::big_endian);
        os << v1 << v2;

        TEST_OK2(os.data() == sample
                , "test_byte_ostream_real (big endian order): "
                  "byte_ostream's internal buffer equals to sample buffer");
    }

    {
        ADD_TESTS(2);

        pfs::byte_string buffer;
        pfs::byte_ostream os(buffer, pfs::endian::little_endian);
        os << v1 << v2;

        pfs::byte_istream is(os.data().cbegin(), os.data().cend(), pfs::endian::little_endian);

        real32_t a1;
        real64_t a2;

        try {
            is >> a1 >> a2;
        } catch (pfs::out_of_range ex) {
            TEST_FAIL2(false, "failed to read data from stream (source too short)");
        }

        TEST_OK(a1 == v1);
        TEST_OK(a2 == v2);
    }
}

void test_byte_stream_byte_string ()
{
    pfs::byte_string sample("\x4D\x91\xA2\xB4"
            "\x43\xB2\x34\x56\x78\x9A\xBC\xDF", 12);

    {
        ADD_TESTS(1);

        pfs::byte_string buffer;
        pfs::byte_ostream os(buffer, pfs::endian::little_endian);
        os << pfs::byte_string_ref_n<1>(& sample);

        TEST_OK2(os.data() == "\x0C" + sample
                , "test_byte_stream_byte_string: "
                  "byte_ostream's internal buffer equals to sample buffer");
    }

    {
        ADD_TESTS(1);

        pfs::byte_string buffer;
        pfs::byte_ostream os(buffer, pfs::endian::little_endian);
        os << pfs::byte_string_ref_n<2>(& sample);

        TEST_OK2(os.data() == pfs::byte_string("\x0C\x00", 2) + sample
                , "test_byte_stream_byte_string: "
                  "byte_ostream's internal buffer equals to sample buffer");
    }

    {
        ADD_TESTS(1);

        pfs::byte_string buffer;
        pfs::byte_ostream os(buffer, pfs::endian::big_endian);
        os << pfs::byte_string_ref_n<2>(& sample);

        TEST_OK2(os.data() == pfs::byte_string("\x00\x0C", 2) + sample
                , "test_byte_stream_byte_string: "
                  "byte_ostream's internal buffer equals to sample buffer");
    }

    {
        ADD_TESTS(1);

        pfs::byte_string buffer;
        pfs::byte_ostream os(buffer, pfs::endian::little_endian);
        os << pfs::byte_string_ref_n<4>(& sample);

        TEST_OK2(os.data() == pfs::byte_string("\x0C\x00\x00\x00", 4) + sample
                , "test_byte_stream_byte_string: "
                  "byte_ostream's internal buffer equals to sample buffer");
    }

    {
        ADD_TESTS(1);

        pfs::byte_string buffer;
        pfs::byte_ostream os(buffer, pfs::endian::big_endian);
        os << pfs::byte_string_ref_n<4>(& sample);

        TEST_OK2(os.data() == pfs::byte_string("\x00\x00\x00\x0C", 4) + sample
                , "test_byte_stream_byte_string: "
                  "byte_ostream's internal buffer equals to sample buffer");
    }

    {
        ADD_TESTS(1);

        pfs::byte_string buffer;
        pfs::byte_ostream os(buffer, pfs::endian::little_endian);
        os << pfs::byte_string_ref_n<4>(& sample);

        pfs::byte_istream is(os.data().cbegin(), os.data().cend(), pfs::endian::little_endian);
        pfs::byte_string bs;
        is >> pfs::byte_string_ref_n<4>(& bs);

        TEST_OK(bs == sample);
    }

    {
        ADD_TESTS(1);

        pfs::byte_string buffer;
        pfs::byte_ostream os(buffer, pfs::endian::little_endian);
        os << pfs::byte_string_ref(& sample, sample.size());

        pfs::byte_istream is(os.data().cbegin(), os.data().cend(), pfs::endian::little_endian);
        pfs::byte_string bs;
        is >> pfs::byte_string_ref(& bs, sample.size());

        TEST_OK(bs == sample);
    }

    {
        ADD_TESTS(1);

        pfs::byte_string buffer;
        pfs::byte_ostream os(buffer, pfs::endian::little_endian);
        os << pfs::byte_string_ref(& sample);

        pfs::byte_istream is(os.data().cbegin(), os.data().cend(), pfs::endian::little_endian);
        char buf[12];
        is >> pfs::buffer_wrapper<char>(buf, 12);

        TEST_OK(sample == pfs::byte_string(buf, sizeof(buf)));
    }

}

void test_byte_stream ()
{
    test_byte_ostream_integral();
    test_byte_ostream_real();
    test_byte_stream_byte_string();
}

#endif /* __TEST_APPEND_HPP__ */

