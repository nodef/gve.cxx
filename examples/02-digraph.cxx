#ifndef _OPENMP
#define _OPENMP
#endif
#include <iostream>
#include <gve.hxx>

using namespace std;


int main() {
  // Create a directed graph with 5 vertices
  cout << "Creating a directed graph..." << endl;
  gve::DiGraph<int> graph;

  // Add edges to the graph
  cout << "Adding edges..." << endl;
  graph.addEdge(1, 2);
  graph.addEdge(1, 4);
  graph.addEdge(1, 5);
  graph.addEdge(2, 3);
  graph.addEdge(4, 5);
  graph.addEdge(5, 3);

  // Apply the pending updates in parallel
  // Graph properties are updated after this call
  cout << "Applying the updates..." << endl;
  gve::updateOmpU(graph);

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

  // Remove a few edges
  cout << "Removing edges (1->4) and (1->5)..." << endl;
  graph.removeEdge(1, 4);
  graph.removeEdge(1, 5);

  // Apply the pending updates
  cout << "Applying the updates..." << endl;
  gve::updateOmpU(graph);

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
  return 0;
}
