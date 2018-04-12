#pragma once

void test_capacity ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                              Capacity                                 //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(13);

    byte_string bs_empty;
    byte_string bs_empty1("");

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

    byte_string bs(bs_sample);
    size_type newcap = bs.capacity() + 10;
    bs.reserve(newcap);
    TEST_OK(bs.capacity() >= newcap);

    TEST_OK(bs.size() < bs.capacity());
    bs.shrink_to_fit();

    // It is a non-binding request to reduce capacity() to size()
    TEST_OK(bs.size() <= bs.capacity())
}
