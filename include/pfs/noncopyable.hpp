#pragma once

namespace pfs {

struct noncopyable
{
    // Avoid CopyConstructable and CopyAssignable
#if __cplusplus >= 201103L
    noncopyable () = default;
    noncopyable (noncopyable const &) = delete;
    noncopyable & operator = (noncopyable const &) = delete;
#else
    noncopyable () {}
private:
    noncopyable (noncopyable const &);
    noncopyable & operator = (noncopyable const &);
#endif
};

} // namespace pfs
