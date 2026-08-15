// -*- coding: utf-8 -*-
// TSTnpancake.cc
// -----------------------------------------------------------------------------
//
// Started on <dom 27-02-2022 23:42:57.992602429 (1646001777)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

//
// Unit tests of the N-Pancake
//

#include<tuple>
#include<vector>

#include "../TSTdefs.h"
#include "../fixtures/TSTnpancakefixture.h"

// Check that instances are correctly recognized
// ----------------------------------------------------------------------------
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

// Check that all successors are correctly generated when using a consistent
// heuristic in the unit variant
// ----------------------------------------------------------------------------
TEST_F (NPancakeFixture, SuccessorsUnitConsistent) {

    for (auto i = 0 ; i < NB_TESTS ; i++ ) {

        // first, generate a random instance
        npancake_t instance = randInstance (10 + rand () % (NB_DISCS-10));

        // initialize the incremental table with the updates of the gap
        // heuristic. Since the size is not determined until an instance is
        // built, the initialization is done in every loop
        npancake_t::init ("unit");

        // verify the variant and whether the heuristic function is consistent
        ASSERT_EQ (npancake_t::get_variant (), "unit");

        // now, expand this node and generate all children. Note that neither h
        // nor the goal are relevant to generate the children and just they take
        // any arbitrary value
        int nbchildren = 0;
        instance.children (
            0,
            instance,
            [&] (int g, int h, npancake_t&& child) {
                
                // increment the number of children computed
                ++nbchildren;
                
                // children are generated in increasing order, first flipping
                // the first two discs, then three, and so on. Verify that
                // children are correctly generated
                ASSERT_EQ (nbchildren+1, getPrefix (instance, child));
                
                // verify that the cost is always equal to 1
                ASSERT_EQ (g, 1);
            });

        // Verify the number of descendants equals its length minus one
        ASSERT_EQ (nbchildren, instance.get_n ()-1);
    }
}

// Check that all successors are correctly generated when using a consistent
// heuristic in the heavy-cost variant
// ----------------------------------------------------------------------------
TEST_F (NPancakeFixture, SuccessorsHeavyCostConsistent) {

    for (auto i = 0 ; i < NB_TESTS ; i++ ) {

        // first, generate a random instance
        npancake_t instance = randInstance (10 + rand () % (NB_DISCS-10));
        std::vector<int> perm = instance.get_perm ();

        // initialize the incremental table with the updates of the gap
        // heuristic. Since the size is not determined until an instance is
        // built, the initialization is done in every loop
        npancake_t::init ("heavy-cost");

        // verify the variant and whether the heuristic function is consistent
        ASSERT_EQ (npancake_t::get_variant (), "heavy-cost");

        // now, expand this node and generate all children. Note that neither h
        // nor the goal are relevant to generate the children and just they take
        // any arbitrary value
        int nbchildren = 0;
        instance.children (
            0,
            instance,
            [&] (int g, int h, npancake_t&& child) {
                
                // increment the number of children computed
                ++nbchildren;
                
                // children are generated in increasing order, first flipping
                // the first two discs, then three, and so on. Verify that
                // children are correctly generated
                ASSERT_EQ (nbchildren+1, getPrefix (instance, child));
                
                // verify that the cost is always equal to the radius of the
                // first disc not being transposed
                ASSERT_EQ (g, ((nbchildren==perm.size ()-1) ? perm.size () + 1 : perm[1+nbchildren]));
            });

        // Verify the number of descendants equals its length minus one
        ASSERT_EQ (nbchildren, instance.get_n ()-1);
    }
}

// check the incremental computation of the gap heuristic is correct for the
// unit variant and that it is also consistent
// ----------------------------------------------------------------------------
TEST_F (NPancakeFixture, UnitIncrementalGapConsistent) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // first, generate a random instance of the n-pancake with the length of
        // the pancake randomly selected in the interval [10, 20)
        int length = 10 + rand () % (NB_DISCS-10);
        npancake_t instance = randInstance (length);

        // initialize the incremental table with the updates of the gap
        // heuristic. Since the size is not determined until an instance is
        // built, the initialization is done in every loop
        npancake_t::init ("unit");

        // verify the variant is unit and that a consistent heuristic function
        // is in use
        ASSERT_EQ (npancake_t::get_variant (), "unit");

        // compute the heuristic value of this instance
        int hparent = instance.h (instance);

        // now, examine all successors. Note that the goal is fake, but this
        // doesn't matter as it is not used!
        instance.children (
            instance.h (instance),
            instance,
            [&] (int g, int h, npancake_t&& successor) {
                
                // and verify that the heuristic value of this successor is equal to
                // its gap
                ASSERT_EQ (h, successor.h (instance));

                // Verify also that the heuristic is consistent
                ASSERT_LE (abs (hparent - h), g);
            });
    }
}

// check the incremental computation of the gap heuristic is correct for the
// heavy-cost variant when using a consistent heuristic function and that it is
// also consistent
// ----------------------------------------------------------------------------
TEST_F (NPancakeFixture, HeavyCostIncrementalGapConsistent) {

    for (auto i = 0 ; i < 10*NB_TESTS ; i++) {

        // first, generate a random instance of the n-pancake with the length of
        // the pancake randomly selected in the interval [10, 20)
        int length = 10 + rand () % (NB_DISCS-10);
        npancake_t instance = randInstance (length);

        // initialize the incremental table with the updates of the gap
        // heuristic. Since the size is not determined until an instance is
        // built, the initialization is done in every loop
        npancake_t::init ("heavy-cost");

        // verify the variant is heavy-cost and that a consistent heuristic
        // function is in use
        ASSERT_EQ (npancake_t::get_variant (), "heavy-cost");

        // compute the heuristic value of this instance
        int hparent = instance.h (instance);

        // now, examine all successors. Note that the goal is fake, but this
        // doesn't matter as it is not used!
        instance.children (
            instance.h (instance),
            instance,
            [&] (int g, int h, npancake_t&& successor) {
                
                // and verify that the heuristic value of this successor is equal to
                // its gap
                ASSERT_EQ (h, successor.h (instance));

                // Verify also that the heuristic is consistent
                ASSERT_LE (abs (hparent - h), g);
            });
    }
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
