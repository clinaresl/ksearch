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
#include <vector>

#include "../fixtures/TSTmAfixture.h"

using namespace std;

// Checks that the mA search solver can be created. If the search algorithm is
// not invoked, it returns the default values
// ----------------------------------------------------------------------------
TEST_F (MAFixture, ExplicitConstructorPancake) {

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

// Checks that solvable instances where start=goal can be correctly solved
// ----------------------------------------------------------------------------
TEST_F (MAFixture, SolvableSameNPancakeSolution) {

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

// Check that mA* correctly finds one single solution between two instances of
// the 5-Pancake
TEST_F (MAFixture, SolvableNPancakeOne) {

        for (auto i = 0 ; i < NB_TESTS ; i++) {

            // create a manager to find a single solution between a couple of
            // random instances of the 5-Pancake
            int k = 1;
            npancake_t start = randInstance (5);
            npancake_t goal = randInstance (5);
            khs::mA<npancake_t> manager {k, start, goal};

            // initialize the static information of the n-pancake
            npancake_t::init ("unit");

            // and invoke the solver
            auto ksolution = manager.solve ();

            // verify the solution found contains one single solution
            ASSERT_EQ (ksolution.size (), k);

            // and verify it is correct
            khs::solution_t solution = ksolution[0];
            ASSERT_TRUE (solution.doctor ());
        }
}

// Check that mA* correctly finds two single solutions between two instances of
// the 5-Pancake
TEST_F (MAFixture, SolvableNPancakeTwo) {

        for (auto i = 0 ; i < NB_TESTS ; i++) {

            // create a manager to find two solutions between a couple of random
            // instances of the 5-Pancake which are guaranteed to be different
            int k = 2;
            npancake_t start = randInstance (5);
            npancake_t goal = randInstance (5);
            while (start == goal) {
                goal = randInstance (5);
            }
            khs::mA<npancake_t> manager {k, start, goal};

            // initialize the static information of the n-pancake
            npancake_t::init ("unit");

            // and invoke the solver
            auto ksolution = manager.solve ();

            // verify the solution found contains two solutions
            ASSERT_EQ (ksolution.size (), k);

            // and verify they are correct
            khs::solution_t<npancake_t, vector> solution = ksolution[0];
            ASSERT_TRUE (solution.doctor ());

            solution = ksolution[1];
            ASSERT_TRUE (solution.doctor ());

            // to conclude, verify that the second solution is strictly larger
            // or equal than the first one
            ASSERT_GE (ksolution[1].get_length (), ksolution[0].get_length ());
        }
}

// Check that mA* correctly finds an arbitrary number of solutions (10 <= k <=
// 20) between two instances of the 5-Pancake
TEST_F (MAFixture, SolvableNPancakeArbitrary) {

        for (auto i = 0 ; i < NB_TESTS ; i++) {

            // create a manager to find an arbitrary number of solutions between
            // a couple of random instances of the 5-Pancake which are
            // guaranteed to be different
            int k = 10 + (rand () % 11);
            npancake_t start = randInstance (5);
            npancake_t goal = randInstance (5);
            while (start == goal) {
                goal = randInstance (5);
            }
            khs::mA<npancake_t> manager {k, start, goal};

            // initialize the static information of the n-pancake
            npancake_t::init ("unit");

            // and invoke the solver
            auto ksolution = manager.solve ();

            // verify the solution found contains two solutions
            ASSERT_EQ (ksolution.size (), k);

            // and verify they are correct
            khs::solution_t<npancake_t, vector> solution = ksolution[0];
            ASSERT_TRUE (solution.doctor ());

            solution = ksolution[1];
            ASSERT_TRUE (solution.doctor ());

            // to conclude, verify that the second solution is strictly larger
            // or equal than the first one
            ASSERT_GE (ksolution[1].get_length (), ksolution[0].get_length ());
        }
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
