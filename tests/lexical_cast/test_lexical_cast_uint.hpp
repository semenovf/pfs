#pragma once

void test_lexical_cast_uint ()
{
    ADD_TESTS(2);

    TEST_OK((pfs::lexical_cast<unsigned char>(pfs::string("0")) == 0));
    TEST_OK((pfs::lexical_cast<unsigned char>(pfs::string("255"))
            == pfs::numeric_limits<unsigned char>::max()));
}
