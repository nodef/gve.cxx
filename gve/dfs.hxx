// Copyright (C) 2025 Subhajit Sahu
// SPDX-License-Identifier: AGPL-3.0-or-later
// See LICENSE for full terms
#pragma once

#include <vector>
#include "_main.hxx"




// An internal namespace helps to hide implementation details.
// This is particularly useful for pre-C++20 modules.
namespace gve {
namespace detail {
using std::vector;




#pragma region METHODS
/**
 * Find vertices visited with DFS.
 * @param vis vertex visited flags (updated)
 * @param x original graph
 * @param u start vertex
 * @param ft should vertex be visited? (vertex)
 * @param fp action to perform on every visited vertex (vertex)
 */
template <class B, class G, class K, class FT, class FP>
inline void dfsVisitedForEachU(vector<B>& vis, const G& x, K u, FT ft, FP fp) {
  if (vis[u] || !ft(u)) return;
  vis[u] = B(1); fp(u);
  x.forEachEdgeKey(u, [&](K v) {
    dfsVisitedForEachU(vis, x, v, ft, fp);
  });
}


/**
 * Find vertices visited with DFS.
 * @tparam FLAG visited flag type
 * @param x original graph
 * @param u start vertex
 * @param ft should vertex be visited? (vertex)
 * @param fp action to perform on every visited vertex (vertex)
 * @returns vertex visited flags
 */
template <class FLAG=bool, class G, class K, class FT, class FP>
inline vector<FLAG> dfsVisitedForEach(const G& x, K u, FT ft, FP fp) {
  vector<FLAG> vis(x.span());
  dfsVisitedForEachU(vis, x, u, ft, fp);
  return vis;
}
#pragma endregion
} // namespace detail
} // namespace gve




// Now, we export the public API.
EXPORT namespace gve {
  // Methods
  using detail::dfsVisitedForEachU;
  using detail::dfsVisitedForEach;
} // namespace gve
