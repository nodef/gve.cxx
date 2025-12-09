// Copyright (C) 2025 Subhajit Sahu
// SPDX-License-Identifier: AGPL-3.0-or-later
// See LICENSE for full terms
#pragma once

#define GVE_BUILD  0  // 5 == GVE_BUILD_TRACE
// #define _OPENMP 1
#include "_main.hxx"
#include "Graph.hxx"
#include "update.hxx"
#include "io.hxx"
#include "csr.hxx"
#include "duplicate.hxx"
#include "transpose.hxx"
#include "symmetrize.hxx"
#include "selfLoop.hxx"
#include "properties.hxx"
#include "dfs.hxx"
#include "bfs.hxx"
#include "partition.hxx"
#include "batch.hxx"
#include "louvain.hxx"
