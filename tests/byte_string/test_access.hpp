#pragma once

void test_access ()
{
    ADD_TESTS(21);

    pfs::byte_string const bs_const(bs_sample);
    pfs::byte_string bs_mutable(bs_sample);

    TEST_OK(bs_sample.at(0) == 'b');
    TEST_OK(bs_sample.at(1) == 'y');
    TEST_OK(bs_sample.at(2) == 't');
    TEST_OK(bs_sample.at(3) == 'e');

    TEST_OK(bs_const.at(0) == 'b');
    TEST_OK(bs_const.at(1) == 'y');
    TEST_OK(bs_const.at(2) == 't');
    TEST_OK(bs_const.at(3) == 'e');

    TEST_OK(bs_sample[0] == 'b');
    TEST_OK(bs_sample[1] == 'y');
    TEST_OK(bs_sample[2] == 't');
    TEST_OK(bs_sample[3] == 'e');

    TEST_OK(bs_const[0] == 'b');
    TEST_OK(bs_const[1] == 'y');
    TEST_OK(bs_const[2] == 't');
    TEST_OK(bs_const[3] == 'e');

    TEST_OK(bs_sample.front() == 'b');
    TEST_OK(bs_const.front() == 'b');

    TEST_OK(bs_sample.back() == 'g');
    TEST_OK(bs_const.back() == 'g');

    pfs::byte_string::const_pointer raw_const = bs_const.data();
    pfs::byte_string::pointer raw_mutable = bs_mutable.data();

    raw_mutable[0] = 'e';
    raw_mutable[1] = 't';
    raw_mutable[2] = 'y';
    raw_mutable[3] = 'b';

    TEST_OK(std::strcmp(bs_mutable.c_str(), "etyb_string") == 0);
}

