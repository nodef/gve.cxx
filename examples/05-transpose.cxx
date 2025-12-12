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

  // Print graph properties
  cout << "Graph properties:" << endl;
  cout << "Number of vertices: " << graph.order() << endl;
  cout << "Number of edges:    " << graph.size()  << endl;

  cout << endl;

  // Show the graph as well
  cout << "Graph edges:" << endl;
  gve::writeGraphDetailed(cout, graph);
  cout << endl;

  cout << endl;

  // Transpose the graph
  cout << "Transposing the graph..." << endl;
  auto grapht = gve::transposeOmp(graph);

  cout << endl;

  // Print transposed graph properties
  cout << "Transposed graph properties:" << endl;
  cout << "Number of vertices: " << grapht.order() << endl;
  cout << "Number of edges:    " << grapht.size()  << endl;

  cout << endl;

  // Show the transposed graph as well
  cout << "Transposed graph edges:" << endl;
  gve::writeGraphDetailed(cout, grapht);
  cout << endl;
  cout << endl;
  return 0;
}
