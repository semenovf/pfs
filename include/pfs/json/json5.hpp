#pragma once
#include <pfs/fsm/fsm.hpp>
#include <pfs/lexical_cast.hpp>
#include <pfs/stack.hpp>
#include <pfs/json/constants.hpp>
#include <pfs/unicode/char.hpp>
#include <pfs/unicode/unicode_iterator.hpp>
#include "exception.hpp"

namespace pfs {
namespace json {

// TODO

/*
    Standard JSON5. The JSON5 Data Interchange Format
    URL: https://spec.json5.org

    Other usefull URLs:
    URL: https://github.com/json5/json5
    -----------------------------------------------------------------------
*/

template <typename JsonType>
struct grammar5
{
    typedef typename JsonType::string_type      string_type;
    typedef typename pfs::unicode::unicode_iterator_traits<
            typename string_type::const_iterator>::iterator iterator;
    typedef typename pfs::unicode::unicode_iterator_traits<
            typename string_type::iterator>::output_iterator output_iterator;
    typedef fsm::fsm<iterator>                 fsm_type;
    typedef typename fsm_type::transition_type transition_type;
    typedef typename fsm_type::char_type       value_type;

} // grammar5

}} // pfs::json

