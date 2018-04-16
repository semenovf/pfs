#pragma once

template <typename StringT>
void test_constructors ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             Constructors                              //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(0);

    //
    // Default constructor
    //
    StringT bs_default;

//     //
//     // Construct from const pointer
//     //
//     byte_string bs_from_raw(raw_bytes);
//     TEST_OK(bs_from_raw.size() == std::strlen(raw_chars));
//     TEST_OK(std::strcmp(bs_from_raw.c_str(), raw_chars) == 0);
//
//     //
//     // Copy constructor
//     //
//     byte_string bs_copy(bs_from_raw);
//     TEST_OK(std::strcmp(bs_copy.c_str(), raw_chars) == 0);
//
//     //
//     // Construct from const pointer with limit
//     //
//     byte_string bs_from_raw1(raw_bytes, 4);
//     TEST_OK(bs_from_raw1.size() == 4);
//     TEST_OK(std::strncmp(bs_from_raw1.c_str(), raw_chars, 4) == 0);
//
//     //
//     // Copy constructor with limits
//     //
//     byte_string bs_copy1(bs_from_raw, 0);
//     byte_string bs_copy2(bs_from_raw, 5);
//     byte_string bs_copy3(bs_from_raw, 2, 5);
//
//     TEST_OK(std::strcmp(bs_copy1.c_str(), raw_chars) == 0);
//     TEST_OK(std::strcmp(bs_copy2.c_str(), "string") == 0);
//     TEST_OK(std::strcmp(bs_copy3.c_str(), "te_st") == 0);
//
//     //
//     // Construct from C string
//     //
//     byte_string bs_cstr(raw_chars);
//     TEST_OK(std::strcmp(bs_cstr.c_str(), raw_chars) == 0);
//
//     //
//     // Construct from C string with limit
//     //
//     byte_string bs_cstr1(raw_chars, 4);
//     TEST_OK(std::strncmp(bs_cstr1.c_str(), raw_chars, 4) == 0);
//
//     //
//     // Construct from value repeate sequence
//     //
//     byte_string bs_rpt(4, byte_string::value_type('b'));
//     TEST_OK(std::strcmp(bs_rpt.c_str(), "bbbb") == 0);
//
//     //
//     // Construct from char repeate sequence
//     //
//     byte_string bs_rpt1(4, 'b');
//     TEST_OK(std::strcmp(bs_rpt1.c_str(), "bbbb") == 0);
//
//     //
//     // Construct from range
//     //
//     byte_string bs_range(raw_bytes, raw_bytes + std::strlen(raw_chars));
//     TEST_OK(std::strcmp(bs_range.c_str(), raw_chars) == 0);
//
//     byte_string bs_range1(raw_chars, raw_chars + std::strlen(raw_chars));
//     TEST_OK(std::strcmp(bs_range1.c_str(), raw_chars) == 0);
//
//     byte_string bs_range2(bs_from_raw.begin(), bs_from_raw.end());
//     TEST_OK(bs_range2.size() == bs_from_raw.size());
//     TEST_OK(std::strcmp(bs_range2.c_str(), raw_chars) == 0);
}
