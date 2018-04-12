#pragma once

void test_iterators ()
{
    ADD_TESTS(37);

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             Iterators                                 //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    byte_string bs_mutable(bs_sample);
    byte_string const bs_const(bs_sample);

    iterator first1 = bs_mutable.begin();
    iterator last1  = bs_mutable.end();

    TEST_OK(*first1++ == 'b');
    TEST_OK(*first1++ == 'y');
    TEST_OK(*first1++ == 't');
    TEST_OK(*first1++ == 'e');
    TEST_OK(*first1++ == '_');
    TEST_OK(*--last1 == 'g');
    TEST_OK(*--last1 == 'n');
    TEST_OK(*--last1 == 'i');
    TEST_OK(*--last1 == 'r');
    TEST_OK(*--last1 == 't');
    TEST_OK(*--last1 == 's');
    TEST_OK(first1 == last1);
    TEST_OK(first1 <= last1);
    TEST_OK(first1 >= last1);

    first1 = bs_mutable.begin();
    last1  = bs_mutable.end();
    *first1++ = 'e';
    *first1++ = 't';
    *first1++ = 'y';
    *first1++ = 'b';
    *--last1 = 's';
    *--last1 = 't';
    *--last1 = 'r';
    *--last1 = 'i';
    *--last1 = 'n';
    *--last1 = 'g';
    TEST_OK(std::strcmp(bs_mutable.c_str(), "etyb_gnirts") == 0)

    const_iterator first2 = bs_mutable.cbegin();
    const_iterator last2  = bs_mutable.cend();

    TEST_OK(first2 < last2);
    TEST_OK(last2 > first2);

    const_iterator first3 = bs_const.begin();
    const_iterator last3 = bs_const.end();

    TEST_OK(first3 + 5 == last3 - 6);
    TEST_OK(*(first3 + 5) == 's');
    TEST_OK(*(last3 - 6) == 's');

    const_iterator first4 = bs_const.cbegin();
    const_iterator last4 = bs_const.cend();

    TEST_OK(first4 != last4);


    //
    // Reverse iterator
    //
    reverse_iterator first5 = bs_mutable.rbegin();
    reverse_iterator last5  = bs_mutable.rend();

    TEST_OK(*first5++ == 's');
    TEST_OK(*first5++ == 't');
    TEST_OK(*first5++ == 'r');
    TEST_OK(*first5++ == 'i');
    TEST_OK(*first5++ == 'n');
    TEST_OK(*first5++ == 'g');
    TEST_OK(*--last5 == 'e');
    TEST_OK(*--last5 == 't');
    TEST_OK(*--last5 == 'y');
    TEST_OK(*--last5 == 'b');
    --last5;
    TEST_OK(first5 == last5);
    TEST_OK(first5 <= last5);
    TEST_OK(first5 >= last5);

    first5 = bs_mutable.rbegin();
    last5  = bs_mutable.rend();
    *first5++ = 'g';
    *first5++ = 'n';
    *first5++ = 'i';
    *first5++ = 'r';
    *first5++ = 't';
    *first5++ = 's';
    *--last5 = 'b';
    *--last5 = 'y';
    *--last5 = 't';
    *--last5 = 'e';
    TEST_OK(std::strcmp(bs_mutable.c_str(), "byte_string") == 0)

    const_reverse_iterator first6 = bs_mutable.crbegin();
    const_reverse_iterator last6  = bs_mutable.crend();

    TEST_OK(first6 < last6);
    TEST_OK(last6 > first6);
}
