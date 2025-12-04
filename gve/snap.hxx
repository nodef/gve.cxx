// Copyright (C) 2025 Subhajit Sahu
// SPDX-License-Identifier: AGPL-3.0-or-later
// See LICENSE for full terms
#pragma once

#include <utility>
#include <string>
#include <istream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include "_main.hxx"
#include "Graph.hxx"
#include "update.hxx"
#ifdef _OPENMP
#include <omp.h>
#endif




// An internal namespace helps to hide implementation details.
// This is particularly useful for pre-C++20 modules.
namespace gve {
namespace detail {
using std::tuple;
using std::string;




#pragma region METHODS
#pragma region READ SNAP TEMPORAL DO
/**
 * Read contents of SNAP Temporal file.
 * @param s input stream
 * @param weighted is it weighted?
 * @param symmetric is it symmetric?
 * @param rows number of rows/vertices
 * @param size number of lines/edges to read
 * @param fb on body line (u, v, w)
 */
template <class FB>
inline void readTemporalDo(std::istream& s, bool weighted, bool symmetric, size_t rows, size_t size, FB fb) {
  if (rows==0 || size==0) return;
  // Process body lines sequentially.
  string line;
  for (; size>0 && std::getline(s, line); --size) {
    size_t u, v; double w = 1;
    std::istringstream sline(line);
    if (!(sline >> u >> v)) break;
    if (weighted) sline >> w;
    fb(u, v, w);
    if (symmetric) fb(v, u, w);
  }
}
template <class FB>
inline void readTemporalDo(const char *pth, bool weighted, bool symmetric, size_t rows, size_t size, FB fb) {
  std::ifstream s(pth);
  readTemporalDo(s, weighted, symmetric, rows, size, fb);
}


#ifdef _OPENMP
/**
 * Read contents of SNAP Temporal file.
 * @param s input stream
 * @param weighted is it weighted?
 * @param symmetric is it symmetric?
 * @param rows number of rows/vertices
 * @param size number of lines/edges to read
 * @param fb on body line (u, v, w)
 */
template <class FB>
inline void readTemporalDoOmp(std::istream& s, bool weighted, bool symmetric, size_t rows, size_t size, FB fb) {
  if (rows==0 || size==0) return;
  // Process body lines in parallel.
  const int THREADS = omp_get_max_threads();
  const int LINES   = 131072;
  vector<string> lines(LINES);
  vector<tuple<size_t, size_t, double>> edges(LINES);
  while (size>0) {
    // Read several lines from the stream.
    int READ = 0;
    for (int i=0; size>0 && i<LINES; ++i, ++READ, --size)
      if (!std::getline(s, lines[i])) break;
    if (READ==0) break;
    // Parse lines using multiple threads.
    #pragma omp parallel for schedule(dynamic, 1024)
    for (int i=0; i<READ; ++i) {
      char *line = (char*) lines[i].c_str();
      size_t u = strtoull(line, &line, 10);
      size_t v = strtoull(line, &line, 10);
      double w = weighted? strtod(line, &line) : 0;
      edges[i] = {u, v, w? w : 1};
    }
    // Notify parsed lines.
    #pragma omp parallel
    {
      for (int i=0; i<READ; ++i) {
        const auto& [u, v, w] = edges[i];
        fb(u, v, w);
        if (symmetric) fb(v, u, w);
      }
    }
  }
}
template <class FB>
inline void readTemporalDoOmp(const char *pth, bool weighted, FB fb) {
  std::ifstream s(pth);
  readTemporalDoOmp(s, weighted, fb);
}
#endif
#pragma endregion




#pragma region READ SNAP TEMPORAL IF
/**
 * Read SNAP Temporal file as graph if test passes.
 * @param a output graph (updated)
 * @param s input stream
 * @param weighted is it weighted?
 * @param symmetric is it symmetric?
 * @param rows number of rows/vertices
 * @param size number of lines/edges to read
 * @param fv include vertex? (u, d)
 * @param fe include edge? (u, v, w)
 */
template <class G, class FV, class FE>
inline void readTemporalIfW(G &a, std::istream& s, bool weighted, bool symmetric, size_t rows, size_t size, FV fv, FE fe) {
  using K = typename G::key_type;
  using V = typename G::vertex_value_type;
  using E = typename G::edge_value_type;
  addVerticesIfU(a, K(1), K(rows+1), V(), fv);
  auto fb = [&](auto u, auto v, auto w) { if (fe(K(u), K(v), K(w))) a.addEdge(K(u), K(v), E(w)); };
  readTemporalDo(s, weighted, symmetric, rows, size, fb);
  a.update();
}
template <class G, class FV, class FE>
inline void readTemporalIfW(G &a, const char *pth, bool weighted, bool symmetric, size_t rows, size_t size, FV fv, FE fe) {
  std::ifstream s(pth);
  readTemporalIfW(a, s, weighted, symmetric, rows, size, fv, fe);
}


#ifdef _OPENMP
/**
 * Read SNAP Temporal file as graph if test passes.
 * @param a output graph (updated)
 * @param s input stream
 * @param weighted is it weighted?
 * @param symmetric is it symmetric?
 * @param rows number of rows/vertices
 * @param size number of lines/edges to read
 * @param fv include vertex? (u, d)
 * @param fe include edge? (u, v, w)
 */
template <class G, class FV, class FE>
inline void readTemporalIfOmpW(G &a, std::istream& s, bool weighted, bool symmetric, size_t rows, size_t size, FV fv, FE fe) {
  using K = typename G::key_type;
  using V = typename G::vertex_value_type;
  using E = typename G::edge_value_type;
  addVerticesIfU(a, K(1), K(rows+1), V(), fv);
  auto fb = [&](auto u, auto v, auto w) { if (fe(K(u), K(v), K(w))) addEdgeOmpU(a, K(u), K(v), E(w)); };
  readTemporalDoOmp(s, weighted, symmetric, rows, size, fb);
  updateOmpU(a);
}
template <class G, class FV, class FE>
inline void readTemporalIfOmpW(G &a, const char *pth, bool weighted, bool symmetric, size_t rows, size_t size, FV fv, FE fe) {
  std::ifstream s(pth);
  readTemporalIfOmpW(a, s, weighted, symmetric, rows, size, fv, fe);
}
#endif
#pragma endregion




#pragma region READ SNAP TEMPORAL
/**
 * Read SNAP Temporal file as graph.
 * @param a output graph (updated)
 * @param s input stream
 * @param weighted is it weighted?
 * @param symmetric is it symmetric?
 * @param rows number of rows/vertices
 * @param size number of lines/edges to read
 */
template <class G>
inline void readTemporalW(G& a, std::istream& s, bool weighted, bool symmetric, size_t rows, size_t size) {
  auto fv = [](auto u, auto d)         { return true; };
  auto fe = [](auto u, auto v, auto w) { return true; };
  readTemporalIfW(a, s, weighted, symmetric, rows, size, fv, fe);
}
template <class G>
inline void readTemporalW(G& a, const char *pth, bool weighted, bool symmetric, size_t rows, size_t size) {
  std::ifstream s(pth);
  readTemporalW(a, s, weighted, symmetric, rows, size);
}


#ifdef _OPENMP
/**
 * Read SNAP Temporal file as graph.
 * @param a output graph (updated)
 * @param s input stream
 * @param weighted is it weighted?
 * @param symmetric is it symmetric?
 * @param rows number of rows/vertices
 * @param size number of lines/edges to read
 */
template <class G>
inline void readTemporalOmpW(G& a, std::istream& s, bool weighted, bool symmetric, size_t rows, size_t size) {
  auto fv = [](auto u, auto d)         { return true; };
  auto fe = [](auto u, auto v, auto w) { return true; };
  readTemporalIfOmpW(a, s, weighted, symmetric, rows, size, fv, fe);
}
template <class G>
inline void readTemporalOmpW(G& a, const char *pth, bool weighted, bool symmetric, size_t rows, size_t size) {
  std::ifstream s(pth);
  readTemporalOmpW(a, s, weighted, symmetric, rows, size);
}
#endif
#pragma endregion
#pragma endregion
} // namespace detail
} // namespace gve




// Now, we export the public API.
EXPORT namespace gve {
  // Methods
  using detail::readTemporalDo;
  using detail::readTemporalIfW;
  using detail::readTemporalW;
#ifdef _OPENMP
  using detail::readTemporalDoOmp;
  using detail::readTemporalIfOmpW;
  using detail::readTemporalOmpW;
#endif
} // namespace gve
