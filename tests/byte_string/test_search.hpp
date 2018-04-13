#pragma once

void test_search ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                               Search                                  //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(46)

    //                        1         2
    //              012345678901234567890
    byte_string bs("This is a byte string");

    //
    // find
    //

    //
    // size_type find (byte_string const & bs, size_type pos = 0) const;
    //
    TEST_OK(bs.find(byte_string("is")) == 2);
    TEST_OK(bs.find(byte_string("is"), 3) == 5);

    //
    // size_type find (const_pointer p, size_type pos, size_type count) const;
    //
    TEST_OK(bs.find(byte_string("byte-xxx").data(), 0, 4) == 10);

    //
    // size_type find (const_pointer p, size_type pos = 0) const;
    //
    TEST_OK(bs.find(byte_string("byte").data()) == 10);
    TEST_OK(bs.find(byte_string("byte").data(), 5) == 10);

    //
    // size_type find (value_type ch, size_type pos = 0) const;
    //
    TEST_OK(bs.find(value_type('y')) == 11);
    TEST_OK(bs.find(value_type('y'), 5) == 11);
    TEST_OK(bs.find(value_type('z')) == byte_string::npos);


    //
    // rfind
    //

    //
    // size_type rfind (byte_string const & bs, size_type pos = npos) const;
    //
    TEST_OK(bs.rfind(byte_string("is")) == 5);
    TEST_OK(bs.rfind(byte_string("is"), 3) == 2);

    //
    // size_type rfind (const_pointer p, size_type pos, size_type count) const;
    //
    TEST_OK(bs.rfind(byte_string("byte-xxx").data(), 20, 4) == 10);

    //
    // size_type rfind (const_pointer p, size_type pos = npos) const;
    //
    TEST_OK(bs.rfind(byte_string("byte").data()) == 10);
    TEST_OK(bs.rfind(byte_string("byte").data(), 14) == 10);

    //
    // size_type rfind (value_type ch, size_type pos = 0) const;
    //
    TEST_OK(bs.rfind(value_type('y')) == 11);
    TEST_OK(bs.rfind(value_type('y'), 11) == 11);
    TEST_OK(bs.rfind(value_type('z')) == byte_string::npos);


    //
    // find_first_of
    //

    //
    // size_type find_first_of (byte_string const & bs, size_type pos = 0) const;
    //
    TEST_OK(bs.find_first_of(byte_string("xiz")) == 2);
    TEST_OK(bs.find_first_of(byte_string("xiz"), 3) == 5);

    //
    // size_type find_first_of (const_pointer p, size_type pos, size_type count) const;
    //
    TEST_OK(bs.find_first_of(byte_string("xiz").data(), 3, 1) == byte_string::npos);
    TEST_OK(bs.find_first_of(byte_string("xiz").data(), 3, 2) == 5);
    TEST_OK(bs.find_first_of(byte_string("xiz").data(), 3, 3) == 5);

    //
    // size_type find_first_of (const_pointer p, size_type pos = 0) const;
    //
    TEST_OK(bs.find_first_of(byte_string("xiz").data()) == 2);
    TEST_OK(bs.find_first_of(byte_string("xiz").data(), 3) == 5);
    TEST_OK(bs.find_first_of(byte_string("xiz").data(), 19) == byte_string::npos);

    //
    // size_type find_first_of (value_type ch, size_type pos = 0) const;
    //
    TEST_OK(bs.find_first_of(value_type('i')) == 2);
    TEST_OK(bs.find_first_of(value_type('i'), 3) == 5);
    TEST_OK(bs.find_first_of(value_type('z')) == byte_string::npos);

    //
    // find_first_not_of
    //

    //
    // size_type find_first_not_of (byte_string const & bs, size_type pos = 0) const
    //
    TEST_OK(bs.find_first_not_of(byte_string("Ths abyterng")) == 2);
    TEST_OK(bs.find_first_not_of(byte_string("Ths abyterng"), 3) == 5);
    TEST_OK(bs.find_first_not_of(bs) == byte_string::npos);

    //
    // size_type find_first_not_of (const_pointer p, size_type pos, size_type count) const
    //
    TEST_OK(bs.find_first_not_of(byte_string("Ths abyterng").data(), 0, 12) == 2);
    TEST_OK(bs.find_first_not_of(byte_string(bs).data(), 0, bs.size()) == byte_string::npos);

    //
    // size_type find_first_not_of (const_pointer p, size_type pos = 0) const
    //
    TEST_OK(bs.find_first_not_of(byte_string("Ths abyterng").data(), 1) == 2);
    TEST_OK(bs.find_first_not_of(byte_string(bs).data(), 1) == byte_string::npos);

    //
    // size_type find_first_not_of (value_type ch, size_type pos = 0) const
    //
    TEST_OK(bs.find_first_not_of(value_type('T'), 0) == 1);
    TEST_OK(bs.find_first_not_of(value_type('T')) == 1);

    //
    // find_last_of
    //
      //                        1         2
      //              012345678901234567890
//    byte_string bs("This is a byte string");

    //
    // size_type find_last_of (byte_string const & bs, size_type pos = 0) const
    //
    TEST_OK(bs.find_last_of(byte_string("xiz")) == 18);
    TEST_OK(bs.find_last_of(byte_string("xiz"), 3) == 2);
    TEST_OK(bs.find_last_of(byte_string("xiz"), 1) == byte_string::npos);

    //
    // size_type find_last_of (const_pointer p, size_type pos, size_type count) const
    //
    TEST_OK(bs.find_last_of(byte_string("xiz").data(), 19, 3) == 18);
    TEST_OK(bs.find_last_of(byte_string("xiz").data(), 19, 1) == byte_string::npos);

    //
    // size_type find_last_of (const_pointer p, size_type pos = 0) const
    //
    TEST_OK(bs.find_last_of(byte_string("xiz").data(), 19) == 18);
    TEST_OK(bs.find_last_of(byte_string("xiz").data(), 1) == byte_string::npos);

    //
    // size_type find_last_of (value_type ch, size_type pos = 0) const
    //
    TEST_OK(bs.find_last_of(value_type('T'), 0) == 0);
    TEST_OK(bs.find_last_of(value_type('T')) == 0);
    TEST_OK(bs.find_last_of(value_type('z')) == byte_string::npos);

    //
    // size_type find_last_not_of (byte_string const & bs, size_type pos = 0) const
    //

    //
    // size_type find_last_not_of (const_pointer p, size_type pos, size_type count) const
    //

    //
    // size_type find_last_not_of (const_pointer p, size_type pos = 0) const
    //

    //
    // size_type find_last_not_of (value_type ch, size_type pos = 0) const
    //

}
