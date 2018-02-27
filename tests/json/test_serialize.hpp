#include <cstdio>
#include <sstream>
#include <pfs/test/test.hpp>
#include "pfs/ctype.hpp"
#include "pfs/json/json.hpp"
#include "pfs/json/ubjson_ostream.hpp"
#include "pfs/json/ubjson_istream.hpp"

namespace test_serialize {

pfs::byte_string hex (pfs::byte_string const & bs)
{
    pfs::byte_string r;
    pfs::byte_string::const_iterator first = bs.cbegin();
    pfs::byte_string::const_iterator last = bs.cend();

    if (first != last)
        r.push_back(*first++);
    
    while (first != last) {
        char buf[3];
        std::sprintf(buf, "%02x", static_cast<unsigned char>(*first));
        r.push_back('\\');
        r.push_back('x');
        r.push_back(buf[0]);
        r.push_back(buf[1]);
        ++first;
    }

    return r;
}

struct test_data_item
{
    pfs::byte_string s1;
    pfs::byte_string s2;
};

template <typename JsonType>
void test_number ()
{
    typedef pfs::json::ubjson_ostream<pfs::byte_ostream, JsonType> ubjson_ostream_t;

    test_data_item tdi[] = {
        { pfs::byte_string("3.1415925"), pfs::byte_string("D\x40\x09\x21\xfb\x3f\xa6\xde\xfc", 9) },
          { pfs::byte_string("null")  , pfs::byte_string("Z") }
        , { pfs::byte_string("true")  , pfs::byte_string("T") }
        , { pfs::byte_string("false") , pfs::byte_string("F") }
        , { pfs::byte_string("0")     , pfs::byte_string("i\x00", 2) }
        , { pfs::byte_string("-128")  , pfs::byte_string("i\x80", 2) }
        , { pfs::byte_string("127")   , pfs::byte_string("i\x7f", 2) }
        , { pfs::byte_string("255")   , pfs::byte_string("U\xff", 2) }
        , { pfs::byte_string("-32768"), pfs::byte_string("I\x80\x00", 3) }
        , { pfs::byte_string("32767") , pfs::byte_string("I\x7f\xff", 3) }
        , { pfs::byte_string("65535") , pfs::byte_string("l\x00\x00\xff\xff", 5) }
        , { pfs::byte_string("-2147483648") , pfs::byte_string("l\x80\x00\x00\x00", 5) }
        , { pfs::byte_string("2147483647")  , pfs::byte_string("l\x7f\xff\xff\xff", 5) }
#if PFS_HAVE_INT64
        , { pfs::byte_string("4294967295")  , pfs::byte_string("L\x00\x00\x00\x00\xff\xff\xff\xff", 9) }
        , { pfs::byte_string("-9223372036854775808"), pfs::byte_string("L\x80\x00\x00\x00\x00\x00\x00\x00", 9) }
        , { pfs::byte_string("9223372036854775807"), pfs::byte_string("L\x7f\xff\xff\xff\xff\xff\xff\xff", 9) }
#endif
        , { pfs::byte_string("1")    , pfs::byte_string("i\x01", 2) }
        , { pfs::byte_string("10")   , pfs::byte_string("i\x0a", 2) }
        , { pfs::byte_string("100")  , pfs::byte_string("i\x64", 2) }
        , { pfs::byte_string("1000") , pfs::byte_string("I\x03\xe8", 3) }
        , { pfs::byte_string("10000"), pfs::byte_string("I\x27\x10", 3) }
        , { pfs::byte_string("100000"), pfs::byte_string("l\x00\x01\x86\xa0", 5) }
        , { pfs::byte_string("1000000"), pfs::byte_string("l\x00\x0f\x42\x40", 5) }
        , { pfs::byte_string("10000000"), pfs::byte_string("l\x00\x98\x96\x80", 5) }
        , { pfs::byte_string("100000000"), pfs::byte_string("l\x05\xf5\xe1\x00", 5) }
        , { pfs::byte_string("1000000000"), pfs::byte_string("l\x3b\x9a\xca\x00", 5) }
#if PFS_HAVE_INT64
        , { pfs::byte_string("10000000000"), pfs::byte_string("L\x00\x00\x00\x02\x54\x0b\xe4\x00", 9) }
        , { pfs::byte_string("100000000000"), pfs::byte_string("L\x00\x00\x00\x17\x48\x76\xe8\x00", 9) }
        , { pfs::byte_string("1000000000000"), pfs::byte_string("L\x00\x00\x00\xe8\xd4\xa5\x10\x00", 9) }
        , { pfs::byte_string("10000000000000"), pfs::byte_string("L\x00\x00\x09\x18\x4e\x72\xa0\x00", 9) }
        , { pfs::byte_string("100000000000000"), pfs::byte_string("L\x00\x00\x5a\xf3\x10\x7a\x40\x00", 9) }
        , { pfs::byte_string("1000000000000000"), pfs::byte_string("L\x00\x03\x8d\x7e\xa4\xc6\x80\x00", 9) }
        , { pfs::byte_string("10000000000000000"), pfs::byte_string("L\x00\x23\x86\xf2\x6f\xc1\x00\x00", 9) }
        , { pfs::byte_string("100000000000000000"), pfs::byte_string("L\x01\x63\x45\x78\x5d\x8a\x00\x00", 9) }
        , { pfs::byte_string("1000000000000000000"), pfs::byte_string("L\x0d\xe0\xb6\xb3\xa7\x64\x00\x00", 9) }
#endif

        , { pfs::byte_string("-1")    , pfs::byte_string("i\xff", 2) }
        , { pfs::byte_string("-10")   , pfs::byte_string("i\xf6", 2) }
        , { pfs::byte_string("-100")  , pfs::byte_string("i\x9c", 2) }
        , { pfs::byte_string("-1000") , pfs::byte_string("I\xfc\x18", 3) }
        , { pfs::byte_string("-10000"), pfs::byte_string("I\xd8\xf0", 3) }
        , { pfs::byte_string("-100000"), pfs::byte_string("l\xff\xfe\x79\x60", 5) }
        , { pfs::byte_string("-1000000"), pfs::byte_string("l\xff\xf0\xbd\xc0", 5) }
        , { pfs::byte_string("-10000000"), pfs::byte_string("l\xff\x67\x69\x80", 5) }
        , { pfs::byte_string("-100000000"), pfs::byte_string("l\xfa\x0a\x1f\x00", 5) }
        , { pfs::byte_string("-1000000000"), pfs::byte_string("l\xc4\x65\x36\x00", 5) }
#if PFS_HAVE_INT64
        , { pfs::byte_string("-10000000000"), pfs::byte_string("L\xff\xff\xff\xfd\xab\xf4\x1c\x00", 9) }
        , { pfs::byte_string("-100000000000"), pfs::byte_string("L\xff\xff\xff\xe8\xb7\x89\x18\x00", 9) }
        , { pfs::byte_string("-1000000000000"), pfs::byte_string("L\xff\xff\xff\x17\x2b\x5a\xf0\x00", 9) }
        , { pfs::byte_string("-10000000000000"), pfs::byte_string("L\xff\xff\xf6\xe7\xb1\x8d\x60\x00", 9) }
        , { pfs::byte_string("-100000000000000"), pfs::byte_string("L\xff\xff\xa5\x0c\xef\x85\xc0\x00", 9) }
        , { pfs::byte_string("-1000000000000000"), pfs::byte_string("L\xff\xfc\x72\x81\x5b\x39\x80\x00", 9) }
        , { pfs::byte_string("-10000000000000000"), pfs::byte_string("L\xff\xdc\x79\x0d\x90\x3f\x00\x00", 9) }
        , { pfs::byte_string("-100000000000000000"), pfs::byte_string("L\xfe\x9c\xba\x87\xa2\x76\x00\x00", 9) }
        , { pfs::byte_string("-1000000000000000000"), pfs::byte_string("L\xf2\x1f\x49\x4c\x58\x9c\x00\x00", 9) }
#endif
        , { pfs::byte_string("3.1415925"), pfs::byte_string("D\x40\x09\x21\xfb\x3f\xa6\xde\xfc", 9) }
    };
    
    const int count = sizeof(tdi)/sizeof(tdi[0]);

    ADD_TESTS(2 * count);

    for (int i = 0; i < count; i++) {
        JsonType j;
        
        pfs::byte_string const * sample = & tdi[i].s1;
        pfs::byte_string const * expected = & tdi[i].s2;
        
        if (j.parse(sample->c_str()) != pfs::error_code()) {
            std::stringstream description;
            description << "json.parse(\"" << hex(*sample).c_str() << "\")";
            TEST_FAIL2(false, description.str().c_str());
        }

        std::stringstream description1;
        description1 << "to ubjson: \"" << sample->c_str() 
                << "\" => \"" << hex(*expected).c_str() << "\"";
        
        TEST_OK2(pfs::json::to_ubjson(j) == *expected, description1.str().c_str());

        sample = & tdi[i].s2;
        expected = & tdi[i].s1;
        
        std::stringstream description2;
        description2 << "from ubjson: \"" << hex(*sample).c_str() << "\" => " 
                << "\"" << expected->c_str() << "\"";
        
        TEST_OK2(pfs::json::from_ubjson<JsonType>(*sample) == j
                , description2.str().c_str());
    }
}

template <typename JsonType>
void test_string ()
{
    {
        typename JsonType::string_type s1(1, '@');
//
//        std::stringstream description1;
//        description1 << "to ubjson: \"" << sample->c_str() 
//                    << "\" => \"" << hex(*expected).c_str() << "\"";
    }
}

template <typename JsonType>
void test ()
{
    test_number<JsonType>();
    test_string<JsonType>();
}

}
