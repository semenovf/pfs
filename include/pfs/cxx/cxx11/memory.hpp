/* 
 * File:   memory.hpp
 * Author: wladt
 *
 * Created on April 25, 2017, 8:16 AM
 */

#ifndef __PFS_CXX11_MEMORY_HPP__
#define __PFS_CXX11_MEMORY_HPP__

#include <memory>
#include <pfs/cxxlang.hpp>

namespace pfs {

template <typename T>
using allocator = std::allocator<T>;

template< class T >
inline pfs_constexpr T * addressof (T & arg)
{
    return std::addressof(arg);
}

template<typename T>
using shared_ptr = std::shared_ptr<T>;

template <typename T, typename... Args >
inline shared_ptr<T> make_shared (Args... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <class T, class T1>
inline shared_ptr<T> reinterpret_pointer_cast (const shared_ptr<T1> & r)
{
	return shared_ptr<T>(r, reinterpret_cast<T*>(r.get()));
}

template <typename T, typename Deleter = std::default_delete<T> >
using unique_ptr = std::unique_ptr<T, Deleter>;

template<typename T, typename... Args>
inline std::unique_ptr<T> make_unique (Args &&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

} // pfs

#endif /* __PFS_CXX11_MEMORY_HPP__ */

