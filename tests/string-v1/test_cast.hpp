#ifndef TEST_CAST_HPP
#define TEST_CAST_HPP

#include <cstring>

template <typename StringT>
void test_c_str_cast ()
{
    typedef pfs::string<StringT> string;
    typedef typename string::value_type  value_type;
    
    test_description<value_type>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(7);

    string orig(string_samples<value_type>(STR_ORIG));
    string same(string_samples<value_type>(STR_SAME));
    string diff(string_samples<value_type>(STR_DIFF));
    string empty(string_samples<value_type>(STR_EMPTY));
    
    TEST_OK(std::strcmp(string_samples<char>(STR_ORIG)
            , pfs::traits::c_str<StringT>(orig)()) == 0);

    TEST_OK(std::strcmp(string_samples<char>(STR_SAME)
            , pfs::traits::c_str<StringT>(orig)()) == 0);

    TEST_OK(std::strcmp(string_samples<char>(STR_DIFF)
            , pfs::traits::c_str<StringT>(orig)()) != 0);

//    TEST_OK(std::strcmp(string_samples<wchar_t>(STR_ORIG)
//            , pfs::traits::c_wstr<StringT>(orig)()) == 0);
//
//    TEST_OK(std::strcmp(string_samples<wchar_t>(STR_SAME)
//            , pfs::traits::c_wstr<StringT>(orig)()) == 0);
//
//    TEST_OK(std::strcmp(string_samples<wchar_t>(STR_DIFF)
//            , pfs::traits::c_wstr<StringT>(orig)()) != 0);
    
    TEST_OK(std::strcmp(pfs::traits::c_str<StringT>(orig)()
            , pfs::traits::c_str<StringT>(orig)()) == 0);

    TEST_OK(std::strcmp(pfs::traits::c_str<StringT>(orig)()
            , pfs::traits::c_str<StringT>(same)()) == 0);

    TEST_OK(std::strcmp(pfs::traits::c_str<StringT>(orig)()
            , pfs::traits::c_str<StringT>(same)()) == 0);

    TEST_OK(std::strcmp(pfs::traits::c_str<StringT>(empty)()
            , "") == 0);
}

#endif /* TEST_CAST_HPP */
