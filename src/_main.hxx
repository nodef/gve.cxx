// Copyright (C) 2025 Subhajit Sahu
// SPDX-License-Identifier: AGPL-3.0-or-later
// See LICENSE for full terms
#pragma once

#include "_debug.hxx"
#include "_algorithm.hxx"
#include "_cmath.hxx"
#include "_ctypes.hxx"
#include "_exception.hxx"
#include "_iostream.hxx"
#include "_iterator.hxx"
#include "_string.hxx"
#include "_utility.hxx"
#include "_random.hxx"
#include "_vector.hxx"
#include "_queue.hxx"
#include "_bitset.hxx"
#include "_mman.hxx"
#include "_memory.hxx"
#ifdef _OPENMP
#include "_openmp.hxx"
#endif
#ifdef __CUDACC__
#include "_cuda.hxx"
#endif
#if defined(MPI_VERSION) || defined(USE_MPI) || defined(MPI_ENABLED)
#include "_mpi.hxx"
#endif
