#pragma once

void test_lexical_cast_float ()
{
    ADD_TESTS(8);

    TEST_OK((pfs::lexical_cast<float>(pfs::string("0"))  == 0.0f));
    TEST_OK((pfs::lexical_cast<float>(pfs::string("0.")) == 0.0f));
    TEST_OK((pfs::lexical_cast<float>(pfs::string("0.0")) == 0.0f));
    TEST_OK((pfs::lexical_cast<float>(pfs::string(".0")) == 0.0f));

    TEST_OK((pfs::lexical_cast<float>(pfs::string("1"))  == 1.0f));
    TEST_OK((pfs::lexical_cast<float>(pfs::string("1.")) == 1.0f));
    TEST_OK((pfs::lexical_cast<float>(pfs::string("1.0")) == 1.0f));
    TEST_OK((pfs::lexical_cast<float>(pfs::string(".1")) == 0.1f));
}
