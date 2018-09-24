#include <pfs/test.hpp>
#include <pfs/unicode/unicode_iterator.hpp>
#include <pfs/unicode/u8_iterator.hpp>
#include <string>
#include <sstream>
#include "test_data.hpp"

template <typename OctetIt>
void __test_decode_obsolete (const char * itertype)
{
    int ntests = sizeof (data) / sizeof (data[0]);
    ADD_TESTS(ntests);

    for (int i = 0; i < ntests; ++i) {
        OctetIt it(iter_cast<OctetIt>(data[i].text));
        OctetIt it_end(iter_cast<OctetIt>(data[i].text) + data[i].len);

        size_t count = 0;
        pfs::unicode::char_t uc = 0;

        while (it < it_end) {
            uc = pfs::unicode::utf8_iterator<OctetIt>::decode(it);
            ++count;
        }

        std::ostringstream desc;

        desc << "Decode UTF-8 using `" << itertype << "' as pointer. String `"
                << data[i].name
                << "'. Number of unicode chars "
                << count
                << ", expected "
                << data[i].nchars;
        TEST_OK2(count == data[i].nchars, desc.str().c_str());
    }
}

template <typename OctetIt>
void __test_decode (const char * itertype)
{
    int ntests = sizeof (data) / sizeof (data[0]);
    ADD_TESTS(ntests);

    for (int i = 0; i < ntests; ++i) {
        OctetIt first(iter_cast<OctetIt>(data[i].text));
        OctetIt last(iter_cast<OctetIt>(data[i].text) + data[i].len);

        size_t count = 0;
        pfs::unicode::char_t uc = 0;
        pfs::unicode::u8_input_iterator<OctetIt> inp_first(first, last);
        pfs::unicode::u8_input_iterator<OctetIt> inp_last(last);

        while (inp_first != inp_last) {
            uc = *inp_first++; //pfs::unicode::utf8_iterator<OctetIt>::decode(it);
            ++count;
        }

        std::ostringstream desc;

        desc << "Decode UTF-8 using `" << itertype << "' as pointer. String `"
                << data[i].name
                << "'. Number of unicode chars "
                << count
                << ", expected "
                << data[i].nchars;
        TEST_OK2(count == data[i].nchars, desc.str().c_str());
    }
}

void test_decode ()
{
    __test_decode_obsolete<unsigned char *>("unsigned char *");
    __test_decode_obsolete<const unsigned char *>("const unsigned char *");
    __test_decode_obsolete<char *>("char *");
    __test_decode_obsolete<const char *>("const char *");
    __test_decode_obsolete<std::string::iterator>("std::string::iterator");
    __test_decode_obsolete<std::string::const_iterator>("std::string::const_iterator");

    __test_decode<unsigned char *>("unsigned char *");
    __test_decode<const unsigned char *>("const unsigned char *");
    __test_decode<char *>("char *");
    __test_decode<const char *>("const char *");
    __test_decode<std::string::iterator>("std::string::iterator");
    __test_decode<std::string::const_iterator>("std::string::const_iterator");
}
