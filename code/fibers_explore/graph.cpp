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

// klasa wizytora algorytmu bfs
struct discover_visitor : default_bfs_visitor {
    discover_visitor(boost::fibers::unbuffered_channel<vertex_t>& ch)
        : ch{ch} {}

    // metoda wywołana na każde odkrycie węzła grafu
    // wypycha wartość węzła na zewnątrz
    void discover_vertex(const vertex_t vertex, const graph_t&) {
        ch.push(vertex);
    }

    // kanał do wypychania węzłów na zewnątrz
    boost::fibers::unbuffered_channel<vertex_t>& ch;
};

int main() {
    // utworzenie przykładowego grafu
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

    // węzeł początkowy dla algorytmu
    vertex_t source{0};

    // utworzenie kanału, przez który będą wyciągane wartości węzłów
    boost::fibers::unbuffered_channel<vertex_t> ch;

    // uruchomienie algorytmu wewnątrz włókna, podając wizytor do niego
    // wizytor z kanałem
    boost::fibers::fiber{[&ch, graph, source]() {
        breadth_first_search(graph, source, visitor(discover_visitor{ch}));
        // zamknięcie kanału, zasygnalizowanie klientowi, że to koniec węzłów
        ch.close();
    }}.detach();

    // leniwe wyciąganie kolejnych wartości węzłów
    for (auto vertex : ch) {
        std::cout << vertex << std::endl;
        std::cout << "*" << std::endl;
    }
}
