// Copyright (C) 2025 Subhajit Sahu
// SPDX-License-Identifier: AGPL-3.0-or-later
// See LICENSE for full terms
#pragma once

#if defined(MPI_VERSION) || defined(USE_MPI) || defined(MPI_ENABLED)
#include <chrono>
#include <ctime>
#include <cstdio>
// #include <cstdlib>
#include <mpi.h>
#include "_debug.hxx"




#pragma region ERROR
#ifndef GVE_TRY_MPI
namespace gve {
namespace detail {
/**
 * Log error on MPI function call failure.
 * @param err error code
 * @param exp expression string
 * @param func current function name
 * @param line current line number
 * @param file current file name
 */
void tryFailedMpi(int err, const char* exp, const char* func, int line, const char* file) {
  char buf[MPI_MAX_ERROR_STRING]; int BUF;
  MPI_Error_string(err, buf, &BUF);
  std::fprintf(stderr,
    "ERROR: %s\n"
    "  in expression %s\n"
    "  at %s:%d in %s\n",
    buf, exp, func, line, file);
  MPI_Abort(MPI_COMM_WORLD, err);
}
} // namespace detail
} // namespace gve

/**
 * Try to execute an MPI function call.
 * @param exp expression to execute
 */
#define GVE_TRY_MPI(exp)  do { int err = exp; if (err != MPI_SUCCESS) gve::detail::tryFailedMpi(err, #exp, __func__, __LINE__, __FILE__); } while (0)

/**
 * Try to execute an MPI function call only if build mode is error or higher.
 * @param exp expression to execute
 **/
#define GVE_TRY_MPIE(exp)  GVE_PERFORME(GVE_TRY_MPI(exp))

/**
 * Try to execute an MPI function call only if build mode is warning or higher.
 * @param exp expression to execute
 **/
#define GVE_TRY_MPIW(exp)  GVE_PERFORMW(GVE_TRY_MPI(exp))

/**
 * Try to execute an MPI function call only if build mode is info or higher.
 * @param exp expression to execute
 **/
#define GVE_TRY_MPII(exp)  GVE_PERFORMI(GVE_TRY_MPI(exp))

/**
 * Try to execute an MPI function call only if build mode is debug or higher.
 * @param exp expression to execute
 **/
#define GVE_TRY_MPID(exp)  GVE_PERFORMD(GVE_TRY_MPI(exp))

/**
 * Try to execute an MPI function call only if build mode is trace.
 * @param exp expression to execute
 **/
#define GVE_TRY_MPIT(exp)  GVE_PERFORMT(GVE_TRY_MPI(exp))
#endif




#ifndef GVE_ASSERT_MPI
namespace gve {
namespace detail {
/**
 * Log error on assertion failure.
 * @param exp expression string
 * @param func current function name
 * @param line current line number
 * @param file current file name
 */
void assertFailedMpi(const char* exp, const char* func, int line, const char* file) {
  std::fprintf(stderr,
    "ERROR: Assertion failed\n"
    "  in expression %s\n"
    "  at %s:%d in %s\n",
    exp, func, line, file);
  MPI_Abort(MPI_COMM_WORLD, 1);
}
} // namespace detail
} // namespace gve

/**
 * Assert that expression is true.
 * @param exp expression that should be true
 */
#define GVE_ASSERT_MPI(exp)  do { if (!(exp)) gve::detail::assertFailedMpi(#exp, __func__, __LINE__, __FILE__); } while (0)
#endif
#pragma endregion



#pragma region BASIC
namespace gve {
namespace detail {
/**
 * Get the size of the communicator.
 * @param comm communicator
 * @returns the size
 */
inline int mpi_comm_size(MPI_Comm comm=MPI_COMM_WORLD) {
  int size; MPI_Comm_size(comm, &size);
  return size;
}


/**
 * Get the rank of the current process.
 * @param comm communicator
 * @returns the rank
 */
inline int mpi_comm_rank(MPI_Comm comm=MPI_COMM_WORLD) {
  int rank; MPI_Comm_rank(comm, &rank);
  return rank;
}
}
}
#pragma endregion




#pragma region GVE_LOG
#ifndef GVE_LOG_MPI
namespace gve {
namespace detail {
/**
 * Print log prefix.
 */
void logPrefixMpi() {
  std::time_t s = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::tm    *t = std::localtime(&s);
  std::printf("%04d-%02d-%02d %02d:%02d:%02d P%02d:"
    , t->tm_year + 1900
    , t->tm_mon  + 1
    , t->tm_mday
    , t->tm_hour
    , t->tm_min
    , t->tm_sec
    , mpi_comm_rank()
  );
}
} // namespace detail
} // namespace gve

/** Log using format. */
#define GVE_LOG_MPI(...) do { gve::detail::logPrefixMpi(); std::printf(" " __VA_ARGS__); } while (0)
#endif
#pragma endregion
#endif
