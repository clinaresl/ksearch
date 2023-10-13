// -*- coding: utf-8 -*-
// TSTroadmap.cc
// -----------------------------------------------------------------------------
//
// Started on <vie 13-10-2023 13:58:16.990640044 (1697198296)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Unit tests of the roadmap_t class
//

#include "../TSTdefs.h"
#include "../fixtures/TSTroadmapfixture.h"

using namespace std;

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

// Check that the static data member with the graph definition is correctly
// created under the unit variant
TEST_F (RoadmapFixture, GraphUnitDefinition) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create a file which defines a graph grid of a randome length between
        // 10 and 50 with unit edge costs
        int length = 10 + (rand () % 40);
        string filename = "testgraph";
        generate_graph (filename, length, true);

        // next, load the graph definition under the unit variant
        roadmap_t::init (filename, "unit");

        // verify the number of edges is correct: there are (length-2)^2 nodes with
        // four edges each; (length-2)*4 nodes with three edges each and, finally, 4
        // nodes with exactly two edges each. This expression simplifies to
        // 4*length*(length-1)
        ASSERT_EQ (roadmap_t::get_graph ().get_nbedges (),
                   4*length*(length-1));

        // verify the contents of the graph are correctly loaded into main
        // memory
        for (auto i = 0 ; i < length*length ; i++) {

            // first, take all the successors of the i-th node
            auto successors = roadmap_t::get_graph ().get_edges (i);

            // verify the number of immediately accessible locations is correct
            auto colid = i%length;
            auto rowid = i/length;

            // central locations
            if ((colid > 0) && (colid < length-1) % (rowid > 0) && (rowid < length-1)) {
                ASSERT_EQ (successors.size (), 4);
            }

            // side locations: upper and lower rows
            if ( ((rowid == 0) || (rowid == length-1)) && (colid > 0) && (colid < length-1) ) {
                ASSERT_EQ (successors.size (), 3);
            }

            // side locations: left and right columns
            if ( ((colid == 0) || (colid == length-1)) && (rowid > 0) && (rowid < length-1) ) {
                ASSERT_EQ (successors.size (), 3);
            }

            // corner locations
            if ( ((colid == 0) && (rowid == 0 || rowid == length-1)) ||
                 ((colid == length-1) && (rowid == 0 || rowid == length-1)) ) {
                ASSERT_EQ (successors.size (), 2);
            }

            // next, verify that all successors are correctly defined, i.e., the
            // successors stored in the graph are precisely the neighoburs
            // defined in a square grid of the randomly chosen length
            auto neighbours = get_neighbours (i, length, true);
            ASSERT_EQ (successors.size (), neighbours.size ());
            for (const auto& successor: successors) {
                ASSERT_TRUE (find (neighbours.begin (), neighbours.end (), make_pair (successor.get_to (), successor.get_weight ())) != neighbours.end ());
            }
        }
    }
}

// Check that the static data member with the graph definition is correctly
// created under the dimacs variant
TEST_F (RoadmapFixture, GrapDimacsDefinition) {

    for (auto i = 0 ; i < NB_TESTS/100 ; i++) {

        // create a file which defines a graph grid of a randome length between
        // 10 and 50 with unit edge costs
        int length = 10 + (rand () % 40);
        string filename = "testgraph";
        generate_graph (filename, length, false);

        // next, load the graph definition under the unit variant
        roadmap_t::init (filename, "dimacs");

        // verify the number of edges is correct: there are (length-2)^2 nodes with
        // four edges each; (length-2)*4 nodes with three edges each and, finally, 4
        // nodes with exactly two edges each. This expression simplifies to
        // 4*length*(length-1)
        ASSERT_EQ (roadmap_t::get_graph ().get_nbedges (),
                   4*length*(length-1));

        // verify the contents of the graph are correctly loaded into main
        // memory
        for (auto i = 0 ; i < length*length ; i++) {

            // first, take all the successors of the i-th node
            auto successors = roadmap_t::get_graph ().get_edges (i);

            // verify the number of immediately accessible locations is correct
            auto colid = i%length;
            auto rowid = i/length;

            // central locations
            if ((colid > 0) && (colid < length-1) % (rowid > 0) && (rowid < length-1)) {
                ASSERT_EQ (successors.size (), 4);
            }

            // side locations: upper and lower rows
            if ( ((rowid == 0) || (rowid == length-1)) && (colid > 0) && (colid < length-1) ) {
                ASSERT_EQ (successors.size (), 3);
            }

            // side locations: left and right columns
            if ( ((colid == 0) || (colid == length-1)) && (rowid > 0) && (rowid < length-1) ) {
                ASSERT_EQ (successors.size (), 3);
            }

            // corner locations
            if ( ((colid == 0) && (rowid == 0 || rowid == length-1)) ||
                 ((colid == length-1) && (rowid == 0 || rowid == length-1)) ) {
                ASSERT_EQ (successors.size (), 2);
            }

            // next, verify that all successors are correctly defined, i.e., the
            // successors stored in the graph are precisely the neighoburs
            // defined in a square grid of the randomly chosen length
            auto neighbours = get_neighbours (i, length, false);
            ASSERT_EQ (successors.size (), neighbours.size ());
            for (const auto& successor: successors) {
                ASSERT_TRUE (find (neighbours.begin (), neighbours.end (), make_pair (successor.get_to (), successor.get_weight ())) != neighbours.end ());
            }
        }
    }
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
