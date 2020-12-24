/*
 * dump_typeinfo.cpp
 */
#include <cstdio>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <type_traits>
#include "naive_spin_mutex.hpp"
#include "ttas_spin_mutex.hpp"
#include "checked_mutex.hpp"
#include "checked_shared_mutex.hpp"
#include "fair_mutex.hpp"
#include "fair_shared_mutex.hpp"
#include "alternate_mutex.hpp"
#include "alternate_shared_mutex.hpp"
// platform native
#if defined(__linux__) || defined(__APPLE__)
#include "posix_native_mutex.hpp"
#define ENABLE_POSIX_NATIVE_MUTEX
#endif
#if defined(_WIN32)
#include "win_native_mutex.hpp"
#define ENABLE_WIN_NATIVE_MUTEX
#endif
#if defined(__APPLE__)
#include "apple_native_mutex.hpp"
#define ENABLE_APPLE_NATIVE_MUTEX
#endif


#ifdef DUMP_SIZEOF
#define DUMP(T) std::printf("%s %zu\n", #T, sizeof(T))
#endif

#ifdef DUMP_STANDARD_LAYOUT
#define DUMP(T) std::printf("%s %d\n", #T, (int)std::is_standard_layout<T>::value)
#endif


int main()
{
  DUMP(int);
  DUMP(std::size_t);
  DUMP(std::thread::id);
  DUMP(std::condition_variable);

  DUMP(std::mutex);
  DUMP(std::timed_mutex);
  DUMP(std::recursive_mutex);
  DUMP(std::recursive_timed_mutex);

  DUMP(yamc::spin::mutex);
  DUMP(yamc::spin_weak::mutex);
  DUMP(yamc::spin_ttas::mutex);

  DUMP(yamc::checked::mutex);
  DUMP(yamc::checked::timed_mutex);
  DUMP(yamc::checked::recursive_mutex);
  DUMP(yamc::checked::recursive_timed_mutex);
  DUMP(yamc::checked::shared_mutex);
  DUMP(yamc::checked::shared_timed_mutex);

  DUMP(yamc::fair::mutex);
  DUMP(yamc::fair::recursive_mutex);
  DUMP(yamc::fair::timed_mutex);
  DUMP(yamc::fair::recursive_timed_mutex);

  DUMP(yamc::fair::shared_mutex);
  DUMP(yamc::fair::shared_timed_mutex);
  DUMP(yamc::fair::basic_shared_mutex<yamc::rwlock::TaskFairness>);
  DUMP(yamc::fair::basic_shared_mutex<yamc::rwlock::PhaseFairness>);
  DUMP(yamc::fair::basic_shared_timed_mutex<yamc::rwlock::TaskFairness>);
  DUMP(yamc::fair::basic_shared_timed_mutex<yamc::rwlock::PhaseFairness>);

  DUMP(yamc::alternate::mutex);
  DUMP(yamc::alternate::recursive_mutex);
  DUMP(yamc::alternate::timed_mutex);
  DUMP(yamc::alternate::recursive_timed_mutex);

  DUMP(yamc::alternate::shared_mutex);
  DUMP(yamc::alternate::shared_timed_mutex);
  DUMP(yamc::alternate::basic_shared_mutex<yamc::rwlock::ReaderPrefer>);
  DUMP(yamc::alternate::basic_shared_mutex<yamc::rwlock::WriterPrefer>);
  DUMP(yamc::alternate::basic_shared_timed_mutex<yamc::rwlock::ReaderPrefer>);
  DUMP(yamc::alternate::basic_shared_timed_mutex<yamc::rwlock::WriterPrefer>);

#if defined(ENABLE_POSIX_NATIVE_MUTEX)
  DUMP(yamc::posix::native_mutex);
  DUMP(yamc::posix::native_recursive_mutex);
  DUMP(yamc::posix::rwlock);
#if YAMC_POSIX_SPINLOCK_SUPPORTED
  DUMP(yamc::posix::spinlock);
#endif
#endif
#if defined(ENABLE_WIN_NATIVE_MUTEX)
  DUMP(yamc::win::native_mutex);
  DUMP(yamc::win::critical_section);
  DUMP(yamc::win::slim_rwlock);
#endif
#if defined(ENABLE_APPLE_NATIVE_MUTEX)
  DUMP(yamc::apple::unfair_lock);
#endif
  return 0;
}
