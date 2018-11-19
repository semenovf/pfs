#pragma once
#include <cstring>

void test_assignments ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             Assignments                               //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(28);

    //
    // Default copy assignment operator
    //
    byte_string bs_default;
    bs_default = bs_sample;
    TEST_OK(bs_default.size() == std::strlen(raw_chars));
    TEST_OK(std::strcmp(bs_default.c_str(), raw_chars) == 0);

#if __cplusplus >= 201103L
    ADD_TESTS(3);
    //
    // Move Assignment
    //
    byte_string bs_move1(raw_bytes);
    byte_string bs_move2;
    bs_move2 = std::move(bs_move1);

    TEST_OK(bs_move1.size() == 0);
    TEST_OK(std::strcmp(bs_move1.c_str(), "") == 0);
    TEST_OK(std::strcmp(bs_move2.c_str(), raw_chars) == 0);
#endif

    //
    // Assignment from raw const pointer
    //
    byte_string bs_from_raw;
    bs_from_raw = raw_bytes;
    TEST_OK(std::strcmp(bs_from_raw.c_str(), raw_chars) == 0);

    byte_string bs_from_raw1;
    bs_from_raw1 = raw_chars;
    TEST_OK(std::strcmp(bs_from_raw1.c_str(), raw_chars) == 0);

    //
    // Assignment from single value
    //
    byte_string bs_from_byte;
    bs_from_byte = raw_bytes[0];
    TEST_OK(bs_from_byte.size() == 1);
    TEST_OK(std::strcmp(bs_from_byte.c_str(), "b") == 0);

    //
    // Assignment from single char
    //
    byte_string bs_from_char;
    bs_from_char = 'b';
    TEST_OK(bs_from_char.size() == 1);
    TEST_OK(std::strcmp(bs_from_char.c_str(), "b") == 0);

    //
    // Assign from count copy of value
    //
    byte_string bs_rpt;
    bs_rpt.assign(4, raw_bytes[0]);
    TEST_OK(bs_rpt.size() == 4);
    TEST_OK(std::strcmp(bs_rpt.c_str(), "bbbb") == 0);

    //
    // Assign from count copy of value
    //
    byte_string bs_rpt1;
    bs_rpt1.assign(4, 'b');
    TEST_OK(bs_rpt1.size() == 4);
    TEST_OK(std::strcmp(bs_rpt1.c_str(), "bbbb") == 0);

    //
    // Assigns from substring
    //
    byte_string bs_rpt2;
    bs_rpt2.assign(bs_sample, 0, bs_sample.size());
    TEST_OK(bs_rpt2.size() == bs_sample.size());
    TEST_OK(std::strcmp(bs_rpt2.c_str(), raw_chars) == 0);

    byte_string bs_rpt3;
    bs_rpt3.assign(bs_sample, 5, byte_string::npos);
    TEST_OK(bs_rpt3.size() == 6);
    TEST_OK(std::strcmp(bs_rpt3.c_str(), "string") == 0);

    byte_string bs_rpt4;
    bs_rpt4.assign(bs_sample, 2, 5);
    TEST_OK(bs_rpt4.size() == 5);
    TEST_OK(std::strcmp(bs_rpt4.c_str(), "te_st") == 0);

    //
    // Assigns from first count values
    //
    byte_string bs_count;
    bs_count.assign(raw_bytes, 4);
    TEST_OK(bs_count.size() == 4);
    TEST_OK(std::strcmp(bs_count.c_str(), "byte") == 0);

    byte_string bs_count1;
    bs_count1.assign(raw_chars, 4);
    TEST_OK(bs_count1.size() == 4);
    TEST_OK(std::strcmp(bs_count1.c_str(), "byte") == 0);

    //
    // Assigns from C string
    //
    byte_string bs_cstr;
    bs_cstr.assign(raw_bytes);
    TEST_OK(std::strcmp(bs_cstr.c_str(), raw_chars) == 0);

    byte_string bs_cstr1;
    bs_cstr1.assign(raw_chars);
    TEST_OK(std::strcmp(bs_cstr1.c_str(), raw_chars) == 0);

    //
    // Assigns from range
    //
    byte_string bs_range;
    bs_range.assign(raw_bytes, raw_bytes + std::strlen(raw_chars));
    TEST_OK(std::strcmp(bs_range.c_str(), raw_chars) == 0);

    byte_string bs_range1;
    bs_range1.assign(raw_chars, raw_chars + std::strlen(raw_chars));
    TEST_OK(std::strcmp(bs_range1.c_str(), raw_chars) == 0);

    byte_string bs_range2;
    bs_range2.assign(bs_from_raw.begin(), bs_from_raw.end());
    TEST_OK(bs_range2.size() == bs_from_raw.size());
    TEST_OK(std::strcmp(bs_range2.c_str(), raw_chars) == 0);
}
