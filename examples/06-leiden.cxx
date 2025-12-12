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
  gve::DiGraph<uint32_t, GVE_NONE, float> graph;
  gve::MappedFile file(filename);
  string_view data((const char*) file.data(), file.size());
  gve::readMtxFormatToGraphOmpW(graph, data);

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

  // Find communities using the Leiden algorithm
  cout << "Finding communities using the Leiden algorithm..." << endl;
  auto result = gve::leidenStaticOmp(graph);
  cout << "Found communities in " << result.time << " ms." << endl;
  auto  fc = [&](int u) { return result.membership[u]; };
  double M = gve::edgeWeightOmp(graph);
  cout << "Modularity of the community structure: " << gve::modularityBy(graph, fc, M) << endl;

  // Print the size of each community
  cout << "Community sizes:" << endl;
  auto communitySizes = gve::communitySizeOmp(graph, result.membership);
  for (size_t i=0, I=communitySizes.size(); i<I; ++i) {
    if (communitySizes[i] == 0) continue;
    cout << "Community " << i << ": Size " << communitySizes[i] << endl;
  }

  cout << endl;

  cout << "Vertices in each community:" << endl;
  auto communityVertices = gve::communityVerticesOmp(graph, result.membership);
  for (size_t i=0, I=communityVertices.size(); i<I; ++i) {
    if (communityVertices[i].empty()) continue;
    cout << "Community " << i << ":";
    for (auto u : communityVertices[i])
      cout << " " << u;
    cout << endl;
  }
  return 0;
}
