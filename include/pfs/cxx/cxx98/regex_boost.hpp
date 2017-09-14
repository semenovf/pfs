
/* 
 * File:   regex_boost.hpp
 * Author: wladt
 *
 * Created on September 13, 2017, 1:53 PM
 */

#ifndef __PFS_CXX_CXX98_REGEX_BOOST_HPP__
#define __PFS_CXX_CXX98_REGEX_BOOST_HPP__

#include <pfs/cxxlang.hpp>
#include <boost/regex.hpp>

namespace pfs {

namespace regex_constants {

typedef ::boost::regex_constants::match_flag_type match_flag_type;

static const match_flag_type match_default     = ::boost::regex_constants::match_default;
static const match_flag_type match_not_bol     = ::boost::regex_constants::match_not_bol;
static const match_flag_type match_not_eol     = ::boost::regex_constants::match_not_eol;
static const match_flag_type match_not_bow     = ::boost::regex_constants::match_not_bow;
static const match_flag_type match_not_eow     = ::boost::regex_constants::match_not_eow;
static const match_flag_type match_any         = ::boost::regex_constants::match_any;
static const match_flag_type match_not_null    = ::boost::regex_constants::match_not_null;
static const match_flag_type match_continuous  = ::boost::regex_constants::match_continuous;
static const match_flag_type match_prev_avail  = ::boost::regex_constants::match_prev_avail;
static const match_flag_type format_default    = ::boost::regex_constants::format_default;
static const match_flag_type format_sed        = ::boost::regex_constants::format_sed;
static const match_flag_type format_no_copy    = ::boost::regex_constants::format_no_copy;
static const match_flag_type format_first_only = ::boost::regex_constants::format_first_only;

} // regex_constants

template <typename StringType>
class sub_match : public ::boost::sub_match<typename StringType::const_pointer>
{
    typedef ::boost::sub_match<typename StringType::const_pointer> base_class;
    
public:
    pfs_constexpr sub_match () : base_class() { }
    
    operator StringType () const
    {
        return this->matched
            ? StringType(this->first, this->second)
            : StringType();
    }
};

template <typename BidirIter>
class match_results : public ::boost::match_results<BidirIter>
{
    typedef ::boost::match_results<BidirIter> base_class;
    
public:
    explicit match_results ()
        : base_class()
    {}

    /**
     * @brief Copy constructs a %match_results.
     */
    match_results (match_results const & rhs)
        : base_class(rhs)
    {}
};

template <typename StringType> 
class basic_regex : public ::boost::basic_regex<typename StringType::code_unit_type>
{
    typedef ::boost::basic_regex<typename StringType::code_unit_type> base_class;
    
public:
    typedef pfs::match_results<typename StringType::const_pointer> match_results;
    typedef typename base_class::flag_type flag_type;

public:
    basic_regex() : base_class() {}

    /**
     * @brief Constructs a basic regular expression from the
     * sequence [p, p + char_traits<_Ch_type>::length(p))
     * interpreted according to the flags in @p f.
     *
     * @param p A pointer to the start of a C-style null-terminated string
     *          containing a regular expression.
     * @param f Flags indicating the syntax rules and options.
     *
     * @throws regex_error if @p p is not a valid regular expression.
     */
    explicit basic_regex (typename StringType::const_pointer p
            , flag_type f = base_class::ECMAScript)
        : base_class(p, f)
    {}

    /**
     * @brief Constructs a basic regular expression from the sequence
     *        [p, p + len) interpreted according to the flags in @p f.
     *
     * @param p   A pointer to the start of a string containing a regular
     *            expression.
     * @param len The length of the string containing the regular
     *            expression.
     * @param f   Flags indicating the syntax rules and options.
     *
     * @throws regex_error if @p p is not a valid regular expression.
     */
    basic_regex (typename StringType::const_pointer p
            , std::size_t len
            , flag_type f = base_class::ECMAScript)
        : base_class(p, len, f)
    { }

    /**
     * @brief Copy-constructs a basic regular expression.
     *
     * @param __rhs A @p regex object.
     */
    basic_regex (basic_regex const & rhs) : base_class(rhs) {}


    /**
     * @brief Constructs a basic regular expression from the string
     * @p s interpreted according to the flags in @p f.
     *
     * @param __s A string containing a regular expression.
     * @param __f Flags indicating the syntax rules and options.
     *
     * @throws regex_error if @p __s is not a valid regular expression.
     */
	explicit basic_regex (StringType const s, flag_type f = base_class::ECMAScript)
        : base_class(s.data(), s.data() + s.size(), f)
	{}

    /**
     * @brief Constructs a basic regular expression from the range
     * [first, last) interpreted according to the flags in @p f.
     *
     * @param first The start of a range containing a valid regular
     *              expression.
     * @param last  The end of a range containing a valid regular
     *              expression.
     * @param f     The format flags of the regular expression.
     *
     * @throws regex_error if @p [first, last) is not a valid regular
     *         expression.
     */
	basic_regex (typename StringType::iterator first
            , typename StringType::iterator last
            , flag_type f = base_class::ECMAScript)
        : base_class(typename StringType::native_iterator(first)
            , typename StringType::native_iterator(last)
            , f)
	{ }

	basic_regex (typename StringType::const_iterator first
            , typename StringType::const_iterator last
            , flag_type f = base_class::ECMAScript)
        : base_class(typename StringType::native_const_iterator(first)
            , typename StringType::native_const_iterator(last)
            , f)
	{ }
};

template <typename StringType>
inline bool regex_match (
          typename StringType::const_iterator first
        , typename StringType::const_iterator last
        , match_results<typename StringType::const_pointer> & m
        , basic_regex<StringType> & re
        , regex_constants::match_flag_type flags = regex_constants::match_default)
{
    return ::boost::regex_match(first.base().base(), last.base().base(), m, re, flags);
}

template <typename StringType>
inline bool regex_match (
          typename StringType::const_pointer first
        , typename StringType::const_pointer last
        , match_results<typename StringType::const_pointer> & m
        , basic_regex<StringType> & re
        , regex_constants::match_flag_type flags = regex_constants::match_default)
{
    return ::boost::regex_match(first, last, m, re, flags);
}

template <typename StringType>
inline bool regex_match (StringType const & s
        , match_results<typename StringType::const_pointer> & m
        , basic_regex<StringType> & re
        , regex_constants::match_flag_type flags = regex_constants::match_default)
{
    return regex_match<StringType>(s.cbegin(), s.cend(), m, re, flags);
}

template <typename StringType>
inline bool regex_match (typename StringType::const_pointer str
        , match_results<typename StringType::const_pointer> & m
        , basic_regex<StringType> & re
        , regex_constants::match_flag_type flags = regex_constants::match_default)
{
    return ::boost::regex_match(str, m, re, flags);
}

} // namespace pfs

#endif /* __PFS_CXX_CXX98_REGEX_BOOST_HPP__ */

