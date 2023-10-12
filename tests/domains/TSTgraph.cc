// -*- coding: utf-8 -*-
// TSTgraph.cc
// -----------------------------------------------------------------------------
//
// Started on <jue 12-10-2023 20:26:06.709213043 (1697135166)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Unit tests of the graph_t class
//

#include <tuple>

#include "../TSTdefs.h"
#include "../fixtures/TSTgraphfixture.h"

using namespace std;

// Check that empty graphs are correctly created
TEST_F (GraphFixture, NullGraph) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create an empty graph
        graph_t graph;

        // and verify the size is the chosen one
        ASSERT_EQ (graph.get_nbedges (), 0);
    }
}

// Check that adding edges to a graph correctly updates the number of edges in
// the graph
TEST_F (GraphFixture, AddNbEdges) {

    // create an empty graph
    graph_t graph;

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a couple of random nodes identified by their numbers and also the cost
        vector<int> vertices = randVectorInt (2, MAX_VALUES*MAX_VALUES*MAX_VALUES);
        int cost = rand () % MAX_VALUES;

        // and add this edge
        graph.add_edge (size_t (vertices[0]), size_t (vertices[1]), cost);

        // ensure the number of edges is the correct one
        ASSERT_EQ (graph.get_nbedges (), 1+i);
    }
}

// Check that adding edges to a graph correctly sets them up
TEST_F (GraphFixture, AddEdges) {

    // create an empty graph
    graph_t graph;

    // create a vector of tuples with all edges randomly added to the graph
    vector<tuple<int, int, int>> edges;

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a couple of random nodes identified by their numbers and also the cost
        vector<int> vertices = randVectorInt (2, MAX_VALUES*MAX_VALUES*MAX_VALUES);
        int cost = rand () % MAX_VALUES;

        // add it to the collection of edges randomly generated
        edges.push_back (make_tuple (vertices[0], vertices[1], cost));

        // and add this edge
        graph.add_edge (size_t (vertices[0]), size_t (vertices[1]), cost);
    }

    // next, verify that all edges have been correctly defined
    for (const auto& edge : edges) {

        // get the source and target nodes of this edge
        auto source = get<0> (edge);
        auto target = get<1> (edge);
        auto cost = get<2> (edge);

        // get all edges defined in the graph from the starting vertex
        auto edges = graph.get_edges (source);

        // and now traverse it until the target node is found with precisely the
        // same cost
        bool found = false;
        for (const auto& edge : edges) {
            if ((edge.get_to () == target) && (edge.get_weight () == cost)) {
                found = true;
                break;
            }
        }

        // assert that this edge has been found
        ASSERT_TRUE (found);
    }
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
