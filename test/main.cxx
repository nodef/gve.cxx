#define _WIN64  1
#ifndef _AMD64_
#define _AMD64_ 1
#endif
#ifndef _OPENMP
#define _OPENMP 1
#endif
#include <cstdio>
#include <string_view>
#include "../gve.hxx"

using namespace std;


int main(int argc, char** argv) {
  const char *mtxfile = argv[1];
  // Create a directed graph manually.
  printf("Creating a small directed graph x ...\n");
  gve::DiGraph<> x;
  x.addEdge(1, 2);
  gve::updateU(x);
  printf("Graph x has %zu vertices and %zu edges.\n\n", x.order(), x.size());
  // Load a graph from an MTX file (use OpenMP)
  gve::DiGraphCsr<> y;
  gve::MappedFile mf(mtxfile);
  string_view data((const char*) mf.data(), mf.size());
  printf("Loading graph y from %s ...\n", mtxfile);
  gve::readMtxFormatToCsrW(y, data);
  printf("Graph y has %zu vertices and %zu edges.\n\n", y.order(), y.size());
  // Copy graph y to graph z.
  printf("Duplicating graph y to z ...\n");
  gve::ArenaDiGraph<uint32_t, GVE_NONE, double> z;
  gve::duplicateArenaOmpW(z, y);
  printf("Graph z has %zu vertices and %zu edges.\n\n", z.order(), z.size());
  // Run PageRank on graph z.
  auto a0 = gve::pagerankStaticOmp(z, gve::PagerankOptions<float>());
  printf("PageRank completed in %.3f ms with %d iterations.\n", a0.time, a0.iterations);
  // Run Louvain on graph z.
  auto a1 = gve::louvainStaticOmp(z);
  printf("Louvain completed in %.3f ms with %d iterations and %d passes.\n", a1.time, a1.iterations, a1.passes);
  // Run Leiden on graph z.
  auto a2 = gve::leidenStaticOmp(z);
  printf("Leiden completed in %.3f ms with %d iterations and %d passes.\n", a2.time, a2.iterations, a2.passes);
  return 0;
}
