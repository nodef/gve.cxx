#define _WIN64 1
#define _AMD64_ 1
#include <cstdio>
#include "gve.hxx"

using namespace std;


int main() {
  gve::DiGraph g;
  g.addEdge(1, 2);
  gve::updateU(g);
  printf("Graph g has %zu vertices and %zu edges.\n", g.order(), g.size());
  return 0;
}
