// -*- coding: utf-8 -*-
// TSTnpancake.cc
// -----------------------------------------------------------------------------
//
// Started on <dom 27-02-2022 23:42:57.992602429 (1646001777)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Unit tests of the N-Pancake
//

#include<tuple>
#include<vector>

#include "../TSTdefs.h"
#include "../fixtures/TSTnpancakefixture.h"

using namespace std;

// Check that instances are correctly recognized
TEST_F (NPancakeFixture, DefaultInstance) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a random instance with at least 10 discs and no more than
        // NB_DISCS + 10
        auto length = 10 + rand () % (NB_DISCS-10);
        npancake_t instance = randInstance (length);

        // and verify the size is the chosen one
        ASSERT_EQ (length, instance.get_n ());
    }
}

// Check that all successors are correctly generated
TEST_F (NPancakeFixture, Successors) {

    for (auto i = 0 ; i < NB_TESTS ; i++ ) {

        // first, generate a random instance
        npancake_t instance = randInstance (10 + rand () % (NB_DISCS-10));

        // initialize the incremental table with the updates of the gap
        // heuristic
        npancake_t::init ("unit");

        // now, expand this node and generate all children. Note that neither h
        // nor the goal are relevant to generate the children and just they take
        // any arbitrary value
        vector<tuple<int, int, npancake_t>> successors;
        instance.children (0, instance, successors);

        // first, verify the number of descendants equals its length minus one
        ASSERT_EQ (successors.size (), instance.get_n ()-1);

        // compute the number of discs flipped in every descendant
        vector<int> flips;
        transform (successors.begin (), successors.end (),
                   back_inserter (flips),
                   [=] (tuple<int, int, npancake_t> successor) {
                       return getPrefix (instance, get<2> (successor));
                   });

        // and next verify that flips are all numbers in the range [2, n) where
        // is n is the size of the instance expanded
        for (auto i = 2 ; i <=instance.get_n () ; i++) {
            ASSERT_EQ (i, flips[i-2]);
        }
    }
}

// check the incremental computation of the gap heuristic is correct for the
// unit variant
TEST_F (NPancakeFixture, UnitIncrementalGap) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // first, generate a random instance
        npancake_t instance = randInstance (10 + rand () % (NB_DISCS-10));

        // initialize the incremental table with the updates of the gap
        // heuristic
        npancake_t::init ("unit");

        // now, examine all successors. Note that the goal is fake, but this
        // doesn't matter as it is not used!
        vector<tuple<int, int, npancake_t>> successors;
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

// check the incremental computation of the gap heuristic is correct for the
// heavy-cost variant
TEST_F (NPancakeFixture, HeavyCostIncrementalGap) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // first, generate a random instance
        npancake_t instance = randInstance (10 + rand () % (NB_DISCS-10));

        // initialize the incremental table with the updates of the gap
        // heuristic
        npancake_t::init ("heavy-cost");

        // now, examine all successors. Note that the goal is fake, but this
        // doesn't matter as it is not used!
        vector<tuple<int, int, npancake_t>> successors;
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
