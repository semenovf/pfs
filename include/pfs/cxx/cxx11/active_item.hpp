#pragma once

// This file contains a lightweight implementation of function object
// that used to store callable objects (function object, pointer to function, 
// reference to function, pointer to member function)
// with arguments in active containers (active_queue, active_map)

namespace pfs {
    
// template <typename Return
//     , typename F
//     , typename A1 = void
//     , typename A2 = void
//     , typename A3 = void
//     , typename A4 = void
//     , typename A5 = void
//     , typename A6 = void
//     , typename A7 = void
//     , typename A8 = void
//     , typename A9 = void
//     , typename A10 = void>
// class function_pointer_active_item
// {
//     
// };
template <
      typename A1
    , typename A2
    , typename A3
    , typename A4
    , typename A5
    , typename A6
    , typename A7
    , typename A8>
class parameter_pack
{
    A1 a1; A2 a2; A3 a3; A4 a4; A5 a5; A6 a6; A7 a7; A8 a8;
};

template <typename F>
class active_item
{
    
};

} // namespace pfs
