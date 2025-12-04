// Copyright (C) 2025 Subhajit Sahu
// SPDX-License-Identifier: AGPL-3.0-or-later
// See LICENSE for full terms
#pragma once

#include "_main.hxx"
#include "update.hxx"




// An internal namespace helps to hide implementation details.
// This is particularly useful for pre-C++20 modules.
namespace gve {
namespace detail {
#pragma region METHODS
#pragma region COUNT SELF-LOOPS
/**
 * Count the number of self-loops in a graph.
 * @param x input graph
 * @returns number of self-loops
 */
template <class G>
inline size_t countSelfLoops(const G& x) {
  size_t a = 0;
  x.forEachVertexKey([&](auto u) { if (x.hasEdge(u, u)) ++a; });
  return a;
}
#pragma endregion




#pragma region ADD SELF-LOOPS
/**
 * Add self-loops to a graph.
 * @param a graph to add self-loops to (updated)
 * @param w edge weight of self-loops
 * @param ft test function to determine if self-loop should be added (vertex)
 */
template <class G, class E, class FT>
inline void addSelfLoopsU(G& a, E w, FT ft) {
  a.forEachVertexKey([&](auto u) { if (ft(u)) a.addEdge(u, u, w); });
  a.update();
}

/**
 * Add self-loops to a graph.
 * @param a input graph
 * @param w edge weight of self-loops
 * @param ft test function to determine if self-loop should be added (vertex)
 * @returns graph with self-loops added
 */
template <class G, class E, class FT>
inline G addSelfLoops(const G& x, E w, FT ft) {
  G a = x; addSelfLoopsU(a, w, ft);
  return a;
}


#ifdef _OPENMP
/**
 * Add self-loops to a graph in parallel.
 * @param a graph to add self-loops to (updated)
 * @param w edge weight of self-loops
 * @param ft test function to determine if self-loop should be added (vertex)
 */
template <class G, class E, class FT>
inline void addSelfLoopsOmpU(G& a, E w, FT ft) {
  #pragma omp parallel
  {
    a.forEachVertexKey([&](auto u) { if (ft(u)) addEdgeOmpU(a, u, u, w); });
  }
  updateOmpU(a);
}

/**
 * Add self-loops to a graph in parallel.
 * @param a input graph
 * @param w edge weight of self-loops
 * @param ft test function to determine if self-loop should be added (vertex)
 * @returns graph with self-loops added
 */
template <class G, class E, class FT>
inline G addSelfLoopsOmp(const G& x, E w, FT ft) {
  G a = x; addSelfLoopsOmpU(a, w, ft);
  return a;
}
#endif
#pragma endregion
#pragma endregion
} // namespace detail
} // namespace gve




// Now, we export the public API.
EXPORT namespace gve {
  // Methods
  using detail::countSelfLoops;
  using detail::addSelfLoopsU;
  using detail::addSelfLoops;
#ifdef _OPENMP
  using detail::addSelfLoopsOmpU;
  using detail::addSelfLoopsOmp;
#endif
} // namespace gve
