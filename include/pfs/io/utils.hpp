
/* 
 * File:   utils.hpp
 * Author: wladt
 *
 * Created on October 9, 2017, 2:08 PM
 */

#ifndef __PFS_IO_UTILS_HPP__
#define __PFS_IO_UTILS_HPP__

#include <pfs/unicode/u8_iterator.hpp>
#include <pfs/io/device.hpp>
#include <pfs/io/iterator.hpp>

namespace pfs {

template <typename StringType>
inline StringType read_all_u8 (io::device & dev)
{
    io::input_iterator<char> first(dev);
    io::input_iterator<char> last;
    
    return read_all_u8<StringType>(first, last);
}

} // pfs

#endif /* __PFS_IO_UTILS_HPP__ */

