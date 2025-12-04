// Copyright (C) 2025 Subhajit Sahu
// SPDX-License-Identifier: AGPL-3.0-or-later
// See LICENSE for full terms
#pragma once

#include <utility>
#include <chrono>
#include "_compile.hxx"
#if defined(MPI_VERSION) || defined(USE_MPI) || defined(MPI_ENABLED)
#include "_mpi.hxx"
#endif




// An internal namespace helps to hide implementation details.
// This is particularly useful for pre-C++20 modules.
namespace gve {
namespace detail {
#pragma region PAIR ACCESSORS
/**
 * Accessor for the first element (key) of a pair.
 * @tparam K key type
 * @tparam V value type
 */
template <class K, class V>
struct PairFirst  { inline K& operator()(std::pair<K, V>& x) noexcept { return x.first; } };

/**
 * Accessor for the second element (value) of a pair.
 * @tparam K key type
 * @tparam V value type
 */
template <class K, class V>
struct PairSecond { inline V& operator()(std::pair<K, V>& x) noexcept { return x.second; } };

/**
 * Read-only accessor for the first element (key) of a pair.
 * @tparam K key type
 * @tparam V value type
 */
template <class K, class V>
struct ConstPairFirst  { inline const K& operator()(const std::pair<K, V>& x) noexcept { return x.first; } };

/**
 * Read-only accessor for the second element (value) of a pair.
 * @tparam K key type
 * @tparam V value type
 */
template <class K, class V>
struct ConstPairSecond { inline const V& operator()(const std::pair<K, V>& x) noexcept { return x.second; } };

/**
 * Value accessor for the first element (key) of a pair.
 * @tparam K key type
 * @tparam V value type
 */
template <class K, class V>
struct PairFirstValue  { inline K operator()(const std::pair<K, V>& x) noexcept { return x.first; } };

/**
 * Value accessor for the second element (value) of a pair.
 * @tparam K key type
 * @tparam V value type
 */
template <class K, class V>
struct PairSecondValue { inline V operator()(const std::pair<K, V>& x) noexcept { return x.second; } };
#pragma endregion




#pragma region MEASURE DURATION
/**
 * Get current time.
 * @returns high resolution clock time
 */
inline auto timeNow() {
  return std::chrono::high_resolution_clock::now();
}


/**
 * Get time duration.
 * @param start start time
 * @param stop stop time
 * @returns time duration in milliseconds
 */
template <class T>
inline float duration(const T& start, const T& stop) {
  auto a = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  return a.count()/1000.0f;
}


/**
 * Get time duration to now.
 * @param start start time
 * @returns time duration in milliseconds
 */
template <class T>
inline float duration(const T& start) {
  auto stop = timeNow();
  return duration(start, stop);
}




/**
 * Measure the duration of execution of a function.
 * @param fn measured function
 * @param N number of times to repeat the measurement
 * @returns average duration in milliseconds
 */
template <class F>
inline float measureDuration(F fn, int N=1) {
  auto start = timeNow();
  for (int i=0; i<N; ++i)
    fn();
  auto stop  = timeNow();
  return duration(start, stop)/N;
}


#if defined(MPI_VERSION) || defined(USE_MPI) || defined(MPI_ENABLED)
/**
 * Measure the duration of execution of a function.
 * @param fn measured function
 * @param N number of times to repeat the measurement
 * @returns average duration in milliseconds
 */
template <class F>
inline float measureDurationMpi(F fn, int N=1) {
  double total = 0;
  for (int i=0; i<N; ++i) {
    // Match up with other processes before start.
    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();
    fn();
    // Let all processes complete together.
    MPI_Barrier(MPI_COMM_WORLD);
    double stop  = MPI_Wtime();
    total += stop - start;
  }
  // Report in milliseconds.
  return float(total*1000/N);
}
#endif


/**
 * Measure the duration of execution of marked sections in a function.
 * @param fn measured function (section marker)
 * @param N number of times to repeat the measurement
 * @returns average duration in milliseconds
 */
template <class F>
inline float measureDurationMarked(F fn, int N=1) {
  float total = 0;
  for (int i=0; i<N; ++i)
    fn([&](auto fm) {
      float t = measureDuration(fm);
      total  += t;
      return t;
    });
  return total/N;
}


#if defined(MPI_VERSION) || defined(USE_MPI) || defined(MPI_ENABLED)
/**
 * Measure the duration of execution of marked sections in a function.
 * @param fn measured function (section marker)
 * @param N number of times to repeat the measurement
 * @returns average duration in milliseconds
 */
template <class F>
inline float measureDurationMarkedMpi(F fn, int N=1) {
  float total = 0;
  for (int i=0; i<N; ++i)
    fn([&](auto fm) {
      float t = measureDurationMpi(fm);
      total  += t;
      return t;
    });
  return total/N;
}
#endif
#pragma endregion




#pragma region RETRY
/**
 * Call a function, retrying if it fails.
 * @param fn called function (returns true if successful)
 * @param N number of tries
 * @returns true if successful
 */
template <class F>
inline bool retry(F fn, int N=2) {
  for (int i=0; i<N; ++i)
    if (fn()) return true;
  return false;
}
#pragma endregion




#pragma region SUBSCRIPT
#ifndef GVE_SUBSCRIPT2D
/**
 * Access value at given x, i index of 2D array.
 * @param a 2D array
 * @param x x index
 * @param y y index
 * @param X size of x dimension
 */
#define GVE_SUBSCRIPT2D(a, x, y, X)  (a)[x + (X)*(y)]
#endif


#ifndef GVE_SUBSCRIPT3D
/**
 * Access value at given x, y, z index of 3D array.
 * @param a 3D array
 * @param x x index
 * @param y y index
 * @param z z index
 * @param X size of x dimension
 * @param Y size of y dimension
 */
#define GVE_SUBSCRIPT3D(a, x, y, z, X, Y)  (a)[x + (X)*(y + (Y)*(z))]
#endif


#ifndef GVE_SUBSCRIPTRC
/**
 * Access value at given row, column of 2D array.
 * @param a 2D array
 * @param r row index
 * @param c column index
 * @param C number of columns in 2D array
 */
#define GVE_SUBSCRIPTRC(a, r, c, C)  (a)[(C)*(r) + (c)]
#endif
#pragma endregion




#pragma region MOVE
/**
 * Conditional move.
 * @param c condition
 * @param t true value
 * @param f false value
 * @returns move(t) if c, otherwise f
 */
#define GVE_CMOVE(c, t, f) \
  ((c)? move(t) : (f))


/**
 * Conditional move vector.
 * @param t true vector
 * @param f false vector
 * @returns move(t) if t is not empty, otherwise f
 */
#define GVE_CMOVE_VECTOR(t, f) \
  GVE_CMOVE(!(t).empty(), t, f)


/**
 * Conditional move graph.
 * @param t true graph
 * @param f false graph
 * @returns move(t) if t is not empty, otherwise f
 */
#define GVE_CMOVE_GRAPH(t, f) \
  GVE_CMOVE((t).order()>0, t, f)
#pragma endregion
} // namespace detail
} // namespace gve




// Now, we export the public API.
EXPORT namespace gve {
  // Types
  using detail::PairFirst;
  using detail::PairSecond;
  using detail::ConstPairFirst;
  using detail::ConstPairSecond;
  using detail::PairFirstValue;
  using detail::PairSecondValue;
  // Methods
  using detail::timeNow;
  using detail::duration;
  using detail::measureDuration;
  using detail::measureDurationMarked;
  #if defined(MPI_VERSION) || defined(USE_MPI) || defined(MPI_ENABLED)
  using detail::measureDurationMpi;
  using detail::measureDurationMarkedMpi;
  #endif
  using detail::retry;
} // namespace gve
