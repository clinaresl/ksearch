// -*- coding: utf-8 -*-
// TSTksolution_t.cc
// -----------------------------------------------------------------------------
//
// Started on <mié 09-08-2023 14:07:52.747780772 (1691582872)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Unit tests of the ksolution_t class
//

#include <vector>

#include "../fixtures/TSTksolutionfixture.h"

using namespace std;

// Verify that the explicit constructor correctly sets up the value of its
// parameters
// ----------------------------------------------------------------------------
TEST_F (KSolutionFixture, ExplicitConstructor) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // Create a container for storing k solutions from a specific start to
        // another goal
        int k = rand () % MAX_VALUE;
        npancake_t start = randInstance (10 + rand () % (NB_DISCS-10));
        npancake_t goal = randInstance (10 + rand () % (NB_DISCS-10));
        khs::ksolution_t ks {k, start, goal};

        // and now ensure that all parameters have been correctly set
        ASSERT_EQ (ks.get_k (), k);
        ASSERT_EQ (ks.get_start (), start);
        ASSERT_EQ (ks.get_goal (), goal);
    }
}

// Verify that a single solution can be added to the solution of the k-shortest
// path problem
// ----------------------------------------------------------------------------
TEST_F (KSolutionFixture, AddSolution) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // generate a random instance of the N-Pancake
        npancake_t start = randInstance (NB_DISCS);

        // initialize the unit cost model
        npancake_t::init ("unit");

        // and randomly generate a path from it with a random length
        pair<vector<npancake_t>, int> randWalk = randPath (start, 10 + rand () % (MAX_PATH_LENGTH - 10));

        // explicitly create other data for populating this solution
        vector<npancake_t> path = randWalk.first;
        npancake_t goal = path.back ();
        int h0 = rand () % MAX_VALUE;
        int cost = randWalk.second;
        size_t expansions = 1 + rand () % MAX_VALUE;
        double cpu_time = static_cast<double>(rand () % MAX_VALUE);
        string solver = randString (50);

        // Create a solution which stores all this information
        khs::solution_t<npancake_t> solution (path, start, goal,
                                              h0, cost, expansions, cpu_time, solver);

        // Create a container for storing k solutions from a specific start to
        // another goal
        int k = rand () % MAX_VALUE;
        khs::ksolution_t ks {k, start, goal};

        // and add the solution randomly generated
        ks += solution;

        // verify now the number of solutions is equal to one
        ASSERT_EQ (ks.size (), 1);

        // and also that its unique solution is strictly equal to the one randomly generated
        ASSERT_EQ (ks[0], solution);
    }
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
