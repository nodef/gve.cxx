#ifndef _OPENMP
#define _OPENMP
#endif
#include <random>
#include <iostream>
#include <gve.hxx>

using namespace std;


int main(int argc, char **argv) {
  const char *filename = argv[1];
  // Load a graph from a Matrix Market (MTX) file, in parallel
  cout << "Loading a directed graph from MTX file..." << endl;
  gve::DiGraphCsr<> csr;
  gve::MappedFile file(filename);
  string_view data((const char*) file.data(), file.size());
  gve::readMtxFormatToCsrOmpW(csr, data);

  cout << endl;

  // Print graph properties
  cout << "Graph properties:" << endl;
  cout << "Number of vertices: " << csr.order() << endl;
  cout << "Number of edges:    " << csr.size()  << endl;

  cout << endl;

  // Print the degrees of each vertex
  cout << "Vertex degrees:" << endl;
  csr.forEachVertexKey([&](int u) {
    cout << "Vertex " << u << ": Degree " << csr.degree(u) << endl;
  });

  cout << endl;

  // Duplicate the CSR graph into a modifiable graph
  cout << "Duplicating the graph into a modifiable graph..." << endl;
  gve::DiGraph<> graph;
  gve::duplicateOmpW(graph, csr);

  // Show the graph as well
  cout << "Graph:" << endl;
  gve::writeGraphDetailed(cout, graph);
  cout << endl;

  cout << endl;

  // Generate a random batch of edge updates
  cout << "Generating edge updates [8 insertions, 2 deletions]..." << endl;
  size_t deletionsCount  = 2;
  size_t insertionsCount = 8;
  random_device dev;
  default_random_engine rnd(dev());
  auto deletions  = gve::generateEdgeDeletions (rnd, graph, deletionsCount,  0, graph.span(), false);
  auto insertions = gve::generateEdgeInsertions(rnd, graph, insertionsCount, 0, graph.span(), false, GVE_NONE());
  gve::tidyBatchUpdateU(deletions, insertions, graph);
  for (auto [u, v, w] : deletions)
    cout << "Delete edge: (" << u << ", " << v << ")" << endl;
  for (auto [u, v, w] : insertions)
    cout << "Insert edge: (" << u << ", " << v << ")" << endl;

  cout << endl;

  // Apply the batch update to the graph
  cout << "Applying the batch update to the graph..." << endl;
  gve::applyBatchUpdateOmpU(graph, deletions, insertions);

  cout << endl;

  // Print updated graph properties
  cout << "Updated graph properties:" << endl;
  cout << "Number of vertices: " << graph.order() << endl;
  cout << "Number of edges:    " << graph.size()  << endl;

  cout << endl;

  // Print the updated degrees of each vertex
  cout << "Updated vertex degrees:" << endl;
  graph.forEachVertexKey([&](int u) {
    cout << "Vertex " << u << ": Degree " << graph.degree(u) << endl;
  });

  cout << endl;

  // Show the updated graph as well
  cout << "Updated graph:" << endl;
  gve::writeGraphDetailed(cout, graph);
  cout << endl;
  cout << endl;
  return 0;
}
