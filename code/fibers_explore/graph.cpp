#include <iostream>
#include <vector>

#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/compressed_sparse_row_graph.hpp>
#include <boost/graph/graph_traits.hpp>

#include <boost/fiber/all.hpp>
#include <boost/range/algorithm.hpp>

using namespace boost;

using graph_t = compressed_sparse_row_graph<directedS>;
using vertex_t = graph_traits<graph_t>::vertex_descriptor;

// visitor class for bfs algorithm
struct discover_visitor : default_bfs_visitor {
    discover_visitor(boost::fibers::unbuffered_channel<vertex_t>& ch)
        : ch{ch} {}

    // will be called always at vertex discovery
    void discover_vertex(const vertex_t vertex, const graph_t&) {
        ch.push(vertex);
    }

    // channel to push vertices out the algorithm
    boost::fibers::unbuffered_channel<vertex_t>& ch;
};

int main() {
    // construct example graph
    /*
     * 0 -> 1 -> 2
     * ^    |    |
     * |    v    v
     * 5 <- 4 <- 3
     */
    auto n_vertices = 6u;
    std::vector<vertex_t> sources{0, 1, 1, 2, 3, 4, 5};
    std::vector<vertex_t> targets{1, 2, 4, 3, 4, 5, 0};

    auto tag = construct_inplace_from_sources_and_targets;
    graph_t graph{tag, sources, targets, n_vertices};

    vertex_t source{0};

    // create channel for pulling vertices from algorithm
    boost::fibers::unbuffered_channel<vertex_t> ch;

    // run fiber with bfs inside, passing the visitor to it, which will send
    // vertices through the channel
    boost::fibers::fiber{[&ch, graph, source]() {
        breadth_first_search(graph, source, visitor(discover_visitor{ch}));
        // at the end close the channel to signal client code it's the end
        ch.close();
    }}.detach();

    // lazily pull all vertices from channel
    for (auto vertex : ch) {
        std::cout << vertex << std::endl;
        std::cout << "*" << std::endl;
    }
}
