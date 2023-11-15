// -*- coding: utf-8 -*-
// TSTsolution_t.cc
// -----------------------------------------------------------------------------
//
// Started on <mié 09-08-2023 11:51:51.275875892 (1691574711)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Unit tests of the solution_t class
//

#include <vector>

#include "../fixtures/TSTsolutionfixture.h"

using namespace std;

// Checks that solutions can be correctly created using the explicit
// constructors with non-null solution paths
// ----------------------------------------------------------------------------
TEST_F (SolutionFixture, ExplicitConstructorInt) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // explicitly create values to populate a solution
        int k = rand () % MAX_VALUE;
        vector<int> path = randVectorInt (MAX_VALUES, MAX_VALUES);
        int start = rand () % MAX_VALUE;
        int goal = rand () % MAX_VALUE;
        int nbcentroids = rand () & MAX_VALUE;
        int h0 = rand () % MAX_VALUE;
        int cost = rand () % MAX_VALUE;
        size_t expansions = rand () % MAX_VALUE;
        double cpu_time = static_cast<double>(rand () % MAX_VALUE);
        string solver = randString (50);

        // create a solution which stores ints
        khs::solution_t<int, vector> solution{k, path, start, goal, nbcentroids,
            h0, cost, expansions, cpu_time, solver};

        // and give this instance a name
        auto name = randString (50);
        solution.set_name (name);

        // verify that all values have been correctly stored
        ASSERT_EQ (k, solution.get_k ());
        ASSERT_EQ (start, solution.get_start ());
        ASSERT_EQ (goal, solution.get_goal ());
        ASSERT_EQ (path, solution.get_solution ());
        ASSERT_EQ (nbcentroids, solution.get_nbcentroids ());
        ASSERT_EQ (h0, solution.get_h0 ());
        ASSERT_EQ (path.size () - 1, solution.get_length ());
        ASSERT_EQ (cost, solution.get_cost ());
        ASSERT_EQ (expansions, solution.get_expansions ());
        ASSERT_EQ (cpu_time, solution.get_cpu_time ());
        ASSERT_EQ (solver, solution.get_solver ());

        // verify also the name is correct
        ASSERT_EQ (solution.get_name (), name);
    }
}

// Checks that empty solutions are recognized as such
// ----------------------------------------------------------------------------
TEST_F (SolutionFixture, EmptySolutionNPancake) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // explicitly create values to populate a solution. Note that empty
        // solutions are characterized by an empty solution path which is
        // generated with no expansions at all, and this is correct if and only
        // if the start and goal nodes are the same
        int k = rand () % MAX_VALUE;
        vector<npancake_t> path;
        npancake_t start = randInstance (NB_DISCS);
        npancake_t goal = start;
        int h0 = rand () % MAX_VALUE;
        int cost = 0;
        size_t expansions = 0;
        double cpu_time = static_cast<double>(rand () % MAX_VALUE);
        string solver = randString (50);

        // create a solution which stores npancakes
        khs::solution_t<npancake_t, vector> solution (k, path, start, goal, 0,
                                                      h0, cost, expansions, cpu_time, solver);

        // check that doctor recognizes this as a correct solution
        ASSERT_TRUE (solution.doctor ());
    }
}

// Check that non-empty correct solutions are recognized as such using the Unit
// Cost model of the Pancake
// ----------------------------------------------------------------------------
TEST_F (SolutionFixture, NonEmptySolutionNPancakeUnit) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // generate a random instance of the N-Pancake
        npancake_t start = randInstance (NB_DISCS);

        // initialize the unit cost model
        npancake_t::init ("unit");

        // and randomly generate a path from it with a random length
        pair<vector<npancake_t>, int> randWalk = randPath (start, rand () % MAX_PATH_LENGTH);

        // explicitly create other data for populating this solution
        int k = rand () % MAX_VALUE;
        vector<npancake_t> path = randWalk.first;
        npancake_t goal = path.back ();
        int h0 = rand () % MAX_VALUE;
        int cost = randWalk.second;
        size_t expansions = 1 + rand () % MAX_VALUE;
        double cpu_time = static_cast<double>(rand () % MAX_VALUE);
        string solver = randString (50);

        // Create a solution which stores all this information
        khs::solution_t<npancake_t, vector> solution (k, path, start, goal, 0,
                                                      h0, cost, expansions, cpu_time, solver);

        // verify the solution is correct
        ASSERT_TRUE (solution.doctor ());
    }
}

// Check that non-empty correct solutions are recognized as such using the Heavy
// Cost model of the Pancake
// ----------------------------------------------------------------------------
TEST_F (SolutionFixture, NonEmptySolutionNPancakeHeavy) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // generate a random instance of the N-Pancake
        npancake_t start = randInstance (NB_DISCS);

        // initialize the unit cost model
        npancake_t::init ("heavy-cost");

        // and randomly generate a path from it with a random length
        pair<vector<npancake_t>, int> randWalk = randPath (start, rand () % MAX_PATH_LENGTH);

        // explicitly create other data for populating this solution
        int k = rand () % MAX_VALUE;
        vector<npancake_t> path = randWalk.first;
        npancake_t goal = path.back ();
        int h0 = rand () % MAX_VALUE;
        int cost = randWalk.second;
        size_t expansions = 1 + rand () % MAX_VALUE;
        double cpu_time = static_cast<double>(rand () % MAX_VALUE);
        string solver = randString (50);

        // Create a solution which stores all this information
        khs::solution_t<npancake_t, vector> solution (k, path, start, goal, 0,
                                                      h0, cost, expansions, cpu_time, solver);

        // verify the solution is correct
        ASSERT_TRUE (solution.doctor ());
    }
}

// Checks that empty solutions which report a number of expansions are incorrect
// ----------------------------------------------------------------------------
TEST_F (SolutionFixture, ErrorEmptySolutionNPancake) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // explicitly create values to populate a solution. Note that empty
        // solutions are characterized by an empty solution path which should
        // generated with no expansions at all, and this is correct if and only
        // if the start and goal nodes are the same
        int k = rand () % MAX_VALUE;
        vector<npancake_t> path;
        npancake_t start = randInstance (NB_DISCS);
        npancake_t goal = start;
        int h0 = rand () % MAX_VALUE;
        int cost = 0;
        size_t expansions = 1 + rand () % MAX_VALUE;
        double cpu_time = static_cast<double>(rand () % MAX_VALUE);
        string solver = randString (50);

        // create a solution which stores npancakes
        khs::solution_t<npancake_t, vector> solution (k, path, start, goal, 0,
                                                      h0, cost, expansions, cpu_time, solver);

        // check that doctor recognizes this solution is incorrect as a strictly
        // positive number of expanions is reported
        ASSERT_FALSE (solution.doctor ());

        // Ensure the error is indeed that there is an empty solution path with
        // a strictly positive number of expansions
        ASSERT_EQ (solution.get_error_code (), khs::solution_error::ERR_EXPANSIONS);
    }
}

// Check that solutions with a start node other than the one appearing in the
// solution path are recognized as incorrect
// ----------------------------------------------------------------------------
TEST_F (SolutionFixture, ErrorStartNPancake) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // generate a random instance of the N-Pancake
        npancake_t start = randInstance (NB_DISCS);

        // initialize the unit cost model
        npancake_t::init ("unit");

        // and randomly generate a path from it with a random length
        pair<vector<npancake_t>, int> randWalk = randPath (start, 2 + rand () % MAX_PATH_LENGTH);

        // explicitly create other data for populating this solution where the
        // start state is different. Note that the second state in the random
        // walk is necessarily different than the first one (because no state is
        // a descendant of itself) and thus it can be used to simulate this error.
        int k = rand () % MAX_VALUE;
        vector<npancake_t> path = randWalk.first;
        start = path[1];
        npancake_t goal = path.back ();
        int h0 = rand () % MAX_VALUE;
        int cost = randWalk.second;
        size_t expansions = 1 + rand () % MAX_VALUE;
        double cpu_time = static_cast<double>(rand () % MAX_VALUE);
        string solver = randString (50);

        // Create a solution which stores all this information
        khs::solution_t<npancake_t, vector> solution (k, path, start, goal, 0,
                                                      h0, cost, expansions, cpu_time, solver);

        // verify the solution is incorrect
        ASSERT_FALSE (solution.doctor ());

        // ensure the error is indeed that the start state is different than the
        // one appearing in the solution path
        ASSERT_EQ (solution.get_error_code (), khs::solution_error::ERR_START);
    }
}

// Check that solutions with a end node other than the one appearing in the
// solution path are recognized as incorrect
// ----------------------------------------------------------------------------
TEST_F (SolutionFixture, ErrorGoalNPancake) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // generate a random instance of the N-Pancake
        npancake_t start = randInstance (NB_DISCS);

        // initialize the unit cost model
        npancake_t::init ("unit");

        // and randomly generate a path from it with a random length
        pair<vector<npancake_t>, int> randWalk = randPath (start, 2 + rand () % MAX_PATH_LENGTH);

        // explicitly create other data for populating this solution where the
        // goal state is different. Note that the before last state in the
        // random walk is necessarily different than the last one (because no
        // state is a descendant of itself) and thus it can be used to simulate
        // this error.
        int k = rand () % MAX_VALUE;
        vector<npancake_t> path = randWalk.first;
        npancake_t goal = path[path.size () - 2];
        int h0 = rand () % MAX_VALUE;
        int cost = randWalk.second;
        size_t expansions = 1 + rand () % MAX_VALUE;
        double cpu_time = static_cast<double>(rand () % MAX_VALUE);
        string solver = randString (50);

        // Create a solution which stores all this information
        khs::solution_t<npancake_t, vector> solution (k, path, start, goal, 0,
                                                      h0, cost, expansions, cpu_time, solver);

        // verify the solution is incorrect
        ASSERT_FALSE (solution.doctor ());

        // ensure the error is indeed that the start state is different than the
        // one appearing in the solution path
        ASSERT_EQ (solution.get_error_code (), khs::solution_error::ERR_GOAL);
    }
}

// Check that solutions paths where a couple of states are not adjacent are
// recognized as incorrect
// ----------------------------------------------------------------------------
TEST_F (SolutionFixture, ErrorAdjacencyNPancake) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // generate a random instance of the N-Pancake
        npancake_t start = randInstance (NB_DISCS);

        // initialize the unit cost model
        npancake_t::init ("unit");

        // and randomly generate a path from it with a random length, at least
        // 10
        pair<vector<npancake_t>, int> randWalk = randPath (start, 10 + rand () % MAX_PATH_LENGTH);

        // modify the path so that a couple of states are not adjacent. To do
        // this, randomly pick a location of the path and remove it
        int k = rand () % MAX_VALUE;
        vector<npancake_t> path;
        int idx = 1 + rand () % (randWalk.first.size () - 2);
        copy (randWalk.first.begin (), randWalk.first.begin () + idx, back_inserter (path));
        copy (randWalk.first.begin () + idx + 1, randWalk.first.end (), back_inserter (path));

        // because we want to simulate the error on adjacency, substract one to
        // the cost (because under the unit cost model that's the cost of all
        // ops) to prevent the error on cost being raised
        int cost = randWalk.second - 1;

        // explicitly create other data for populating this solution
        npancake_t goal = path.back ();
        int h0 = rand () % MAX_VALUE;
        size_t expansions = 1 + rand () % MAX_VALUE;
        double cpu_time = static_cast<double>(rand () % MAX_VALUE);
        string solver = randString (50);


        // Create a solution which stores all this information
        khs::solution_t<npancake_t, vector> solution (k, path, start, goal, 0,
                                                      h0, cost, expansions, cpu_time, solver);

        // verify the solution is incorrect
        ASSERT_FALSE (solution.doctor ());

        // ensure the error is indeed that the start state is different than the
        // one appearing in the solution path
        ASSERT_EQ (solution.get_error_code (), khs::solution_error::ERR_ADJACENT);
    }
}

// Check that non-empty solutions with a wrong cost are recognized as incorrect
// solutions
// ----------------------------------------------------------------------------
TEST_F (SolutionFixture, ErrorCostNPancakeHeavy) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // generate a random instance of the N-Pancake
        npancake_t start = randInstance (NB_DISCS);

        // initialize the unit cost model
        npancake_t::init ("heavy-cost");

        // and randomly generate a path from it with a random length
        pair<vector<npancake_t>, int> randWalk = randPath (start, rand () % MAX_PATH_LENGTH);

        // explicitly create other data for populating this solution
        int k = rand () % MAX_VALUE;
        vector<npancake_t> path = randWalk.first;
        npancake_t goal = path.back ();
        int h0 = rand () % MAX_VALUE;
        int cost = randWalk.second + 1 + rand () % MAX_VALUE;
        size_t expansions = 1 + rand () % MAX_VALUE;
        double cpu_time = static_cast<double>(rand () % MAX_VALUE);
        string solver = randString (50);

        // Create a solution which stores all this information
        khs::solution_t<npancake_t, vector> solution (k, path, start, goal, 0,
                                                      h0, cost, expansions, cpu_time, solver);

        // verify the solution is incorrect
        ASSERT_FALSE (solution.doctor ());

        // ensure the error is indeed that the start state is different than the
        // one appearing in the solution path
        ASSERT_EQ (solution.get_error_code (), khs::solution_error::ERR_SOLUTION_COST);
    }
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
