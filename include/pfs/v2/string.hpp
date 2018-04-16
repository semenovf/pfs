#pragma once
#include <pfs/v2/stdcxx/basic_string.hpp>

namespace pfs {

class string : public stdcxx::basic_string<char, string>
{
    typedef stdcxx::basic_string<char, string> base_class;

// public:
//     typedef typename base_class::value_type       value_type;
//     typedef typename base_class::traits_type      traits_type;
//     typedef typename base_class::size_type        size_type;
//     typedef typename base_class::difference_type  difference_type;
//     typedef typename base_class::reference        reference;
//     typedef typename base_class::const_reference  const_reference;
//     typedef typename base_class::pointer          pointer;
//     typedef typename base_class::const_pointer    const_pointer;
//     typedef typename base_class::iterator         iterator;
//     typedef typename base_class::const_iterator   const_iterator;
//     typedef typename base_class::reverse_iterator reverse_iterator;
//     typedef typename base_class::const_reverse_iterator const_reverse_iterator;

public:
    string () : base_class () {}

    string (string const & other)
        : base_class(other)
    {}

    string (string const & other
            , size_type pos
            , size_type count = base_class::npos)
        : base_class(other, pos, count)
    {}

    string (const_pointer s)
        : base_class(s)
    {}

    string (const_pointer s, size_type n)
        : base_class(s, n)
    {}

    string (size_type n, value_type c)
        : base_class(n, c)
    {}

    template <typename InputIterator>
    string (InputIterator first, InputIterator last)
        : base_class(first, last)
    {}

    ~string ()
    {}

//     /**
//      * @fn string::string (std::string const & s).
//      *
//      * @brief Constructs string from std::string.
//      * @param s value.
//      */
//     template <typename T>
//     string (T const & s, typename enable_if<is_same<T, std::string>::value
//             && !is_same<T, native_type>::value, T>::type * = 0)
//         : base_class(s)
//     {}
};

} // pfs
