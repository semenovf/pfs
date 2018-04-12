#pragma once

void test_insert ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                          Operations (insert)                          //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    {
        ADD_TESTS(2);
        byte_string bs(bs_sample);

        TEST_OK(bs.size() > 0)

        bs.clear();

        TEST_OK(bs.empty());
    }

    {
        ADD_TESTS(20);
        byte_string bsb("xmplr");
        byte_string bsc("xmplr");

        //
        // byte_string & insert (size_type index, size_type count, value_type ch)
        // byte_string & insert (size_type index, size_type count, char ch)
        //
        byte_string & bs_ref = bsb.insert(0, 1, value_type('E'));
        bs_ref = bsc.insert(0, 1, 'E');
        TEST_OK(std::strcmp(bsb.c_str(), "Exmplr") == 0);
        TEST_OK(std::strcmp(bsc.c_str(), "Exmplr") == 0);

        //
        // byte_string & insert (size_type index, const_pointer s)
        // byte_string & insert (size_type index, char const * s)
        //
        char const * e = "e";
        bs_ref = bsb.insert(2, reinterpret_cast<const_pointer>(e));
        bs_ref = bsc.insert(2, e);
        TEST_OK(std::strcmp(bsb.c_str(), "Exemplr") == 0);
        TEST_OK(std::strcmp(bsc.c_str(), "Exemplr") == 0);

        //
        // byte_string & insert (size_type index, const_pointer s, size_type count)
        // byte_string & insert (size_type index, char const * s, size_type count)
        //
        char const * ol = "1.x";
        bs_ref = bsb.insert(0, reinterpret_cast<const_pointer>(ol), 2);
        bs_ref = bsc.insert(0, ol, 2);
        TEST_OK(std::strcmp(bsb.c_str(), "1.Exemplr") == 0);
        TEST_OK(std::strcmp(bsc.c_str(), "1.Exemplr") == 0);

        //
        // byte_string & insert (size_type index, byte_string const & s)
        // byte_string & insert (size_type index, std::string const & s)
        //
        char const * a = "a";
        bs_ref = bsb.insert(8, byte_string(a));
        bs_ref = bsc.insert(8, std::string(a));
        TEST_OK(std::strcmp(bsb.c_str(), "1.Exemplar") == 0);
        TEST_OK(std::strcmp(bsc.c_str(), "1.Exemplar") == 0);

        //
        // basic_string & insert (size_type index, basic_string const & str
        //           , size_type index_str, size_type count)
        // basic_string & insert (size_type index, std::string const & str
        //           , size_type index_str, size_type count)
        //
        char const * suffix = "xxx is an example string.";
        bs_ref = bsb.insert(10, byte_string(suffix), 3, 14);
        bs_ref = bsc.insert(10, std::string(suffix), 3, 14);
        TEST_OK(std::strcmp(bsb.c_str(), "1.Exemplar is an example") == 0);
        TEST_OK(std::strcmp(bsc.c_str(), "1.Exemplar is an example") == 0);

        //
        // iterator insert (const_iterator pos, value_type ch)
        // iterator insert (const_iterator pos, char ch)
        //
        iterator itb = bsb.insert(bsb.cend(), value_type('.'));
        iterator itc = bsc.insert(bsc.cend(), '.');
        TEST_OK(std::strcmp(bsb.c_str(), "1.Exemplar is an example.") == 0);
        TEST_OK(std::strcmp(bsc.c_str(), "1.Exemplar is an example.") == 0);

        //
        // iterator insert (const_iterator pos, size_type count, value_type ch)
        // iterator insert (const_iterator pos, size_type count, char ch)
        //
        itb = bsb.insert(bsb.cend(), 3, value_type('!'));
        itc = bsc.insert(bsc.cend(), 3, '!');
        TEST_OK(std::strcmp(bsb.c_str(), "1.Exemplar is an example.!!!") == 0);
        TEST_OK(std::strcmp(bsc.c_str(), "1.Exemplar is an example.!!!") == 0);
        TEST_OK(itb == bsb.begin() + 25);
        TEST_OK(itc == bsc.begin() + 25);

        //
        // iterator insert (const_iterator pos, InputIt first, InputIt last)
        //
        std::string dash(" ---");
        itb = bsb.insert(bsb.cbegin() + 10, dash.begin(), dash.end());
        itc = bsc.insert(bsc.cbegin() + 10, dash.begin(), dash.end());
        TEST_OK(std::strcmp(bsb.c_str(), "1.Exemplar --- is an example.!!!") == 0);
        TEST_OK(std::strcmp(bsc.c_str(), "1.Exemplar --- is an example.!!!") == 0);
        TEST_OK(itb == bsb.begin() + 10);
        TEST_OK(itc == bsc.begin() + 10);

#if __cplusplus >= 201103L
        //
        // iterator insert (const_iterator pos, std::initializer_list<value_type> ilist)
        // iterator insert (const_iterator pos, std::initializer_list<char> ilist)
        //
        ADD_TESTS(4);
        bsb = "abcghi";
        bsc = "abcghi";
        itb = bsb.insert(bsb.cbegin() + 3, {value_type('d'), value_type('e'), value_type('f')});
        itc = bsc.insert(bsc.cbegin() + 3, {'d', 'e', 'f'});
        TEST_OK(std::strcmp(bsb.c_str(), "abcdefghi") == 0);
        TEST_OK(std::strcmp(bsc.c_str(), "abcdefghi") == 0);
        TEST_OK(itb == bsb.begin() + 3);
        TEST_OK(itc == bsc.begin() + 3);
#endif

#if __cplusplus > 201402L // since C++17
        // TODO

        //
        // template <typename T>
        // basic_string & insert (size_type pos, T const & t)
        //
        // template <typename T>
        // basic_string & insert (size_type index, T const & t
        //          , size_type index_str, size_type count = npos)
        //

#endif
    }
}

void test_erase ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                          Operations (erase)                           //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(6);

    //
    // byte_string & erase (size_type index = 0, size_type count = npos)
    //
    byte_string bs("This is an example");

    byte_string & bs_ref = bs.erase(0, 5);
    TEST_OK(std::strcmp(bs.c_str(), "is an example") == 0);

    bs_ref = bs.erase(5);
    TEST_OK(std::strcmp(bs_ref.c_str(), "is an") == 0);

    //
    // iterator erase (const_iterator pos)
    //
    iterator it = bs.erase(bs.cbegin() + 2);
    TEST_OK(std::strcmp(bs.c_str(), "isan") == 0);
    TEST_OK(it == bs.cbegin() + 2);

    //
    // iterator erase (const_iterator first, const_iterator last)
    //
    it = bs.erase(bs.cbegin() + 1, bs.cbegin() + 3);
    TEST_OK(std::strcmp(bs.c_str(), "in") == 0);
    TEST_OK(it == bs.cbegin() + 1);
}

void test_push_back ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                          Operations (push_back)                       //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(1);

    byte_string bs;
    bs.push_back('H');
    bs.push_back('e');
    bs.push_back('l');
    bs.push_back(value_type('l'));
    bs.push_back(value_type('o'));
    TEST_OK(std::strcmp(bs.c_str(), "Hello") == 0);
}

void test_pop_back ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                          Operations (pop_back)                        //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(8);

    byte_string bs("Hello");
    TEST_OK(std::strcmp(bs.c_str(), "Hello") == 0);
    bs.pop_back();
    TEST_OK(std::strcmp(bs.c_str(), "Hell") == 0);
    bs.pop_back();
    TEST_OK(std::strcmp(bs.c_str(), "Hel") == 0);
    bs.pop_back();
    TEST_OK(std::strcmp(bs.c_str(), "He") == 0);
    bs.pop_back();
    TEST_OK(std::strcmp(bs.c_str(), "H") == 0);
    bs.pop_back();
    TEST_OK(std::strcmp(bs.c_str(), "") == 0);
    TEST_OK(bs.empty());
    bs.pop_back();
    TEST_OK(bs.empty());
}

void test_append ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                          Operations (append)                          //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    {
        ADD_TESTS(1);
        byte_string bs;

        //
        // byte_string & append (size_type count, value_type ch)
        // byte_string & append (size_type count, char ch)
        //
        byte_string & bs_ref = bs.append(1, value_type('H'));
        bs_ref = bs.append(1, 'e');

        //
        // byte_string & append (byte_string const & s)
        // byte_string & append (std::string const & s)
        //
        bs_ref = bs.append(byte_string("l"));
        bs_ref = bs.append(std::string("l"));

        //
        // byte_string & append (byte_string const & s, size_type pos, size_type count = npos)
        // byte_string & append (std::string const & s, size_type pos, size_type count = npos)
        //
        bs_ref = bs.append(byte_string("oops, "), 1, 1);
        bs_ref = bs.append(std::string("oops, "), 4, 2);

        //
        // byte_string & append (const_pointer s, size_type count)
        // byte_string & append (const char * s, size_type count)
        //
        char const * chars = "World";
        const_pointer bytes = reinterpret_cast<const_pointer>(chars);
        bs_ref = bs.append(bytes, 2);
        bs_ref = bs.append(chars + 2, 2);

        //
        // byte_string & append (const_pointer s)
        // byte_string & append (const char * s)
        //
        chars = "d";
        bytes = reinterpret_cast<const_pointer>(chars);
        chars = "!";
        bs_ref = bs.append(bytes);
        bs_ref = bs.append(chars);

        //
        // template <typename InputIterator>
        // byte_string & append (InputIterator first, InputIterator last)
        //
        std::string s("--- And Bye!---");
        bs_ref = bs.append(s.begin() + 3, s.end() - 3);

        TEST_OK(std::strcmp(bs.c_str(), "Hello, World! And Bye!") == 0);

#if __cplusplus >= 201103L
        //
        // byte_string & append (std::initializer_list<value_type> ilist)
        // byte_string & append (std::initializer_list<char> ilist)
        //
        ADD_TESTS(1);
        bs.clear();
        bs_ref = bs.append({value_type('a'), value_type('b'), value_type('c')});
        bs_ref = bs.append({'d', 'e', 'f'});
        TEST_OK(std::strcmp(bs.c_str(), "abcdef") == 0);
#endif
    }

    {
        ADD_TESTS(1);

        byte_string bs;

        //
        // byte_string & operator += (byte_string const & s)
        // byte_string & operator += (std::string const & s)
        //
        byte_string & bs_ref = bs += byte_string("Hel");
        bs_ref = bs += std::string("lo");

        //
        // byte_string & operator += (value_type ch)
        // byte_string & operator += (char ch)
        //
        bs_ref = bs += value_type(',');
        bs_ref = bs += ' ';

        //
        // byte_string & operator += (const_pointer s)
        // operator += (const char * s)
        //
        char const * chars = "Wor";
        const_pointer bytes = reinterpret_cast<const_pointer>(chars);
        bs_ref = bs += bytes;
        bs_ref = bs += "ld!";

        //TEST_OK(std::strcmp(bs.c_str(), "Hello, World! And Bye!") == 0);
        TEST_OK(std::strcmp(bs.c_str(), "Hello, World!") == 0);

#if __cplusplus >= 201103L
        ADD_TESTS(1);

        bs.clear();

        //
        // byte_string & operator += (std::initializer_list<value_type> ilist)
        // byte_string & operator += (std::initializer_list<char> ilist)
        //
        bs_ref = bs.append({value_type('a'), value_type('b'), value_type('c')});
        bs_ref = bs.append({'d', 'e', 'f'});
        TEST_OK(std::strcmp(bs.c_str(), "abcdef") == 0);

#endif
    }
}

void test_compare ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                          Operations (compare)                         //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(0);
    //
    // int compare (byte_string const & s) const
    //
}

void test_starts_with ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                        Operations (starts_with)                       //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(0);
}

void test_ends_with ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                        Operations (ends_with)                         //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(0);
}

void test_replace ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                        Operations (replace)                           //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(0);
}

void test_substr ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                        Operations (substr)                           //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(4);

    byte_string bs("0123456789abcdifghij");

    TEST_OK(bs.substr() == bs);
    TEST_OK(bs.substr(10) == byte_string("abcdifghij"));
    TEST_OK(bs.substr(5, 3) == byte_string("567"));
    TEST_OK(bs.substr(bs.size() - 3, 50) == byte_string("hij"));
}

void test_copy ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                        Operations (copy)                              //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(2);

    char const * chars = "__cplusplus";
    byte_string bs(chars);
    value_type bytes[20];

    TEST_OK(bs.size() == bs.copy(bytes, sizeof(bytes)));

    bytes[bs.size()] = '\x0';
    TEST_OK(std::strcmp(chars, reinterpret_cast<char const *>(bytes)) == 0);
}

void test_resize ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                        Operations (resize)                            //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(4);

    byte_string bs_long("This is a vary long string");
    byte_string bs_short("short string");

    bs_long.resize(9);
    TEST_OK(bs_long.size() == 9);
    TEST_OK(std::strcmp(bs_long.c_str(), "This is a") == 0);

    bs_short.resize(15, '-');
    TEST_OK(bs_short.size() == 15);
    TEST_OK(std::strcmp(bs_short.c_str(), "short string---") == 0);
}

void test_swap ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                        Operations (swap)                              //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(4);

    byte_string a("AAA");
    byte_string b("BBB");

    TEST_OK(std::strcmp(a.c_str(), "AAA") == 0)
    TEST_OK(std::strcmp(b.c_str(), "BBB") == 0)

    a.swap(b);

    TEST_OK(std::strcmp(a.c_str(), "BBB") == 0)
    TEST_OK(std::strcmp(b.c_str(), "AAA") == 0)
}

void test_operations ()
{
    test_insert();
    test_erase();
    test_push_back();
    test_pop_back();
    test_append();
    test_compare();
    test_starts_with();
    test_ends_with();
    test_replace();
    test_substr();
    test_copy();
    test_resize();
    test_swap();
}
