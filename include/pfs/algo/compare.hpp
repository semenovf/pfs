/* 
 * File:   compare.hpp
 * Author: wladt
 *
 * Created on February 26, 2017, 11:23 AM
 */

#ifndef __PFS_ALGO_COMPARE_HPP__
#define __PFS_ALGO_COMPARE_HPP__

namespace pfs {

template <typename InputIt1, typename InputIt2>
int compare (
		  InputIt1 begin1
		, InputIt1 end1
		, InputIt2 begin2
		, InputIt2 end2)

{
    InputIt1 it1(begin1);
    InputIt2 it2(begin2);
    
    while (it1 != end1 && it2 != end2) {
        if (*it1 < *it2)
            return -1;
        if (*it2 < *it1)
            return 1;
    }
    
    if (it1 == end1 && it2 != end2)
        return 1;

    if (it1 != end1 && it2 == end2)
        return -1;
    
    return 0;
}

} // pfs

#endif /* __PFS_ALGO_COMPARE_HPP__ */

