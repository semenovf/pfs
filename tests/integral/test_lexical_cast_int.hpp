#pragma once

void test_lexical_cast_int ()
{
    ADD_TESTS(5);

    TEST_OK((pfs::lexical_cast<char>(pfs::string("0")) == 0));
    TEST_OK((pfs::lexical_cast<char>(pfs::string("127"))
            == pfs::numeric_limits<char>::max()));
    TEST_OK((pfs::lexical_cast<char>(pfs::string("-128"))
            == pfs::numeric_limits<char>::min()));

    TEST_OK((pfs::lexical_cast<unsigned char>(pfs::string("0")) == 0));
    TEST_OK((pfs::lexical_cast<unsigned char>(pfs::string("255"))
            == pfs::numeric_limits<unsigned char>::max()));
}
