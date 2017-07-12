/* 
 * File:   test_iterator.hpp
 * Author: wladt
 *
 * Created on July 12, 2017, 9:04 PM
 */

#ifndef TEST_ITERATOR_HPP
#define TEST_ITERATOR_HPP

template <typename StringImplType>
void test_iterator ()
{
    typedef pfs::safeformat_iterator<StringImplType> fmt_iterator;
    typedef typename fmt_iterator::string_type       string_type;
}

#endif /* TEST_ITERATOR_HPP */

