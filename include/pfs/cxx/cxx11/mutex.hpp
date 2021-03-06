#pragma once
#include <mutex>

namespace pfs {

//typedef native_handle_type decltype(std::mutex::native_handle);

using mutex = std::mutex;

template <typename Mutex>
using lock_guard = std::lock_guard<Mutex>;

template <typename Mutex>
using unique_lock = std::unique_lock<Mutex>;

#define DEFER_LOCK  std::defer_lock_t{}
#define TRY_TO_LOCK std::try_to_lock_t{}
#define ADOPT_LOCK  std::adopt_lock_t{}

} // pfs
