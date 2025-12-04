// Copyright (C) 2025 Subhajit Sahu
// SPDX-License-Identifier: AGPL-3.0-or-later
// See LICENSE for full terms
#pragma once

#ifdef _OPENMP
#include <omp.h>




// An internal namespace helps to hide implementation details.
// This is particularly useful for pre-C++20 modules.
namespace gve {
namespace detail {
#pragma region CONSTANTS
#ifndef _OPENMP_MAX_THREADS
/** Maximum number of threads. */
#define _OPENMP_MAX_THREADS 128
#endif
#pragma endregion




#pragma region BELONGS
/**
 * Check if work belongs to current thread.
 * @param key work key
 * @param thread current thread
 * @param THREADS available threads
 * @returns true if work belongs to current thread
 */
template <class K>
inline bool belongsOmp(K key, int thread, int THREADS) {
  const K CHUNK_SIZE = 1024;
  K chunk = key / CHUNK_SIZE;
  return chunk % THREADS == K(thread);
}


/**
 * Check if work belongs to current thread.
 * @param key work key
 * @returns true if work belongs to current thread
 */
template <class K>
inline bool belongsOmp(K key) {
  int thread  = omp_get_thread_num();
  int THREADS = omp_get_num_threads();
  return belongsOmp(key, thread, THREADS);
}
#pragma endregion
} // namespace detail
} // namespace gve
#endif
