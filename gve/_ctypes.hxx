// Copyright (C) 2025 Subhajit Sahu
// SPDX-License-Identifier: AGPL-3.0-or-later
// See LICENSE for full terms
#pragma once

#include <type_traits>
#include <istream>
#include <ostream>
// #include <cstdint>
#include "_compile.hxx"
#include "_debug.hxx"




// An internal namespace helps to hide implementation details.
// This is particularly useful for pre-C++20 modules.
namespace gve {
namespace detail {
#pragma region TYPES
/** Signed size type. */
using ssize_t = std::make_signed_t<size_t>;
#pragma endregion




#pragma region CLASSES
#ifndef GVE_NONE
/**
 * Zero size type.
 */
struct None {
  #pragma region METHODS
  #pragma region COMPARISION OPERATORS
  friend inline bool operator==(None l, None r)     noexcept { return true; }
  template <class T>
  friend inline bool operator==(None l, const T& r) noexcept { return false; }
  template <class T>
  friend inline bool operator==(const T& l, None r) noexcept { return false; }
  #pragma endregion


  #pragma region AUTOMATIC CONVERSIONS TO INTEGRAL AND FLOATING TYPES
  template <class T, typename = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>>
  operator T() const noexcept { return T(1); }
  #pragma endregion


  #pragma region STREAM OPERATORS
  friend inline std::istream& operator>>(std::istream& a, None& x) noexcept { return a; }
  friend inline std::ostream& operator<<(std::ostream& a, None x)  noexcept { return a; }
  #pragma endregion
  #pragma endregion


  #pragma region CONSTRUCTORS
  /**
   * Construct a zero size object.
   */
  explicit None() {}

  /**
   * Construct a zero size object.
   * @param _ any value (ignored)
   */
  template <class T>
  explicit None(T _) {}
  #pragma endregion
};
#define GVE_NONE gve::detail::None
#endif




#ifndef GVE_SHOW_TYPE
/**
 * Show type of template parameter at compile time.
 * @tparam T type to show
 */
template <class T>
struct ShowType;

/**
 * Show type of template parameter at compile time.
 * @param T type to show
 */
#define GVE_SHOW_TYPE(T) gve::detail::ShowType<T>{}

/**
 * Show type of template parameter at compile time, only if build mode is error or higher.
 * @param T type to show
 */
#define GVE_SHOW_TYPEE(T)  GVE_PERFORME(GVE_SHOW_TYPE(T))

/**
 * Show type of template parameter at compile time, only if build mode is warning or higher.
 * @param T type to show
 */
#define GVE_SHOW_TYPEW(T)  GVE_PERFORMW(GVE_SHOW_TYPE(T))

/**
 * Show type of template parameter at compile time, only if build mode is info or higher.
 * @param T type to show
 */
#define GVE_SHOW_TYPEI(T)  GVE_PERFORMI(GVE_SHOW_TYPE(T))

/**
 * Show type of template parameter at compile time, only if build mode is debug or higher.
 * @param T type to show
 */
#define GVE_SHOW_TYPED(T)  GVE_PERFORMD(GVE_SHOW_TYPE(T))

/**
 * Show type of template parameter at compile time, only if build mode is trace or higher.
 * @param T type to show
 */
#define GVE_SHOW_TYPET(T)  GVE_PERFORMT(GVE_SHOW_TYPE(T))
#endif
#pragma endregion
// - https://stackoverflow.com/a/39637476/1413259




#pragma region TEMPLATE TYPES
// For template classes in template :).
#ifndef tclass0
/** Template class with no arguments. */
#define tclass0  template <> class
/** Template class with one argument. */
#define tclass1  template <class> class
/** Template class with two arguments. */
#define tclass2  template <class, class> class
/** Template class with three arguments. */
#define tclass3  template <class, class, class> class
/** Template class with zero or more arguments. */
#define tclass0s  template <class...> class
/** Template class with one or more arguments. */
#define tclass1s  template <class, class...> class
/** Template class with two or more arguments. */
#define tclass2s  template <class, class, class...> class
/** Template class with three or more arguments. */
#define tclass3s  template <class, class, class, class...> class
#endif
#pragma endregion
} // namespace detail
} // namespace gve




// Now, we export the public API.
EXPORT namespace gve {
  // Types
  using detail::ssize_t;
  // using detail::ShowType;
  // Classes
  using detail::None;
} // namespace gve
