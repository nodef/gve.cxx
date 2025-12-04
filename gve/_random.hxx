// Copyright (C) 2025 Subhajit Sahu
// SPDX-License-Identifier: AGPL-3.0-or-later
// See LICENSE for full terms
#pragma once

#include <cstdint>
#include "_compile.hxx"




// An internal namespace helps to hide implementation details.
// This is particularly useful for pre-C++20 modules.
namespace gve {
namespace detail {
#pragma region CLASSES
/**
 * A 32-bit xorshift RNG.
 */
class xorshift32_engine {
  #pragma region DATA
  private:
  /** State of the RNG. */
  uint32_t state;
  #pragma endregion


  #pragma region METHODS
  public:
  /**
   * Generate a random number.
   */
  uint32_t operator()() {
    uint32_t x = state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return state = x;
  }
  #pragma endregion


  #pragma region CONSTRUCTORS
  /**
   * Construct an RNG with a random seed.
   * @param state initial state
   */
  xorshift32_engine(uint32_t state)
  : state(state) {}
  #pragma endregion
};
// - https://stackoverflow.com/a/71523041/1413259
// - https://www.jstatsoft.org/article/download/v008i14/916
#pragma endregion
} // namespace detail
} // namespace gve




// Now, we export the public API.
EXPORT namespace gve {
  // Classes
  using detail::xorshift32_engine;
} // namespace gve
