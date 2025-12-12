#ifndef _OPENMP
#define _OPENMP
#endif
#include <iostream>
#include <gve.hxx>

using namespace std;


int main(int argc, char **argv) {
  const char *filename = argv[1];
  // Load a graph from a Matrix Market (MTX) file, in parallel
  cout << "Loading a directed graph from MTX file..." << endl;
  gve::DiGraphCsr<> graph;
  gve::MappedFile file(filename);
  string_view data((const char*) file.data(), file.size());
  gve::readMtxFormatToCsrOmpW(graph, data);

  cout << endl;

  // Print graph properties
  cout << "Graph properties:" << endl;
  cout << "Number of vertices: " << graph.order() << endl;
  cout << "Number of edges:    " << graph.size()  << endl;

  cout << endl;

  // Print the degrees of each vertex
  cout << "Vertex degrees:" << endl;
  graph.forEachVertexKey([&](int u) {
    cout << "Vertex " << u << ": Degree " << graph.degree(u) << endl;
  });

  cout << endl;

  // Show the graph as well
  cout << "Graph:" << endl;
  gve::writeGraphDetailed(cout, graph);
  cout << endl;
  return 0;
}
