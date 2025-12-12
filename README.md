A high-performance parallel Graph interface supporting efficient Dynamic batch updates.

Research in graph-structured data has grown rapidly due to graphs' ability to represent complex real-world information and capture intricate relationships, particularly as many real-world graphs evolve dynamically through edge/vertex insertions and deletions. This has spurred interest in programming frameworks for managing, maintaining, and processing such dynamic graphs. In our report, we evaluate the performance of [PetGraph (Rust)], [Stanford Network Analysis Platform (SNAP)], [SuiteSparse:GraphBLAS], [cuGraph], [Aspen], and [our custom implementation] in tasks including loading graphs from disk to memory, cloning loaded graphs, applying in-place edge deletions/insertions, and performing a simple iterative graph traversal algorithm. Our implementation demonstrates significant performance improvements: it outperforms PetGraph, SNAP, SuiteSparse:GraphBLAS, cuGraph, and Aspen by factors of `177x`, `106x`, `76x`, `17x`, and `3.3x` in graph loading; `20x`, `235x`, `0.24x`, `1.3x`, and `0x` in graph cloning; `141x`/`45x`, `44x`/`25x`, `13x`/`11x`, `28x`/`34x`, and `3.5x`/`2.2x` in edge deletions/insertions; and `67x`/`63x`, `86x`/`86x`, `2.5x`/`2.6x`, `0.25x`/`0.24x`, and `1.3x`/`1.3x` in traversal on updated graphs with deletions/insertions.

Below, we plot the runtime (in seconds, logarithmic scale) for loading a graph from file into memory with PetGraph, SNAP, SuiteSparse:GraphBLAS, cuGraph, Aspen, and **Our DiGraph** for each graph in the dataset.

![Image](https://github.com/user-attachments/assets/3ef09c02-3fa1-4558-9cef-45531093a47a)

Next, we plot the runtime (in milliseconds, logarithmic scale) of deleting a batch of `10^−7|𝐸|` to `0.1|𝐸|` randomly generated edges into a graph, **in-place**, in multiples of `10`. Here, we evaluate PetGraph, SNAP, SuiteSparse:GraphBLAS, cuGraph, Aspen, and Our DiGraph on each graph in the dataset. The left subfigure presents overall runtimes using the geometric mean for consistent scaling, while the right subfigure shows runtimes for individual graphs.

![Image](https://github.com/user-attachments/assets/7252a1fc-abb2-4f38-afd5-0479e08c6468)


Below, we plot the runtime of inserting a batch of edges into a graph, **in-place**, using PetGraph, SNAP, SuiteSparse:GraphBLAS, cuGraph, Aspen, and **Our DiGraph**.

![Image](https://github.com/user-attachments/assets/a5edbd2e-012e-4d52-9edc-fc5f83bf2d21)

Finally, we plot the runtime of traversing a graph using a simple iterative algorithm (`42`-step reverse walks from each vertex in a graph) on graphs with edge deletions. We evaluate PetGraph, SNAP, SuiteSparse:GraphBLAS, cuGraph, Aspen, and **Our DiGraph** on each graph in the dataset.

![Image](https://github.com/user-attachments/assets/60ec8b22-e4d0-4ee7-8752-21bda26add01)

Refer to our technical report for more details: \
[Performance Comparison of Graph Representations Which Support Dynamic Graph Updates][report].

<br>
<br>


## Installation

Run:
```bash
$ npm i gve.cxx
```

And then include `gve.hxx` as follows:
```c
// main.c
#include "node_modules/gve.cxx/gve.hxx"

int main() { /* ... */ }
```

And then compile with `clang` or `gcc` as usual.

```bash
$ clang -std=c++17 -target -fopenmp x86_64-pc-windows-msvc main.cxx  # or, use gcc
```

You may also use a simpler approach:

```c
// main.c
#include <gve.hxx>

int main() { /* ... */ }
```

If you add the path to `node_modules/gve.cxx` to your compiler's include paths.

```bash
$ clang -I./node_modules/gve.cxx -std=c++17 -fopenmp -target x86_64-pc-windows-msvc main.cxx
```

<br>


## Example

```cxx
#include <iostream>
#include <gve.hxx>

using namespace std;


int main() {
  // Create a directed graph with 5 vertices
  gve::DiGraph<int> graph;

  // Add edges to the graph
  graph.addEdge(0, 1);
  graph.addEdge(1, 2);
  graph.addEdge(2, 3);
  graph.addEdge(3, 4);
  graph.addEdge(4, 0);

  // Update the graph.
  gve::updateU(graph);

  // Print the number of vertices and edges
  cout << "Number of vertices: " << graph.order() << endl;
  cout << "Number of edges: " << graph.size() << endl;

  return 0;
}
```

<br>


## API Overview

### Core Classes

#### Graphs

*   **`ArenaDiGraph<K, V, E>`**
    Directed graph implementation using arena allocation. Optimized for high-performance scenarios with static or append-only workloads.
    *   `K`: Key type (vertex ID), default `uint32_t`.
    *   `V`: Vertex value type, default `None`.
    *   `E`: Edge value type (weight), default `None`.

*   **`DiGraph<K, V, E>`**
    Standard directed graph implementation using `std::vector`. more flexible than `ArenaDiGraph` but potentially higher memory overhead.

*   **`DiGraphCsr<K, V, E, O>`**
    Read-only directed graph using Compressed Sparse Row (CSR) format. Best for static graph analysis.
    *   `O`: Offset type, default `size_t`.

### Graph Operations

#### Construction & Modification
*   **`addVertex(G& a, K u, V d)`**: Add a vertex `u` with data `d` to graph `a`.
*   **`addEdgeU(G& a, K u, K v, E w)`**: Add an edge `(u, v)` with weight `w`.
*   **`removeEdgeU(G& a, K u, K v)`**: Remove edge `(u, v)`.
*   **`updateU(G& a)`**: Commit changes to the graph structure (required after batch modifications).
*   **`duplicate(const G& x)`**: Create and return a deep copy of graph `x`.
*   **`transpose(const G& x)`**: Return the transpose (reversed edges) of graph `x`.
*   **`symmetrizeU(G& a)`**: specific method to make graph symmetric (undirected) in-place.

#### Partitioning
*   **`partitionById(const G& x, int p, int P)`**: Partition graph vertices based on ID ranges.
*   **`partitionByBfs(const G& x, int p, int P, FC fc, FT ft)`**: Partition using BFS traversal and a cost function.

#### Batch Updates
*   **`generateEdgeDeletions`** / **`generateEdgeInsertions`**: Generate random sets of edge updates for testing.
*   **`tidyBatchUpdateU`**: clean, sort, and deduplicate a batch of edge updates.
*   **`applyBatchUpdateU`**: Apply a batch of edge insertions and deletions to a graph.

#### Input / Output
*   **`readMtxFormatToGraphW`**: Read a graph from a Matrix Market field.
*   **`write(ostream& out, const G& graph, bool detailed)`**: specific method to write graph structure to an output stream.
*   **`readEdgelistFormat*`**: Family of functions to read edge list formats.

### Algorithms

> **Note:** Most algorithms have a parallel version with the `Omp` suffix (e.g., `pagerankStaticOmp`) if OpenMP is enabled.

#### Traversal
*   **`bfsVisitedForEach(const G& x, K u, FT ft, FP fp)`**: Perform BFS starting at `u`.
*   **`dfsVisitedForEach(const G& x, K u, FT ft, FP fp)`**: Perform DFS starting at `u`.

#### PageRank
*   **`PagerankOptions<V>`**: Configuration options (damping factor, tolerance, max iterations).
*   **`PagerankResult<V>`**: Result structure containing ranks and timing statistics.
*   **`pagerankStatic(const G& xt, const PagerankOptions& o)`**: Compute PageRank on a static graph.
*   **`pagerankNaiveDynamic`**: Update PageRank using naive dynamic approach.
*   **`pagerankDynamicTraversal`**: Update PageRank using dynamic traversal.
*   **`pagerankDynamicFrontier`**: Update PageRank using dynamic frontier approach.

#### Community Detection
*   **`LouvainOptions<V>`** / **`LeidenOptions<V>`**: Configuration options.
*   **`louvainStatic(const G& x, const LouvainOptions& o)`**: Detect communities using the Louvain method.
*   **`leidenStatic(const G& x, const LeidenOptions& o)`**: Detect communities using the Leiden method.

### Utilities

*   **`timeNow()`**: Get current time point.
*   **`duration(start, end)`**: Calculate duration in milliseconds.
*   **`retry(Func f, int retries)`**: Helper to retry a function on failure.
*   **`FormatError`**: Exception thrown for data format errors.

### CUDA Utilities
(Available when compiled with CUDA)
*   **`copyValuesCuW`**: Copy memory between host and device.
*   **`fillValueCuW`**: Fill device memory.
*   **`sumValues*`**: Reduction operations.
*   **`liNorm*`**: Norm calculations.


<br>
<br>


## References

- [Algorithm 1037: SuiteSparse:GraphBLAS: Parallel Graph Algorithms in the Language of Sparse Linear Algebra; Timothy A. Davis et al. (2023)](https://dl.acm.org/doi/full/10.1145/3577195)
- [Low-latency graph streaming using compressed purely-functional trees; Laxman Dhulipala et al. (2019)](https://dl.acm.org/doi/abs/10.1145/3314221.3314598)
- [cuGraph C++ primitives: vertex/edge-centric building blocks for parallel graph computing; Seunghwa Kang et al. (2023)](https://ieeexplore.ieee.org/abstract/document/10196665)
- [SNAP: A General-Purpose Network Analysis and Graph-Mining Library; Jure Leskovec et al. (2016)](https://dl.acm.org/doi/abs/10.1145/2898361)
- [The University of Florida Sparse Matrix Collection; Timothy A. Davis et al. (2011)](https://doi.org/10.1145/2049662.2049663)
- [How can I convert a std::string to int?](https://stackoverflow.com/a/7664227/1413259)
- [Fastest way to read numerical values from text file in C++ (double in this case)](https://stackoverflow.com/a/5678975/1413259)
- [What's the difference between istringstream, ostringstream and stringstream? / Why not use stringstream in every case?](https://stackoverflow.com/a/3292168/1413259)
- [c++ stringstream is too slow, how to speed up?](https://stackoverflow.com/a/5830907/1413259)
- [Best Approach to read huge files utilizing multithreading; Stephan van Hulst :: Coderanch](https://coderanch.com/t/699934/java/Approach-read-huge-files-utilizing)
- [How to get current time and date in C++?](https://stackoverflow.com/a/997988/1413259)
- [Signed variant of size_t in standard C++ library](https://stackoverflow.com/q/65496071/1413259)
- [Is 'signed size_t' different from 'ssize_t'?](https://stackoverflow.com/q/20744349/1413259)
- [How to create a temporary directory?](https://stackoverflow.com/a/4632032/1413259)
- [How to amend a commit without changing commit message (reusing the previous one)?](https://stackoverflow.com/a/10365442/1413259)
- [Syntax for a single-line while loop in Bash](https://stackoverflow.com/a/1289029/1413259)
- [How can I save username and password in Git?](https://stackoverflow.com/a/35942890/1413259)
- [How do I tell git to use fewer cores/threads when compressing?](https://superuser.com/a/539478/305990)
- [Containers library :: cppreference](https://en.cppreference.com/w/cpp/container)
- [Date and time utilities :: cppreference](https://en.cppreference.com/w/cpp/chrono)
- [Standard library header &lt;string&gt; :: cppreference](https://en.cppreference.com/w/cpp/header/string)
- [Standard library header &lt;algorithm&gt; :: cppreference](https://en.cppreference.com/w/cpp/header/algorithm)

<br>
<br>


[![ORG](https://img.shields.io/badge/org-nodef-green?logo=Org)](https://nodef.github.io)
![](https://ga-beacon.deno.dev/G-RC63DPBH3P:SH3Eq-NoQ9mwgYeHWxu7cw/github.com/nodef/gve.cxx)

[PetGraph (Rust)]: https://github.com/petgraph/petgraph
[Stanford Network Analysis Platform (SNAP)]: https://github.com/snap-stanford/snap
[SuiteSparse:GraphBLAS]: https://github.com/GraphBLAS/LAGraph
[cuGraph]: https://github.com/rapidsai/cugraph
[Aspen]: https://github.com/ldhulipala/aspen
[our custom implementation]: https://github.com/puzzlef/graph-openmp
[sheets-o1]: https://docs.google.com/spreadsheets/d/102WZCbN0cGFns8VlCoY_b-_dh-5C9JhPKgUg2d32WU0/edit?usp=sharing
[report]: https://arxiv.org/abs/2502.13862
