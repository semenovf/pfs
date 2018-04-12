#pragma once

void test_operations ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             Operations                                //\n";
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
        bsb.insert(0, 1, value_type('E'));
        bsc.insert(0, 1, 'E');
        TEST_OK(std::strcmp(bsb.c_str(), "Exmplr") == 0);
        TEST_OK(std::strcmp(bsc.c_str(), "Exmplr") == 0);
        
        //
        // byte_string & insert (size_type index, const_pointer s)
        // byte_string & insert (size_type index, char const * s)
        //
        char const * e = "e";
        bsb.insert(2, reinterpret_cast<const_pointer>(e));
        bsc.insert(2, e);
        TEST_OK(std::strcmp(bsb.c_str(), "Exemplr") == 0);
        TEST_OK(std::strcmp(bsc.c_str(), "Exemplr") == 0);

        //
        // byte_string & insert (size_type index, const_pointer s, size_type count)
        // byte_string & insert (size_type index, char const * s, size_type count)
        //
        char const * ol = "1.x";
        bsb.insert(0, reinterpret_cast<const_pointer>(ol), 2);
        bsc.insert(0, ol, 2);
        TEST_OK(std::strcmp(bsb.c_str(), "1.Exemplr") == 0);
        TEST_OK(std::strcmp(bsc.c_str(), "1.Exemplr") == 0);
        
        //
        // byte_string & insert (size_type index, byte_string const & s)
        // byte_string & insert (size_type index, std::string const & s)
        //
        char const * a = "a";
        bsb.insert(8, byte_string(a));
        bsc.insert(8, std::string(a));
        TEST_OK(std::strcmp(bsb.c_str(), "1.Exemplar") == 0);
        TEST_OK(std::strcmp(bsc.c_str(), "1.Exemplar") == 0);
        
        //
        // basic_string& insert (size_type index, basic_string const & str
        //           , size_type index_str, size_type count)
        // basic_string & insert (size_type index, std::string const & str
        //           , size_type index_str, size_type count)
        //
        char const * suffix = "xxx is an example string.";
        bsb.insert(10, byte_string(suffix), 3, 14);
        bsc.insert(10, std::string(suffix), 3, 14);
        TEST_OK(std::strcmp(bsb.c_str(), "1.Exemplar is an example") == 0);
        TEST_OK(std::strcmp(bsc.c_str(), "1.Exemplar is an example") == 0);
        
        //
        // iterator insert (const_iterator pos, value_type ch)
        // iterator insert (const_iterator pos, char ch)
        //
        bsb.insert(bsb.cend(), value_type('.'));
        bsc.insert(bsc.cend(), '.');
        TEST_OK(std::strcmp(bsb.c_str(), "1.Exemplar is an example.") == 0);
        TEST_OK(std::strcmp(bsc.c_str(), "1.Exemplar is an example.") == 0);

        //
        // iterator insert (const_iterator pos, size_type count, value_type ch)
        // iterator insert (const_iterator pos, size_type count, char ch)
        //
        iterator bsb_it = bsb.insert(bsb.cend(), 3, value_type('!'));
        iterator bsc_it = bsc.insert(bsc.cend(), 3, '!');
        TEST_OK(std::strcmp(bsb.c_str(), "1.Exemplar is an example.!!!") == 0);
        TEST_OK(std::strcmp(bsc.c_str(), "1.Exemplar is an example.!!!") == 0);
        TEST_OK(bsb_it == bsb.begin() + 25);
        TEST_OK(bsc_it == bsc.begin() + 25);

        //
        // insert (const_iterator pos, InputIt first, InputIt last)
        //
        std::string dash(" ---");
        bsb_it = bsb.insert(bsb.cbegin() + 10, dash.begin(), dash.end());
        bsc_it = bsc.insert(bsc.cbegin() + 10, dash.begin(), dash.end());
        TEST_OK(std::strcmp(bsb.c_str(), "1.Exemplar --- is an example.!!!") == 0);
        TEST_OK(std::strcmp(bsc.c_str(), "1.Exemplar --- is an example.!!!") == 0);
        TEST_OK(bsb_it == bsb.begin() + 10);
        TEST_OK(bsc_it == bsc.begin() + 10);
        
#if __cplusplus >= 201103L
        //
        // iterator insert (const_iterator pos, std::initializer_list<value_type> ilist)
        // iterator insert (const_iterator pos, std::initializer_list<char> ilist)
        //
        ADD_TESTS(4);
        bsb = "abcghi";
        bsc = "abcghi";
        bsb_it = bsb.insert(bsb.cbegin() + 3, {value_type('d'), value_type('e'), value_type('f')});
        bsc_it = bsc.insert(bsc.cbegin() + 3, {'d', 'e', 'f'});
        TEST_OK(std::strcmp(bsb.c_str(), "abcdefghi") == 0);
        TEST_OK(std::strcmp(bsc.c_str(), "abcdefghi") == 0);
        TEST_OK(bsb_it == bsb.begin() + 3);
        TEST_OK(bsc_it == bsc.begin() + 3);
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
