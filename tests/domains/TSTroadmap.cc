// -*- coding: utf-8 -*-
// TSTroadmap.cc
// -----------------------------------------------------------------------------
//
// Started on <vie 13-10-2023 13:58:16.990640044 (1697198296)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

//
// Unit tests of the roadmap_t class
//

#include "../TSTdefs.h"
#include "../fixtures/TSTroadmapfixture.h"

graph_t RoadmapFixture::dgraph = graph_t ();
graph_t RoadmapFixture::tgraph = graph_t ();

// Check that graphs can be created using the default constructor
// ----------------------------------------------------------------------------
TEST_F (RoadmapFixture, DefaultGraph) {

    graph_t graph;

    // and check that it contains no vertices and no edges
    ASSERT_EQ (graph.get_nbvertices (), 0);
    ASSERT_EQ (graph.get_nbedges (), 0);
}

// Check that instances of a roadmap are correctly created
TEST_F (RoadmapFixture, ExplicitRoadmap) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a roadmap with a random index
        size_t index = size_t (rand () % MAX_VALUE);
        roadmap_t roadmap {index};

        // and verify the index is correct
        ASSERT_EQ (roadmap.get_index (), index);
    }
}

// Check that the contents of a full graph with travellend distance are
// correctly retrieved from a binary file
// ----------------------------------------------------------------------------
TEST_F (RoadmapFixture, ReadBinaryDistance) {

    graph_t graph_txt;

    // request reading the entire graph of NY from the files stored in testdata/
    ASSERT_GT(graph_txt.load ("testdata/USA-road-d.NY.gr"), 0);

    // Now, read the contents of the same graph but from the binary file
    // generated in the previous unit test ---order matters!!
    graph_t graph_bin;
    graph_bin.read_binary ("testdata/USA-road-d.NY.gr", false);

    // verify the names of the files used for generating data are correct
    ASSERT_EQ (graph_txt.get_filename (), graph_bin.get_filename ());

    // check the information on all vertices
    ASSERT_EQ (graph_txt.get_nbvertices (), graph_bin.get_nbvertices ());
    for (std::size_t i = 0 ; i < graph_bin.get_nbvertices () ; i++) {

        // check the longitude and latitude are the same numbers retrieved
        // directly from the input file
        ASSERT_EQ (graph_txt.get_vertex (i)._longitude, graph_bin.get_vertex (i)._longitude);
        ASSERT_EQ (graph_txt.get_vertex (i)._latitude, graph_bin.get_vertex (i)._latitude);
    }

    // next, ensure that all edges are correctly retrieved
    ASSERT_EQ (graph_txt.get_nbedges (), graph_bin.get_nbedges ());
    for (std::size_t i = 0 ; i < graph_txt.get_nbvertices () ; i++) {

        // and verify that this vertex contains exactly the same neighbours in
        // both graphs. Importantly, equalEdges is used to compare the children
        // in one graph and the other because the eq operator of edges only uses
        // the final vertex.
        auto tchildren = graph_txt.get_edges (i);
        auto bchildren = graph_bin.get_edges (i);
        ASSERT_EQ (tchildren.size (), bchildren.size ());
        ASSERT_TRUE (equalEdges (tchildren, bchildren));

        // and also the same parents
        auto tparents = graph_txt.get_parents (i);
        auto bparents = graph_bin.get_parents (i);
        ASSERT_EQ (tparents.size (), bparents.size ());
        ASSERT_TRUE (equalEdges (tparents, bparents));

        // Verify also that the information of parents and successors is
        // consistent

        // First, every child of this vertex contains this node as its parent
        for (auto const& ichild: tchildren) {
            auto iparents = graph_bin.get_parents (ichild._to);
            ASSERT_NE (find (iparents.begin (), iparents.end (), edge_t{i, 0}),
                       iparents.end ());
        }

        // Second, every parent of this vertex must contain this node as a
        // successor
        for (auto const& iparent: tparents) {
            auto ichildren = graph_bin.get_edges (iparent._to);
            ASSERT_NE (find (ichildren.begin (), ichildren.end (), edge_t{i, 0}),
                       ichildren.end ());
        }
    }
}

// Check that the contents of a full graph with travellend time are
// correctly retrieved from a binary file
// ----------------------------------------------------------------------------
TEST_F (RoadmapFixture, ReadBinaryTime) {

    graph_t graph_txt;

    // request reading the entire graph of NY from the files stored in testdata/
    ASSERT_GT(graph_txt.load ("testdata/USA-road-t.NY.gr"), 0);

    // Now, read the contents of the same graph but from the binary file
    // generated in the previous unit test ---order matters!!
    graph_t graph_bin;
    graph_bin.read_binary ("testdata/USA-road-t.NY.gr", false);

    // verify the names of the files used for generating data are correct
    ASSERT_EQ (graph_txt.get_filename (), graph_bin.get_filename ());

    // check the information on all vertices
    ASSERT_EQ (graph_txt.get_nbvertices (), graph_bin.get_nbvertices ());
    for (std::size_t i = 0 ; i < graph_bin.get_nbvertices () ; i++) {

        // check the longitude and latitude are the same numbers retrieved
        // directly from the input file
        ASSERT_EQ (graph_txt.get_vertex (i)._longitude, graph_bin.get_vertex (i)._longitude);
        ASSERT_EQ (graph_txt.get_vertex (i)._latitude, graph_bin.get_vertex (i)._latitude);
    }

    // next, ensure that all edges are correctly retrieved
    ASSERT_EQ (graph_txt.get_nbedges (), graph_bin.get_nbedges ());
    for (std::size_t i = 0 ; i < graph_txt.get_nbvertices () ; i++) {

        // and verify that this vertex contains exactly the same neighbours in
        // both graphs. Importantly, equalEdges is used to compare the children
        // in one graph and the other because the eq operator of edges only uses
        // the final vertex.
        auto tchildren = graph_txt.get_edges (i);
        auto bchildren = graph_bin.get_edges (i);
        ASSERT_EQ (tchildren.size (), bchildren.size ());
        ASSERT_TRUE (equalEdges (tchildren, bchildren));

        // and also the same parents
        auto tparents = graph_txt.get_parents (i);
        auto bparents = graph_bin.get_parents (i);
        ASSERT_EQ (tparents.size (), bparents.size ());
        ASSERT_TRUE (equalEdges (tparents, bparents));

        // Verify also that the information of parents and successors is
        // consistent

        // First, every child of this vertex contains this node as its parent
        for (auto const& ichild: tchildren) {
            auto iparents = graph_bin.get_parents (ichild._to);
            ASSERT_NE (find (iparents.begin (), iparents.end (), edge_t{i, 0}),
                       iparents.end ());
        }

        // Second, every parent of this vertex must contain this node as a
        // successor
        for (auto const& iparent: tparents) {
            auto ichildren = graph_bin.get_edges (iparent._to);
            ASSERT_NE (find (ichildren.begin (), ichildren.end (), edge_t{i, 0}),
                       ichildren.end ());
        }
    }
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
