#pragma once

void test_capacity ()
{
    ADD_TESTS(12);

    pfs::byte_string bs_empty;
    pfs::byte_string bs_empty1("");

    TEST_OK(!bs_sample.empty());
    TEST_OK(bs_empty.empty());
    TEST_OK(bs_empty1.empty());

    TEST_OK(bs_sample.size() == std::strlen(raw_chars));
    TEST_OK(bs_empty.size() == 0);
    TEST_OK(bs_empty1.size() == 0);

    TEST_OK(bs_sample.length() == std::strlen(raw_chars));
    TEST_OK(bs_empty.length() == 0);
    TEST_OK(bs_empty1.length() == 0);

    TEST_OK(bs_sample.max_size() == bs_empty.max_size()
            && bs_empty.max_size() == bs_empty1.max_size());

    pfs::byte_string bs(bs_sample);
    pfs::byte_string::size_type newcap = bs.capacity() + 10;
    bs.reserve(newcap);
    TEST_OK(bs.capacity() >= newcap);

    TEST_OK(bs.size() < bs.capacity());
    bs.shrink_to_fit();

#if __cplusplus >= 201103L
    ADD_TESTS(1);
    TEST_OK(bs.size() == bs.capacity())
#endif
}
