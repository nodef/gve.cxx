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

#if !defined(NDEBUG) && defined(GVE_BUILD) && GVE_BUILD>=1
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


#pragma region GVE_BUILD MODES
#ifndef GVE_BUILD_RELEASE
/** Build has no debug information. */
#define GVE_BUILD_RELEASE  0
/** Build has only error information. */
#define GVE_BUILD_ERROR    1
/** Build has error and warning information. */
#define GVE_BUILD_WARNING  2
/** Build has error, warning and info information. */
#define GVE_BUILD_INFO     3
/** Build has error, warning, info and debug information. */
#define GVE_BUILD_DEBUG    4
/** Build has error, warning, info, debug and trace information. */
#define GVE_BUILD_TRACE    5
#endif
#pragma endregion




#pragma region PERFORM
#ifndef GVE_PEFORME
#if !defined(NDEBUG) && defined(GVE_BUILD) && GVE_BUILD>=GVE_BUILD_ERROR
/** Perform only if build mode is error or higher. */
#define GVE_PERFORME(...)  __VA_ARGS__
#else
/** Perform only if build mode is error or higher. */
#define GVE_PERFORME(...)
#endif

#if !defined(NDEBUG) && defined(GVE_BUILD) && GVE_BUILD>=GVE_BUILD_WARNING
/** Perform only if build mode is warning or higher. */
#define GVE_PERFORMW(...)  __VA_ARGS__
#else
/** Perform only if build mode is warning or higher. */
#define GVE_PERFORMW(...)
#endif

#if !defined(NDEBUG) && defined(GVE_BUILD) && GVE_BUILD>=GVE_BUILD_INFO
/** Perform only if build mode is info or higher. */
#define GVE_PERFORMI(...)  __VA_ARGS__
#else
/** Perform only if build mode is info or higher. */
#define GVE_PERFORMI(...)
#endif

#if !defined(NDEBUG) && defined(GVE_BUILD) && GVE_BUILD>=GVE_BUILD_DEBUG
/** Perform only if build mode is debug or higher. */
#define GVE_PERFORMD(...)  __VA_ARGS__
#else
/** Perform only if build mode is debug or higher. */
#define GVE_PERFORMD(...)
#endif

#if !defined(NDEBUG) && defined(GVE_BUILD) && GVE_BUILD>=GVE_BUILD_TRACE
/** Perform only if build mode is trace. */
#define GVE_PERFORMT(...)  __VA_ARGS__
#else
/** Perform only if build mode is trace. */
#define GVE_PERFORMT(...)
#endif
#endif
#pragma endregion




#pragma region PRINT
#ifndef GVE_FPRINTFE
/** File print using format only if build mode is error or higher. */
#define GVE_FPRINTFE(...)  GVE_PERFORME(std::fprintf(__VA_ARGS__))
/** File print using format only if build mode is warning or higher. */
#define GVE_FPRINTFW(...)  GVE_PERFORMW(std::fprintf(__VA_ARGS__))
/** File print using format only if build mode is info or higher. */
#define GVE_FPRINTFI(...)  GVE_PERFORMI(std::fprintf(__VA_ARGS__))
/** File print using format only if build mode is debug or higher. */
#define GVE_FPRINTFD(...)  GVE_PERFORMD(std::fprintf(__VA_ARGS__))
/** File print using format only if build mode is trace. */
#define GVE_FPRINTFT(...)  GVE_PERFORMT(std::fprintf(__VA_ARGS__))
#endif


#ifndef GVE_PRINTFE
/** Print using format only if build mode is error or higher. */
#define GVE_PRINTFE(...)  GVE_PERFORME(std::printf(__VA_ARGS__))
/** Print using format only if build mode is warning or higher. */
#define GVE_PRINTFW(...)  GVE_PERFORMW(std::printf(__VA_ARGS__))
/** Print using format only if build mode is info or higher. */
#define GVE_PRINTFI(...)  GVE_PERFORMI(std::printf(__VA_ARGS__))
/** Print using format only if build mode is debug or higher. */
#define GVE_PRINTFD(...)  GVE_PERFORMD(std::printf(__VA_ARGS__))
/** Print using format only if build mode is trace. */
#define GVE_PRINTFT(...)  GVE_PERFORMT(std::printf(__VA_ARGS__))
#endif


#ifndef GVE_WRITEE
/** Write only if build mode is error or higher. */
#define GVE_WRITEE(...)  GVE_PERFORME(write(__VA_ARGS__))
/** Write only if build mode is warning or higher. */
#define GVE_WRITEW(...)  GVE_PERFORMW(write(__VA_ARGS__))
/** Write only if build mode is info or higher. */
#define GVE_WRITEI(...)  GVE_PERFORMI(write(__VA_ARGS__))
/** Write only if build mode is debug or higher. */
#define GVE_WRITED(...)  GVE_PERFORMD(write(__VA_ARGS__))
/** Write only if build mode is trace. */
#define GVE_WRITET(...)  GVE_PERFORMT(write(__VA_ARGS__))
#endif


#ifndef GVE_PRINTE
/** Print only if build mode is error or higher. */
#define GVE_PRINTE(...)  GVE_PERFORME(print(__VA_ARGS__))
/** Print only if build mode is warning or higher. */
#define GVE_PRINTW(...)  GVE_PERFORMW(print(__VA_ARGS__))
/** Print only if build mode is info or higher. */
#define GVE_PRINTI(...)  GVE_PERFORMI(print(__VA_ARGS__))
/** Print only if build mode is debug or higher. */
#define GVE_PRINTD(...)  GVE_PERFORMD(print(__VA_ARGS__))
/** Print only if build mode is trace. */
#define GVE_PRINTT(...)  GVE_PERFORMT(print(__VA_ARGS__))
#endif


#ifndef GVE_PRINTLNE
#ifndef println
#define println(...) do { std::printf(__VA_ARGS__); std::printf("\n"); } while(0)
#endif
/** Print line only if build mode is error or higher. */
#define GVE_PRINTLNE(...)  GVE_PERFORME(println(__VA_ARGS__))
/** Print line only if build mode is warning or higher. */
#define GVE_PRINTLNW(...)  GVE_PERFORMW(println(__VA_ARGS__))
/** Print line only if build mode is info or higher. */
#define GVE_PRINTLNI(...)  GVE_PERFORMI(println(__VA_ARGS__))
/** Print line only if build mode is debug or higher. */
#define GVE_PRINTLND(...)  GVE_PERFORMD(println(__VA_ARGS__))
/** Print line only if build mode is trace. */
#define GVE_PRINTLNT(...)  GVE_PERFORMT(println(__VA_ARGS__))
#endif
#pragma endregion




#pragma region GVE_LOG
#ifndef GVE_LOG
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

#if defined(MPI_VERSION) || defined(USE_MPI) || defined(MPI_ENABLED)
/**
 * Print log prefix.
 */
void logPrefixMpi();

/** Log using format. */
#define GVE_LOG(...)  do { logPrefixMpi(); printf(" " __VA_ARGS__); } while (0)
#else
/** Log using format. */
#define GVE_LOG(...)  do { logPrefix(); printf(" " __VA_ARGS__); } while (0)
#endif
#endif

#ifndef GVE_LOGE
/** Log using format only if build mode is error or higher. */
#define GVE_LOGE(...)  GVE_PERFORME(GVE_LOG(__VA_ARGS__))
/** Log using format only if build mode is warning or higher. */
#define GVE_LOGW(...)  GVE_PERFORMW(GVE_LOG(__VA_ARGS__))
/** Log using format only if build mode is info or higher. */
#define GVE_LOGI(...)  GVE_PERFORMI(GVE_LOG(__VA_ARGS__))
/** Log using format only if build mode is debug or higher. */
#define GVE_LOGD(...)  GVE_PERFORMD(GVE_LOG(__VA_ARGS__))
/** Log using format only if build mode is trace. */
#define GVE_LOGT(...)  GVE_PERFORMT(GVE_LOG(__VA_ARGS__))
#endif
#pragma endregion




#pragma region GVE_ASSERT
#ifndef GVE_ASSERT
#if !defined(NDEBUG) && defined(GVE_BUILD) && GVE_BUILD>=GVE_BUILD_ERROR
  /** Assert expression. */
  #define GVE_ASSERT(exp)            assert(exp)
  /** Assert expression with custom message. */
  #define GVE_ASSERT_THAT(exp, msg)  assert((exp) && (msg))
#else
  /** Assert expression. */
  #define GVE_ASSERT(exp)
  /** Assert expression with custom message. */
  #define GVE_ASSERT_THAT(exp, msg)
#endif
#endif
#pragma endregion




#pragma region METHODS
#pragma region ON SIGNAL
/** Stack trace size for SIGSEGV signal handler. */
#ifndef GVE_STACK_TRACE_SIZE
#define GVE_STACK_TRACE_SIZE  32
#endif


/**
 * Handle SIGSEGV signal.
 * @param sig signal number
 */
inline void on_sigsegv(int sig) {
  #if !defined(NDEBUG) && defined(GVE_BUILD) && GVE_BUILD>=1
  #if defined(__unix__) || defined(__APPLE__) || defined(__linux__)
  void *entries[GVE_STACK_TRACE_SIZE];
  size_t n = backtrace(entries, GVE_STACK_TRACE_SIZE);
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
  #if !defined(NDEBUG) && defined(GVE_BUILD) && GVE_BUILD>=1
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
  // using detail::on_sigsegv;
  // using detail::install_sigsegv;
}
