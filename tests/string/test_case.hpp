#pragma once

template <typename StringT>
void test_case ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                        Upper/Lower case functions                     //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(4);

    StringT str_lowercase("abcdefghijklmnopqrstuvwxyz");
    StringT str_uppercase("ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    StringT result;

    TEST_OK2(pfs::to_lower(str_lowercase) == str_lowercase, "Convert lower case to lower case");
    TEST_OK2(pfs::to_lower(str_uppercase) == str_lowercase, "Convert upper case to lower case");
    TEST_OK2(pfs::to_upper(str_lowercase) == str_uppercase, "Convert lower case to upper case");
    TEST_OK2(pfs::to_upper(str_uppercase) == str_uppercase, "Convert upper case to upper case");
}


