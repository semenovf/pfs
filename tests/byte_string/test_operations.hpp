#pragma once

void test_operations ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             Operations                                //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    {
        ADD_TESTS(2);
        pfs::byte_string bs(bs_sample);

        TEST_OK(bs.size() > 0)

        bs.clear();

        TEST_OK(bs.empty());
    }

    {
        ADD_TESTS(3);
        pfs::byte_string bs("xmplr");

        // byte_string & insert (size_type index, size_type count, char ch)
        bs.insert(0, 1, 'E');
        TEST_OK(std::strcmp(bs.c_str(), "Exmplr") == 0);

        // byte_string & insert (size_type index, char const * s)
        bs.insert(2, "e");
        TEST_OK(std::strcmp(bs.c_str(), "Exemplr") == 0);

        // byte_string & insert (size_type index, std::string const & s)
        bs.insert(6, std::string("a"));
        TEST_OK(std::strcmp(bs.c_str(), "Exemplar") == 0);
    }
}
