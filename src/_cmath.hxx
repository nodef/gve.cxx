// Copyright (C) 2025 Subhajit Sahu
// SPDX-License-Identifier: AGPL-3.0-or-later
// See LICENSE for full terms
#pragma once

#include <type_traits>
#include <cmath>
#include <random>
#include "_compile.hxx"




// An internal namespace helps to hide implementation details.
// This is particularly useful for pre-C++20 modules.
namespace gve {
namespace detail {
#pragma region CEIL DIV
/**
 * Get the ceiling of a division.
 * @param x the dividend
 * @param y the divisor
 * @returns ceil(x/y)
 */
template <class T>
inline T ceilDiv(T x, T y) {
  return (x + y-1) / y;
}

/**
 * Get the ceiling of a division.
 * @param x the dividend
 * @param y the divisor
 * @returns ceil(x/y)
 */
template <>
inline float ceilDiv<float>(float x, float y) {
  return std::ceil(x / y);
}

/**
 * Get the ceiling of a division.
 * @param x the dividend
 * @param y the divisor
 * @returns ceil(x/y)
 */
template <>
inline double ceilDiv<double>(double x, double y) {
  return std::ceil(x / y);
}
#pragma endregion




#pragma region SGN
/**
 * Get the sign of a value.
 * @param x the value
 * @returns -1 if x<0, 0 if x==0, 1 if x>0
 */
template <typename T>
inline int sgn(T x) {
  return (T() < x) - (x < T());
}
// - https://stackoverflow.com/a/4609795/1413259
#pragma endregion




#pragma region BITWISE OPERATIONS
/**
 * Count the number of leading zeros in a value.
 * @param x the value
 * @returns number of leading zeros
 */
template <class T>
inline constexpr int countLeadingZeros(T x) noexcept {
  static_assert(std::is_integral_v<T>, "clz only works with integral types");
  IF_CONSTEXPR (sizeof(T) <= sizeof(unsigned int))       return __builtin_clz(x);
  IF_CONSTEXPR (sizeof(T) <= sizeof(unsigned long))      return __builtin_clzl(x);
  IF_CONSTEXPR (sizeof(T) <= sizeof(unsigned long long)) return __builtin_clzll(x);
  static_assert(sizeof(T) <= sizeof(unsigned long long), "clz only works with types up to 64 bits");
}
#pragma endregion




#pragma region POW2
/**
 * Check if a value is a power of 2.
 * @param x the value
 * @returns true if x is a power of 2
 */
template <class T>
inline constexpr bool isPow2(T x) noexcept {
  return !(x & (x-1));
}


/**
 * Get the previous power of 2 of a value.
 * @param x the value
 * @returns previous power of 2 of x
 */
template <class T>
inline constexpr T prevPow2(T x) noexcept {
  return 1 << T(std::log2(x));
}


/**
 * Get the next power of 2 of a value.
 * @param x the value
 * @returns next power of 2 of x
 */
template <class T>
inline constexpr T nextPow2(T x) noexcept {
  if (std::is_integral_v<T>) return T(1) << (8*sizeof(T) - countLeadingZeros(x));
  return 1 << T(std::ceil(std::log2(x)));
}
#pragma endregion




#pragma region PRIME
/**
 * Examine if a value is prime.
 * @param x the value
 * @returns true if x is prime
 */
template <class T>
inline bool isPrime(T x) {
  // 1. 2, 3 are prime
  if (x<=3) return x>1;
  // 2. Multiples of 2, 3 not prime
  if (x % 2==0 || x % 3==0) return false;
  // 3. Factor of 6k-1 or 6k+1 => not prime
  for (T i=6, I=T(std::sqrt(x))+1; i<=I; i+=6)
    if (x % (i-1)==0 || x % (i+1)==0) return false;
  return true;
}


/**
 * Obtain the next prime of a value.
 * @param x the value
 * @returns next prime of x
 */
template <class T>
inline T nextPrime(T x) {
  while (true)
    if (isPrime(++x)) return x;
}


/**
 * Obtain a random prime number in a range.
 * @param begin beginning of the range
 * @param end end of the range
 * @param rnd random number generator
 * @returns a random prime number in [begin, end] or end+1 if not found
 */
template <class T, class R>
inline T randomPrime(T begin, T end, R& rnd) {
  std::uniform_int_distribution<T> dis(begin, end);
  for (int i=0; i<128; ++i) {
    T a = dis(rnd);
    if (isPrime(a)) return a;
  }
  return end+1;
}
#pragma endregion
} // namespace detail
} // namespace gve




// Now, we export the public API.
EXPORT namespace gve {
  // Methods
  using detail::ceilDiv;
  using detail::sgn;
  using detail::countLeadingZeros;
  using detail::isPow2;
  using detail::prevPow2;
  using detail::nextPow2;
  using detail::isPrime;
  using detail::nextPrime;
  using detail::randomPrime;
}
