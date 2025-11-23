#pragma once
#include <utility>
#include <type_traits>
#include <iterator>
#include <array>
#include <vector>
#include <ostream>
#include <iostream>
#include <chrono>
#include <ctime>
#include "_compile.hxx"
#include "_debug.hxx"




// An internal namespace helps to hide implementation details.
// This is particularly useful for pre-C++20 modules.
namespace gve {
namespace detail {
#pragma region WRITE
/**
 * Write values to a stream.
 * @param a the stream
 * @param ib begin iterator of values
 * @param ie end iterator of values
 */
template <class I>
inline void write_values(std::ostream& a, I ib, I ie) {
  using T = typename std::iterator_traits<I>::value_type;
  if (std::is_fundamental<T>::value) {
    a << "{";
    for (; ib < ie; ++ib)
      a << " " << *ib;
    a << " }";
  }
  else {
    a << "{\n";
    for (; ib < ie; ++ib)
      a << "  " << *ib << "\n";
    a << "}";
  }
}


/**
 * Write values to a stream.
 * @param a the stream
 * @param x the container of values
 */
template <class J>
inline void writeValues(std::ostream& a, const J& x) {
  write_values(a, x.begin(), x.end());
}


/**
 * Write a pair to a stream.
 * @param a the stream
 * @param x the pair
 */
template <class K, class V>
inline void write(std::ostream& a, const std::pair<K, V>& x) {
  a << x.first << ": " << x.second;
}


/**
 * Write an array to a stream.
 * @param a the stream
 * @param x the array
 */
template <class T, size_t N>
inline void write(std::ostream& a, const std::array<T, N>& x) {
  writeValues(a, x);
}


/**
 * Write a vector to a stream.
 * @param a the stream
 * @param x the vector
 */
template <class T>
inline void write(std::ostream& a, const std::vector<T>& x) {
  writeValues(a, x);
}


/**
 * Write a pair to a stream.
 * @param a the stream
 * @param x the pair
 */
template <class K, class V>
inline std::ostream& operator<<(std::ostream& a, const std::pair<K, V>& x) {
  write(a, x); return a;
}


/**
 * Write an array to a stream.
 * @param a the stream
 * @param x the array
 */
template <class T, size_t N>
inline std::ostream& operator<<(std::ostream& a, const std::array<T, N>& x) {
  write(a, x); return a;
}


/**
 * Write a vector to a stream.
 * @param a the stream
 * @param x the vector
 * @returns the stream
 */
template <class T>
inline std::ostream& operator<<(std::ostream& a, const std::vector<T>& x) {
  write(a, x); return a;
}
#pragma endregion




#pragma region WRITE TIME
/**
 * Write a time to a stream.
 * @param a the stream
 * @param x the time
 */
inline void writeTime(std::ostream& a, const std::time_t& x) {
  const int BUF = 64;
  char  buf[BUF];
  std::tm t;
  std::time_t s = std::chrono::system_clock::to_time_t(
    std::chrono::system_clock::now()
  );
  localtime_safe(&s, &t);
  sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d"
    , t.tm_year + 1900
    , t.tm_mon  + 1
    , t.tm_mday
    , t.tm_hour
    , t.tm_min
    , t.tm_sec
  );
  a << buf;
}


/**
 * Write a time point to a stream.
 * @param a the stream
 * @param x the time point
 */
inline void writeTimePoint(std::ostream& a, const std::chrono::time_point<std::chrono::system_clock>& x) {
  writeTime(a, std::chrono::system_clock::to_time_t(x));
}


/**
 * Write a time to a stream.
 * @param a the stream
 * @param x the time
 * @returns the stream
 */
inline std::ostream& operator<<(std::ostream& a, const std::time_t& x) {
  writeTime(a, x); return a;
}


/**
 * Write a time point to a stream.
 * @param a the stream
 * @param x the time point
 * @returns the stream
 */
inline std::ostream& operator<<(std::ostream& a, const std::chrono::time_point<std::chrono::system_clock>& x) {
  writeTimePoint(a, x); return a;
}
#pragma endregion




#pragma region PRINT
/**
 * Print an object.
 * @param x the object to print
 */
template <class T>
inline void print(const T& x) {
  std::cout << x;
}
#pragma endregion
} // namespace detail
} // namespace gve




// Now, we export the public API.
EXPORT namespace gve {
  // Methods
  using detail::write_values;
  using detail::write;
  using detail::writeTime;
  using detail::writeTimePoint;
  using detail::print;
  using detail::operator<<;
} // namespace gve
