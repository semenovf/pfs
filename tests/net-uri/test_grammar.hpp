#pragma once
#include "pfs/fsm/test.hpp"

template <typename StringT>
void test_grammar ()
{
    ADD_TESTS(34);

    typedef pfs::net::uri_grammar<StringT> uri_grammar;
    typedef pfs::fsm::test_valid_entry_seq<uri_grammar> test_valid;
    typedef pfs::fsm::test_invalid_entry_seq<uri_grammar> test_invalid;

    uri_grammar grammar;

    TEST_OK(test_valid()(grammar.p_unreserved_tr, "0"));
    TEST_OK(test_valid()(grammar.p_unreserved_tr, "9"));
    TEST_OK(test_valid()(grammar.p_unreserved_tr, "A"));
    TEST_OK(test_valid()(grammar.p_unreserved_tr, "Z"));
    TEST_OK(test_valid()(grammar.p_unreserved_tr, "a"));
    TEST_OK(test_valid()(grammar.p_unreserved_tr, "z"));
    TEST_OK(test_valid()(grammar.p_unreserved_tr, "-"));
    TEST_OK(test_valid()(grammar.p_unreserved_tr, "."));
    TEST_OK(test_valid()(grammar.p_unreserved_tr, "_"));
    TEST_OK(test_valid()(grammar.p_unreserved_tr, "~"));
    TEST_OK(test_invalid()(grammar.p_unreserved_tr, "!", 0));
    TEST_OK(test_invalid()(grammar.p_unreserved_tr, "+", 0));

    TEST_OK(test_valid()(grammar.p_pct_encoded_tr, "%00"));
    TEST_OK(test_valid()(grammar.p_pct_encoded_tr, "%1F"));
    TEST_OK(test_valid()(grammar.p_pct_encoded_tr, "%FF"));
    TEST_OK(test_invalid()(grammar.p_pct_encoded_tr, "%0", 0));
    TEST_OK(test_invalid()(grammar.p_pct_encoded_tr, "%0FA", 3));

    TEST_OK(test_valid()(grammar.p_pchar_tr, "0"));
    TEST_OK(test_valid()(grammar.p_pchar_tr, "9"));
    TEST_OK(test_valid()(grammar.p_pchar_tr, "A"));
    TEST_OK(test_valid()(grammar.p_pchar_tr, "Z"));
    TEST_OK(test_valid()(grammar.p_pchar_tr, "a"));
    TEST_OK(test_valid()(grammar.p_pchar_tr, "z"));
    TEST_OK(test_valid()(grammar.p_pchar_tr, "-"));
    TEST_OK(test_valid()(grammar.p_pchar_tr, "."));
    TEST_OK(test_valid()(grammar.p_pchar_tr, "_"));
    TEST_OK(test_valid()(grammar.p_pchar_tr, "~"));
    TEST_OK(test_valid()(grammar.p_pchar_tr, "%00"));
    TEST_OK(test_valid()(grammar.p_pchar_tr, "%1F"));
    TEST_OK(test_valid()(grammar.p_pchar_tr, "%FF"));
    TEST_OK(test_valid()(grammar.p_pchar_tr, "!"));
    TEST_OK(test_valid()(grammar.p_pchar_tr, "+"));
    TEST_OK(test_invalid()(grammar.p_pchar_tr, "%0", 0));
    TEST_OK(test_invalid()(grammar.p_pchar_tr, "%0FA", 3));

    // number = [ minus ] int [ frac ] [ exp ]
//    TEST_OK(test_valid_entry()(grammar.p_number_tr
//            , 0
//            , iterator(DIGIT9_1.begin())
//            , iterator(DIGIT9_1.end())));

//	TEST_OK(test_valid_entry()(
//              uri_grammar_type::p_authority_tr
//            , 0
//            , sequence_type("192.168.1.1")));

//	TEST_OK((pfs::fsm::test_valid_entry<string_type>(
//              uri_grammar_type::p_authority_tr
//            , 0
//            , string_type("user@192.168.1.1"))));

//	// ALPHA / DIGIT / "-" / "." / "_" / "~"
//	//
//	TEST_OK(test_valid_entry<string_type>(pfs::unreserved_tr, 0, _u8("Z")));
//	TEST_OK(test_valid_entry<string_type>(pfs::unreserved_tr, 0, _u8("z")));
//	TEST_OK(test_valid_entry<string_type>(pfs::unreserved_tr, 0, _u8("9")));
//	TEST_OK(test_valid_entry<string_type>(pfs::unreserved_tr, 0, _u8("~")));
//
//	TEST_OK(test_invalid_entry<string_type>(pfs::unreserved_tr, 0, _u8("?"), -1));
//	TEST_OK(test_invalid_entry<string_type>(pfs::unreserved_tr, 0, _u8("+"), -1));
//	TEST_OK(test_invalid_entry<string_type>(pfs::unreserved_tr, 0, _u8("/"), -1));
//
//	// "%" HEXDIG HEXDIG
//	//
//	TEST_OK(test_valid_entry<string_type>(pfs::pct_encoded_tr, 0, _u8("%FF")));
//	TEST_OK(test_valid_entry<string_type>(pfs::pct_encoded_tr, 0, _u8("%00")));
//	TEST_OK(test_valid_entry<string_type>(pfs::pct_encoded_tr, 0, _u8("%9F")));
//	TEST_OK(test_valid_entry<string_type>(pfs::pct_encoded_tr, 0, _u8("%AB")));
//
//	TEST_OK(test_invalid_entry<string_type>(pfs::pct_encoded_tr, 0, _u8("%AR"), -1));
//	TEST_OK(test_invalid_entry<string_type>(pfs::pct_encoded_tr, 0, _u8("}{"), -1));
//	TEST_OK(test_invalid_entry<string_type>(pfs::pct_encoded_tr, 0, _u8("%%A9"), -1));
//
//
//	// "/" [ segment-nz *( "/" segment ) ]
//	//
//	TEST_OK(test_valid_entry<string_type>(pfs::path_absolute_tr, 0, _u8("/")));
//	TEST_OK(test_valid_entry<string_type>(pfs::path_absolute_tr, 0, _u8("/ABCDE")));
//	TEST_OK(test_valid_entry<string_type>(pfs::path_absolute_tr, 0, _u8("/name@domain.com/%DE%AD%BE%EF")));
//	TEST_OK(test_valid_entry<string_type>(pfs::path_absolute_tr, 0, _u8("/name@domain.com/")));
//
//	TEST_OK(test_invalid_entry<string_type>(pfs::path_absolute_tr, 0, _u8("/name@{}/"), 6));
//	TEST_OK(test_invalid_entry<string_type>(pfs::path_absolute_tr, 0, _u8("name@{}/"), -1));
//
//
//	// URI / relative-ref
//	//
//	TEST_OK(test_valid_entry<string_type>(pfs::uri_reference_tr, 0, _u8("http://user@host/?query%20string")));
//	TEST_OK(test_valid_entry<string_type>(pfs::uri_reference_tr, 0, _u8("http://user@host#fragment%20string")));
//	TEST_OK(test_valid_entry<string_type>(pfs::uri_reference_tr, 0, _u8("ftp://user@host/?query%20string#fragment%20string")));
}
