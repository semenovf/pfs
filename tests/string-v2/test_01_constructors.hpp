#ifndef __TEST_01_CONSTRUCTORS_HPP__
#define __TEST_01_CONSTRUCTORS_HPP__

enum {
    DEFAULT_CONSTRUCTOR = 1
};

template <typename BaseClass>
struct MyString : public pfs::string<BaseClass>
{
    typedef pfs::string<BaseClass> base_class;

    int flag;

    MyString () : base_class()
    {
        flag = DEFAULT_CONSTRUCTOR;
    }
//
//    /**
//     * Initializes with reference to native container
//     */
//    string (native_reference rhs)
//        : base_class(rhs)
//    {}
//
//    string (const_native_reference rhs)
//        : base_class(rhs)
//    {}
//
//    string (const_pointer s, size_type n)
//        : base_class(s, n)
//    {}
//
//    string (char const * s)
//        : base_class(s)
//    {}
//
//    /**
//     * @fn string::string (std::string const & s).
//     *
//     * @brief Constructs string from std::string.
//     * @param s value.
//     */
//    template <typename T>
//    string (T const & s, typename enable_if<is_same<T, std::string>::value
//            && !is_same<T, native_type>::value, T>::type * = 0)
//        : base_class(s)
//    {}
//
//    // TODO
////    template <typename T>
////    string (typename enable_if<is_same<T, std::wstring>::value && !is_same<T, native_type>::value, T>::type const & s, T * = 0)
////        : _p(s)
////    {}
//
//    string (iterator first, iterator last)
//        : base_class(first.base(), last.base())
//    {}
//
//    string (const_iterator first, const_iterator last)
//        : base_class(first.base(), last.base())
//    {}
//
//    string (typename base_class::iterator first
//            , typename base_class::iterator last)
//        : base_class(first, last)
//    {}
//
//    string (typename base_class::const_iterator first
//            , typename base_class::const_iterator last)
//        : base_class(first, last)
//    {}
//
//    template <typename InputIt>
//    string (InputIt first, InputIt last)
//        : base_class(first, last)
//    {}
//
//    string (size_type count, value_type ch)
//    {
//        while (count--)
//            this->push_back(ch);
//    }
};

template <typename BaseClass>
void test_constructors ()
{
    ADD_TESTS(1);

    typedef MyString<BaseClass> string_type;

    string_type s0; // Default constructor

    TEST_OK(s0.flag == DEFAULT_CONSTRUCTOR);

    //string_type s1;

}

#endif /* __TEST_01_CONSTRUCTORS_HPP__ */
