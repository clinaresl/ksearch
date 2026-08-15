// -*- coding: utf-8 -*-
// tstmA.cc
// -----------------------------------------------------------------------------
//
// Started on <jue 10-08-2023 16:59:29.570258624 (1691679569)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

//
// Unit tests of the mA search algorithm
//

#include <limits>

#include "../fixtures/TSTmAfixture.h"

// Checks that the mA search solver can be created. If the search algorithm is
// not invoked, it returns the default values
// ----------------------------------------------------------------------------
TEST_F (MAFixture, ExplicitConstructorNPancake) {

    for (auto i = 0 ; i < NB_TESTS ; i++ ) {

        // create a manager to run the mA search algorithm to solve a random
        // instance a random number of times. h=0 is given for the original node
        // because no search is performed
        int k = 1 + rand () % MAX_VALUES;
        npancake_t start = randInstance (NB_DISCS);
        npancake_t goal = randInstance (NB_DISCS);
        khs::mA<npancake_t> manager {k, start, goal};

        // because the search algorithm is not invoked, it is expected ...
        ASSERT_EQ(manager.get_start (), start);
        ASSERT_EQ(manager.get_goal (), goal);
        ASSERT_EQ (manager.get_k (), k);
        ASSERT_EQ (manager.get_expansions (), 0);

        // check that the heuristic value is accurately computed
        ASSERT_EQ (manager.get_h0 (), start.h (goal));

        // note that the running CPU time can not be queried and the result is
        // unpredictable
    }

}

// Checks that solvable instances where start=goal can be correctly solved in
// the n-pancake
// ----------------------------------------------------------------------------
TEST_F (MAFixture, NPancakeBruteForceSameSolution) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to run the mA search algorithm to solve a random
        // instance a random number of times. h=0 is given for the original node
        // because no search is performed
        int k = 1 + rand () % MAX_VALUES;
        npancake_t start = randInstance (NB_DISCS);
        khs::mA<npancake_t> manager {k, start, start};

        // and invoke the solver
        auto ksolution = manager.solve ();

        // now, make sure the result makes sense, i.e., there is only one
        // solution (in spite of the value of k), which has a null length,
        // signaled with a value equal to -1 (and thus, null cost also in spite
        // of the cost model used)
        ASSERT_EQ (ksolution.size (), 1);
        ASSERT_EQ (ksolution[0].get_length (), -1);
        ASSERT_EQ (ksolution[0].get_cost (), 0);
    }
}

// Checks that solvable instances where start=goal can be correctly solved in
// the n-pancake with consistent heuristics
// ----------------------------------------------------------------------------
TEST_F (MAFixture, NPancakeConsistentSameSolution) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to run the mA search algorithm to solve a random
        // instance a random number of times. h=0 is given for the original node
        // because no search is performed
        int k = 1 + rand () % MAX_VALUES;
        npancake_t start = randInstance (NB_DISCS);
        npancake_t::init ("unit");
        khs::mA<npancake_t> manager {k, start, start, false};

        // verify the variant is unit and that a consistent heuristic function
        // is in use
        ASSERT_EQ (npancake_t::get_variant (), "unit");

        // and invoke the solver
        auto ksolution = manager.solve ();

        // now, make sure the result makes sense, i.e., there is only one
        // solution (in spite of the value of k), which has a null length,
        // signaled with a value equal to -1 (and thus, null cost also in spite
        // of the cost model used)
        ASSERT_EQ (ksolution.size (), 1);
        ASSERT_EQ (ksolution[0].get_length (), -1);
        ASSERT_EQ (ksolution[0].get_cost (), 0);
    }
}

// Check that mA0 correctly finds one single solution between a random instance
// of the 6-Pancake and the identity permutation in the unit domain
// ----------------------------------------------------------------------------
TEST_F (MAFixture, NPancakeUnitBruteForceOne) {

    for (auto i = 0 ; i < NB_TESTS/10 ; i++) {

        // create a manager to find a single solution between a couple of
        // random instances of the 6-Pancake
        int k = 1;
        npancake_t start = randInstance (6);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5, 6};
        while (start == goal) {
            start = randInstance (6);
        }
        npancake_t::init ("unit");

        // verify the variant and whether the heuristic function is consistent
        // even if no heuristic function is used
        ASSERT_EQ (npancake_t::get_variant (), "unit");

        khs::mA<npancake_t> manager {k, start, goal};

        // and invoke the solver
        auto ksolution = manager.solve0 ();

        // verify the solution found contains one single solution
        ASSERT_EQ (ksolution.size (), k);

        // and verify it is correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that mA0 correctly find two single solutions between a random instance
// of the 6-Pancake and the identity permutation in the unit domain
// ----------------------------------------------------------------------------
TEST_F (MAFixture, NPancakeUnitBruteForceTwo) {

    for (auto i = 0 ; i < NB_TESTS/10 ; i++) {

        // create a manager to find two solutions between a couple of random
        // instances of the 6-Pancake which are guaranteed to be different
        int k = 2;
        npancake_t start = randInstance (6);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5, 6};
        while (start == goal) {
            start = randInstance (6);
        }
        npancake_t::init ("unit");

        // verify the variant and whether the heuristic function is consistent
        // even if no heuristic function is used
        ASSERT_EQ (npancake_t::get_variant (), "unit");

        khs::mA<npancake_t> manager {k, start, goal};

        // and invoke the solver
        auto ksolution = manager.solve0 ();

        // verify the solution found contains two solutions
        ASSERT_EQ (ksolution.size (), k);

        // and verify they are correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that mA0 correctly finds an arbitrary number of solutions (10 <= k <=
// 20) between a random instance of the 6-Pancake and the identity permutation
// in the unit variant
// ----------------------------------------------------------------------------
TEST_F (MAFixture, NPancakeUnitBruteForceArbitrary) {

    for (auto i = 0 ; i < NB_TESTS/10 ; i++) {

        // create a manager to find an arbitrary number of solutions between
        // a couple of random instances of the 6-Pancake which are
        // guaranteed to be different
        int k = 10 + (rand () % 11);
        npancake_t start = randInstance (6);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5, 6};
        while (start == goal) {
            start = randInstance (6);
        }
        npancake_t::init ("unit");

        // verify the variant and whether the heuristic function is consistent
        // even if no heuristic function is used
        ASSERT_EQ (npancake_t::get_variant (), "unit");

        khs::mA<npancake_t> manager {k, start, goal};

        // and invoke the solver
        auto ksolution = manager.solve0 ();

        // verify the solution found contains two solutions
        ASSERT_EQ (ksolution.size (), k);

        // and verify they are correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that mA* correctly finds one single solution between a random instance
// of the 8-Pancake and the identity permutation in the unit domain using a
// consistent heuristic
// ----------------------------------------------------------------------------
TEST_F (MAFixture, NPancakeUnitConsistentOne) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to find a single solution between a couple of
        // random instances of the 8-Pancake
        int k = 1;
        npancake_t start = randInstance (8);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5, 6, 7, 8};
        while (start == goal) {
            start = randInstance (8);
        }
        npancake_t::init ("unit");

        // verify the variant and whether the heuristic function is consistent
        ASSERT_EQ (npancake_t::get_variant (), "unit");

        khs::mA<npancake_t> manager {k, start, goal, false};

        // and invoke the solver
        auto ksolution = manager.solve0 ();

        // verify the solution found contains one single solution
        ASSERT_EQ (ksolution.size (), k);

        // and verify it is correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that mA* correctly find two single solutions between a random instance
// of the 8-Pancake and the identity permutation in the unit domain using a
// consistent heuristic
// ----------------------------------------------------------------------------
TEST_F (MAFixture, NPancakeUnitConsistentTwo) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to find two solutions between a couple of random
        // instances of the 8-Pancake which are guaranteed to be different
        int k = 2;
        npancake_t start = randInstance (8);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5, 6, 7, 8};
        while (start == goal) {
            start = randInstance (8);
        }
        npancake_t::init ("unit");

        // verify the variant and whether the heuristic function is consistent
        ASSERT_EQ (npancake_t::get_variant (), "unit");

        khs::mA<npancake_t> manager {k, start, goal, false};

        // and invoke the solver
        auto ksolution = manager.solve0 ();

        // verify the solution found contains two solutions
        ASSERT_EQ (ksolution.size (), k);

        // and verify they are correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that mA* correctly finds an arbitrary number of solutions (10 <= k <=
// 20) between a random instance of the 8-Pancake and the identity permutation
// in the unit variant with a consistent heuristic
// ----------------------------------------------------------------------------
TEST_F (MAFixture, NPancakeUnitConsistentArbitrary) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to find an arbitrary number of solutions between
        // a couple of random instances of the 8-Pancake which are
        // guaranteed to be different
        int k = 10 + (rand () % 11);
        npancake_t start = randInstance (8);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5, 6, 7, 8};
        while (start == goal) {
            start = randInstance (8);
        }
        npancake_t::init ("unit");

        // verify the variant and whether the heuristic function is consistent
        ASSERT_EQ (npancake_t::get_variant (), "unit");

        khs::mA<npancake_t> manager {k, start, goal, false};

        // and invoke the solver
        auto ksolution = manager.solve0 ();

        // verify the solution found contains two solutions
        ASSERT_EQ (ksolution.size (), k);

        // and verify they are correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that mA0 correctly finds one single solution between a random instance
// of the 6-Pancake and the identity permutation in the heavy-cost domain
// ----------------------------------------------------------------------------
TEST_F (MAFixture, NPancakeHeavyCostBruteForceOne) {

    for (auto i = 0 ; i < NB_TESTS/10 ; i++) {

        // create a manager to find a single solution between a couple of
        // random instances of the 6-Pancake
        int k = 1;
        npancake_t start = randInstance (6);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5, 6};
        while (start == goal) {
            start = randInstance (6);
        }
        npancake_t::init ("heavy-cost");
        khs::mA<npancake_t> manager {k, start, goal};

        // verify the variant and whether the heuristic function is consistent
        // even if no heuristic function is used
        ASSERT_EQ (npancake_t::get_variant (), "heavy-cost");

        // and invoke the solver
        auto ksolution = manager.solve0 ();

        // verify the solution found contains one single solution
        ASSERT_EQ (ksolution.size (), k);

        // and verify it is correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that mA0 correctly finds two single solutions between a random instance
// of the 6-Pancake and the identity permutation in the heavy-cost domain
// ----------------------------------------------------------------------------
TEST_F (MAFixture, NPancakeHeavyCostBruteForceTwo) {

    for (auto i = 0 ; i < NB_TESTS/10 ; i++) {

        // create a manager to find two solutions between a couple of random
        // instances of the 6-Pancake which are guaranteed to be different
        int k = 2;
        npancake_t start = randInstance (6);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5, 6};
        while (start == goal) {
            start = randInstance (6);
        }
        npancake_t::init ("heavy-cost");
        khs::mA<npancake_t> manager {k, start, goal};

        // verify the variant and whether the heuristic function is consistent
        // even if no heuristic function is used
        ASSERT_EQ (npancake_t::get_variant (), "heavy-cost");

        // and invoke the solver
        auto ksolution = manager.solve0 ();

        // verify the solution found contains two solutions
        ASSERT_EQ (ksolution.size (), k);

        // and verify they are correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that mA0 correctly finds an arbitrary number of solutions (10 <= k <=
// 20) between a random instance of the 6-Pancake and the identity permutation
// in the heavy-cost domain
// ----------------------------------------------------------------------------
TEST_F (MAFixture, NPancakeHeavyCostBruteForceArbitrary) {

    for (auto i = 0 ; i < NB_TESTS/10 ; i++) {

        // create a manager to find an arbitrary number of solutions between
        // a couple of random instances of the 6-Pancake which are
        // guaranteed to be different
        int k = 10 + (rand () % 11);
        npancake_t start = randInstance (6);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5, 6};
        while (start == goal) {
            start = randInstance (6);
        }
        npancake_t::init ("heavy-cost");
        khs::mA<npancake_t> manager {k, start, goal};

        // verify the variant and whether the heuristic function is consistent
        // even if no heuristic function is used
        ASSERT_EQ (npancake_t::get_variant (), "heavy-cost");

        // and invoke the solver
        auto ksolution = manager.solve0 ();

        // verify the solution found contains two solutions
        ASSERT_EQ (ksolution.size (), k);

        // and verify they are correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that mA* correctly finds one single solution between a random instance
// of the 6-Pancake and the identity permutation in the heavy-cost domain using
// a consistent heuristic
// ----------------------------------------------------------------------------
TEST_F (MAFixture, NPancakeHeavyCostConsistentOne) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to find a single solution between a couple of
        // random instances of the 8-Pancake
        int k = 1;
        npancake_t start = randInstance (6);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5, 6};
        while (start == goal) {
            start = randInstance (6);
        }
        npancake_t::init ("heavy-cost");
        khs::mA<npancake_t> manager {k, start, goal, false};

        // verify the variant and whether the heuristic function is consistent
        ASSERT_EQ (npancake_t::get_variant (), "heavy-cost");

        // and invoke the solver
        auto ksolution = manager.solve0 ();

        // verify the solution found contains one single solution
        ASSERT_EQ (ksolution.size (), k);

        // and verify it is correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that mA* correctly find two single solutions between a random instance
// of the 6-Pancake and the identity permutation in the heavy-cost domain using
// a consistent heuristic
// ----------------------------------------------------------------------------
TEST_F (MAFixture, NPancakeHeavyCostConsistentTwo) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to find two solutions between a couple of random
        // instances of the 8-Pancake which are guaranteed to be different
        int k = 2;
        npancake_t start = randInstance (6);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5, 6};
        while (start == goal) {
            start = randInstance (6);
        }
        npancake_t::init ("heavy-cost");
        khs::mA<npancake_t> manager {k, start, goal, false};

        // verify the variant and whether the heuristic function is consistent
        ASSERT_EQ (npancake_t::get_variant (), "heavy-cost");

        // and invoke the solver
        auto ksolution = manager.solve0 ();

        // verify the solution found contains two solutions
        ASSERT_EQ (ksolution.size (), k);

        // and verify they are correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that mA* correctly finds an arbitrary number of solutions (10 <= k <=
// 20) between a random instance of the 6-Pancake and the identity permutation
// in the heavy-cost variant with a consistent heuristic
// ----------------------------------------------------------------------------
TEST_F (MAFixture, NPancakeHeavyCostConsistentArbitrary) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to find an arbitrary number of solutions between
        // a couple of random instances of the 8-Pancake which are
        // guaranteed to be different
        int k = 10 + (rand () % 11);
        npancake_t start = randInstance (6);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5, 6};
        while (start == goal) {
            start = randInstance (6);
        }
        npancake_t::init ("heavy-cost");
        khs::mA<npancake_t> manager {k, start, goal, false};

        // verify the variant and whether the heuristic function is consistent
        ASSERT_EQ (npancake_t::get_variant (), "heavy-cost");

        // and invoke the solver
        auto ksolution = manager.solve0 ();

        // verify the solution found contains two solutions
        ASSERT_EQ (ksolution.size (), k);

        // and verify they are correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
