// Copyright (C) 2025 Subhajit Sahu
// SPDX-License-Identifier: AGPL-3.0-or-later
// See LICENSE for full terms
#pragma once

#define __STDC_WANT_LIB_EXT1__ 1
#include <ctime>
#include <cassert>
#include <cstdio>
// #include <cstdlib>
#include <chrono>

#if !defined(NDEBUG) && defined(BUILD) && BUILD>=1
#include <signal.h>
#if defined(__unix__) || defined(__APPLE__) || defined(__linux__)
#include <unistd.h>
#include <execinfo.h>
#endif
#endif

#include "_compile.hxx"




// An internal namespace helps to hide implementation details.
// This is particularly useful for pre-C++20 modules.
namespace gve {
namespace detail {
using std::fprintf;
using std::printf;


#pragma region BUILD MODES
#ifndef BUILD_RELEASE
/** Build has no debug information. */
#define BUILD_RELEASE  0
/** Build has only error information. */
#define BUILD_ERROR    1
/** Build has error and warning information. */
#define BUILD_WARNING  2
/** Build has error, warning and info information. */
#define BUILD_INFO     3
/** Build has error, warning, info and debug information. */
#define BUILD_DEBUG    4
/** Build has error, warning, info, debug and trace information. */
#define BUILD_TRACE    5
#endif
#pragma endregion




#pragma region PERFORM
#ifndef PEFORME
#if !defined(NDEBUG) && defined(BUILD) && BUILD>=BUILD_ERROR
/** Perform only if build mode is error or higher. */
#define PERFORME(...)  __VA_ARGS__
#else
/** Perform only if build mode is error or higher. */
#define PERFORME(...)
#endif

#if !defined(NDEBUG) && defined(BUILD) && BUILD>=BUILD_WARNING
/** Perform only if build mode is warning or higher. */
#define PERFORMW(...)  __VA_ARGS__
#else
/** Perform only if build mode is warning or higher. */
#define PERFORMW(...)
#endif

#if !defined(NDEBUG) && defined(BUILD) && BUILD>=BUILD_INFO
/** Perform only if build mode is info or higher. */
#define PERFORMI(...)  __VA_ARGS__
#else
/** Perform only if build mode is info or higher. */
#define PERFORMI(...)
#endif

#if !defined(NDEBUG) && defined(BUILD) && BUILD>=BUILD_DEBUG
/** Perform only if build mode is debug or higher. */
#define PERFORMD(...)  __VA_ARGS__
#else
/** Perform only if build mode is debug or higher. */
#define PERFORMD(...)
#endif

#if !defined(NDEBUG) && defined(BUILD) && BUILD>=BUILD_TRACE
/** Perform only if build mode is trace. */
#define PERFORMT(...)  __VA_ARGS__
#else
/** Perform only if build mode is trace. */
#define PERFORMT(...)
#endif
#endif
#pragma endregion




#pragma region PRINT
#ifndef FPRINTFE
/** File print using format only if build mode is error or higher. */
#define FPRINTFE(...)  PERFORME(std::fprintf(__VA_ARGS__))
/** File print using format only if build mode is warning or higher. */
#define FPRINTFW(...)  PERFORMW(std::fprintf(__VA_ARGS__))
/** File print using format only if build mode is info or higher. */
#define FPRINTFI(...)  PERFORMI(std::fprintf(__VA_ARGS__))
/** File print using format only if build mode is debug or higher. */
#define FPRINTFD(...)  PERFORMD(std::fprintf(__VA_ARGS__))
/** File print using format only if build mode is trace. */
#define FPRINTFT(...)  PERFORMT(std::fprintf(__VA_ARGS__))
#endif


#ifndef PRINTFE
/** Print using format only if build mode is error or higher. */
#define PRINTFE(...)  PERFORME(std::printf(__VA_ARGS__))
/** Print using format only if build mode is warning or higher. */
#define PRINTFW(...)  PERFORMW(std::printf(__VA_ARGS__))
/** Print using format only if build mode is info or higher. */
#define PRINTFI(...)  PERFORMI(std::printf(__VA_ARGS__))
/** Print using format only if build mode is debug or higher. */
#define PRINTFD(...)  PERFORMD(std::printf(__VA_ARGS__))
/** Print using format only if build mode is trace. */
#define PRINTFT(...)  PERFORMT(std::printf(__VA_ARGS__))
#endif


#ifndef WRITEE
/** Write only if build mode is error or higher. */
#define WRITEE(...)  PERFORME(write(__VA_ARGS__))
/** Write only if build mode is warning or higher. */
#define WRITEW(...)  PERFORMW(write(__VA_ARGS__))
/** Write only if build mode is info or higher. */
#define WRITEI(...)  PERFORMI(write(__VA_ARGS__))
/** Write only if build mode is debug or higher. */
#define WRITED(...)  PERFORMD(write(__VA_ARGS__))
/** Write only if build mode is trace. */
#define WRITET(...)  PERFORMT(write(__VA_ARGS__))
#endif


#ifndef PRINTE
/** Print only if build mode is error or higher. */
#define PRINTE(...)  PERFORME(print(__VA_ARGS__))
/** Print only if build mode is warning or higher. */
#define PRINTW(...)  PERFORMW(print(__VA_ARGS__))
/** Print only if build mode is info or higher. */
#define PRINTI(...)  PERFORMI(print(__VA_ARGS__))
/** Print only if build mode is debug or higher. */
#define PRINTD(...)  PERFORMD(print(__VA_ARGS__))
/** Print only if build mode is trace. */
#define PRINTT(...)  PERFORMT(print(__VA_ARGS__))
#endif


#ifndef PRINTLNE
#define println(...) do { std::printf(__VA_ARGS__); std::printf("\n"); } while(0)
/** Print line only if build mode is error or higher. */
#define PRINTLNE(...)  PERFORME(println(__VA_ARGS__))
/** Print line only if build mode is warning or higher. */
#define PRINTLNW(...)  PERFORMW(println(__VA_ARGS__))
/** Print line only if build mode is info or higher. */
#define PRINTLNI(...)  PERFORMI(println(__VA_ARGS__))
/** Print line only if build mode is debug or higher. */
#define PRINTLND(...)  PERFORMD(println(__VA_ARGS__))
/** Print line only if build mode is trace. */
#define PRINTLNT(...)  PERFORMT(println(__VA_ARGS__))
#endif
#pragma endregion




#pragma region LOG
#ifndef LOG
/**
 * Get current time safely.
 * @param time time
 * @param result time result
 */
inline void localtime_safe(const std::time_t* time, std::tm* result) {
  #if defined(_WIN32)
  localtime_s(result, time);
  #elif defined(__STDC_LIB_EXT1__)
  localtime_s(time, result);
  #else
  localtime_r(time, result);
  #endif
}

/**
 * Print log prefix.
 */
inline void logPrefix() {
  std::tm t;
  std::time_t s = std::chrono::system_clock::to_time_t(
    std::chrono::system_clock::now()
  );
  localtime_safe(&s, &t);
  printf("%04d-%02d-%02d %02d:%02d:%02d"
    , t.tm_year + 1900
    , t.tm_mon  + 1
    , t.tm_mday
    , t.tm_hour
    , t.tm_min
    , t.tm_sec
  );
}

#ifdef MPI
/**
 * Print log prefix.
 */
void logPrefixMpi();

/** Log using format. */
#define LOG(...)  do { logPrefixMpi(); printf(" " __VA_ARGS__); } while (0)
#else
/** Log using format. */
#define LOG(...)  do { logPrefix(); printf(" " __VA_ARGS__); } while (0)
#endif
#endif

#ifndef LOGE
/** Log using format only if build mode is error or higher. */
#define LOGE(...)  PERFORME(LOG(__VA_ARGS__))
/** Log using format only if build mode is warning or higher. */
#define LOGW(...)  PERFORMW(LOG(__VA_ARGS__))
/** Log using format only if build mode is info or higher. */
#define LOGI(...)  PERFORMI(LOG(__VA_ARGS__))
/** Log using format only if build mode is debug or higher. */
#define LOGD(...)  PERFORMD(LOG(__VA_ARGS__))
/** Log using format only if build mode is trace. */
#define LOGT(...)  PERFORMT(LOG(__VA_ARGS__))
#endif
#pragma endregion




#pragma region ASSERT
#ifndef ASSERT
#if !defined(NDEBUG) && defined(BUILD) && BUILD>=BUILD_ERROR
  /** Assert expression. */
  #define ASSERT(exp)            assert(exp)
  /** Assert expression with custom message. */
  #define ASSERT_THAT(exp, msg)  assert((exp) && (msg))
#else
  /** Assert expression. */
  #define ASSERT(exp)
  /** Assert expression with custom message. */
  #define ASSERT_THAT(exp, msg)
#endif
#endif
#pragma endregion




#pragma region METHODS
#pragma region ON SIGNAL
/** Stack trace size for SIGSEGV signal handler. */
#ifndef STACK_TRACE_SIZE
#define STACK_TRACE_SIZE  32
#endif


/**
 * Handle SIGSEGV signal.
 * @param sig signal number
 */
inline void on_sigsegv(int sig) {
  #if !defined(NDEBUG) && defined(BUILD) && BUILD>=1
  #if defined(__unix__) || defined(__APPLE__) || defined(__linux__)
  void *entries[STACK_TRACE_SIZE];
  size_t n = backtrace(entries, STACK_TRACE_SIZE);
  std::fprintf(stderr, "ERROR: SIGNAL %d:\n", sig);
  backtrace_symbols_fd(entries, n, STDERR_FILENO);
  #else
  std::fprintf(stderr, "ERROR: SIGNAL %d\n", sig);
  #endif
  std::exit(1);
  #endif
}
// - https://stackoverflow.com/a/77336/1413259


/**
 * Install SIGSEGV signal handler, if build mode is error or higher.
 */
inline void install_sigsegv() {
  #if !defined(NDEBUG) && defined(BUILD) && BUILD>=1
  signal(SIGSEGV, on_sigsegv);
  #endif
}
// - https://stackoverflow.com/a/77336/1413259
#pragma endregion
#pragma endregion
} // namespace detail
} // namespace gve




// Now, we export the public API.
EXPORT namespace gve {
  // Methods
  using detail::on_sigsegv;
  using detail::install_sigsegv;
}
