// -*- coding: utf-8 -*-
// TSTnpuzzle.cc
// -----------------------------------------------------------------------------
//
// Started on <dom 27-02-2022 23:42:57.992602429 (1646001777)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Unit tests of the sliding tile npuzzle
//

#include<tuple>
#include<vector>

#include "../fixtures/TSTnpuzzlefixture.h"

using namespace std;

// Check that instances are correctly recognized
TEST_F (NPuzzleFixture, DefaultInstance) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create an instance of the N-Puzzle with N ranging in the interval [3,
        // MAX_VALUES]
        int n = 3 + rand () % (MAX_VALUES-3);
        npuzzle_t instance = randInstance (n*n);

        // and verify its size is correctly stored
        ASSERT_EQ (instance.get_n (), n);

        // verify also the blank is correctly stored
        ASSERT_EQ (instance[instance.get_blank ()], 0);
    }
}

// Check the variant is correctly stored
TEST_F (NPuzzleFixture, Variant) {

    for (auto i = 0 ; i < NB_TESTS/10 ; i++) {

        // create an instance of the N-Puzzle with N ranging in the interval [3,
        // MAX_VALUES/10]
        int n = 3 + rand () % (MAX_VALUES/10-3);
        npuzzle_t instance = randInstance (n*n);

        // initialize the incremental table with the updates of the manhattan
        // distance, since that is the service which sets the current variant
        string variant = rand () % 2 ? "unit" : "heavy-cost";
        npuzzle_t::initops ();
        npuzzle_t::init (variant);

        // and check the variant is correct
        ASSERT_EQ (instance.get_variant (), variant);
    }
}

// check the incremental computation of the manhattan distance is correct for
// the unit variant
TEST_F (NPuzzleFixture, UnitIncrementalManhattan) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // first, generate a random instance of the N-puzzle with N ranging in
        // the interval [3, MAX_VALUES/10]
        int n = 3 + rand () % (MAX_VALUES/10-3);
        npuzzle_t instance = randInstance (n*n);

        // initialize the list of operators and also the incremental table with
        // the updates of the manhattan distance
        npuzzle_t::initops();
        npuzzle_t::init ("unit");

        // now, examine all successors. Note that the goal is fake, but this
        // doesn't matter as it is not used!
        vector<tuple<int, int, npuzzle_t>> successors;
        instance.children (instance.h (instance), instance, successors);
        for (auto isuccessor : successors) {

            // get the information of this successor
            auto [_, hval, istate] = isuccessor;

            // and verify that the heuristic value of this successor is equal to
            // its manhatan distance
            ASSERT_EQ (hval, istate.h (instance));
        }
    }
}

// check the incremental computation of the manhattan distance is correct for
// the heavy-cost variant
TEST_F (NPuzzleFixture, HeavyCostIncrementalManhattan) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // first, generate a random instance of the N-puzzle with N ranging in
        // the interval [3, MAX_VALUES/10]
        int n = 3 + rand () % (MAX_VALUES/10-3);
        npuzzle_t instance = randInstance (n*n);

        // initialize the list of operators and also the incremental table with
        // the updates of the manhattan distance
        npuzzle_t::initops();
        npuzzle_t::init ("heavy-cost");

        // now, examine all successors. Note that the goal is fake, but this
        // doesn't matter as it is not used!
        vector<tuple<int, int, npuzzle_t>> successors;
        instance.children (instance.h (instance), instance, successors);
        for (auto isuccessor : successors) {

            // get the information of this successor
            auto [_, hval, istate] = isuccessor;

            // and verify that the heuristic value of this successor is equal to
            // its manhatan distance
            ASSERT_EQ (hval, istate.h (instance));
        }
    }
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
