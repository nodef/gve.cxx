#pragma once
#include <exception>
#include "_compile.hxx"




// An internal namespace helps to hide implementation details.
// This is particularly useful for pre-C++20 modules.
namespace gve {
namespace detail {
#pragma region TYPES
/**
 * Error thrown when a string is not in the expected format.
 */
class FormatError : public std::exception {
  #pragma region DATA
  /** Error message. */
  const char *msg;
  /** Pointer to the character where format check fails. */
  const void *it;
  #pragma endregion


  #pragma region CONSTRUCTORS
  public:
  /**
   * Create a format error.
   * @param msg error message
   * @param it iterator to the character where format check fails
   */
  template <class I>
  FormatError(const char *msg, I it) :
  msg(msg), it(&*it) {}

  /**
   * Create a format error, without iterator.
   * @param msg error message
   */
  FormatError(const char *msg) :
  msg(msg), it(nullptr) {}

  /**
   * Create an empty format error.
   */
  FormatError() :
  msg(nullptr), it(nullptr) {}
  #pragma endregion


  #pragma region METHODS
  public:
  inline bool empty() const noexcept {
    return msg == nullptr;
  }

  /**
   * Get error message.
   * @returns error message
   */
  inline const char* what() const noexcept override {
    return msg;
  }

  /**
   * Get pointer to the character where format check fails.
   * @returns pointer to the character
   */
  inline const void* where() const noexcept {
    return it;
  }
  #pragma endregion
};
#pragma endregion
} // namespace detail
} // namespace gve




// Now, we export the public API.
EXPORT namespace gve {
  // Classes
  using detail::FormatError;
} // namespace gve
