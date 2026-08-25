// -*- coding: utf-8 -*-
// TSTksolution_t.cc
// -----------------------------------------------------------------------------
//
// Started on <mié 09-08-2023 14:07:52.747780772 (1691582872)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

//
// Unit tests of the ksolution_t class
//

#include <vector>

#include "../fixtures/TSTksolutionfixture.h"
#include "../../src/ksearch.h"

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
        khs::ksolution_t<npancake_t, std::vector> ks {k, start, goal};

        // and now ensure that all parameters have been correctly set
        ASSERT_EQ (ks.get_k (), k);
        ASSERT_EQ (ks.get_start (), start);
        ASSERT_EQ (ks.get_goal (), goal);
    }
}

// Verify that an empty container can be effectdively added to the solution of
// the k-shortest path problem
// ----------------------------------------------------------------------------
TEST_F (KSolutionFixture, AddEmptySolution) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // generate a random instance of the N-Pancake
        npancake_t start = randInstance (NB_DISCS);

        // initialize the unit cost model
        npancake_t::init ("unit");

        // and randomly generate a path from it with a random length
        std::pair<std::vector<npancake_t>, int> randWalk = randPath (start, 10 + rand () % (MAX_PATH_LENGTH - 10));

        // explicitly create other data for populating this solution
        int k = rand () % MAX_VALUE;
        std::vector<npancake_t> path = randWalk.first;
        npancake_t goal = path.back ();
        int z_exp = rand () % MAX_VALUE;
        int z_gen = rand () % MAX_VALUE;
        int h0 = rand () % MAX_VALUE;
        int cost = randWalk.second;
        size_t expansions = 1 + rand () % MAX_VALUE;
        double cpu_time = static_cast<double>(rand () % MAX_VALUE);
        int nbpaths = rand () % MAX_VALUE;
        std::string solver = randString (50);

        // Create a container of solutions with only this solution. Note there
        // is already a specific unit test for verifying that a single solution
        // can be added to an empty container
        khs::solution_t<npancake_t, std::vector> solution (k, path, start, goal, z_exp, z_gen, 
                                                      h0, cost, expansions, cpu_time,
                                                      nbinconsistencies, air, nbpaths, solver, false);
        khs::ksolution_t<npancake_t, std::vector> k1 {k, start, goal};
        k1 += solution;

        // Create an empty container supposedly used for storing k solutions
        // from a specific start to another goal
        khs::ksolution_t<npancake_t, std::vector> k0 {k, start, goal};

        // and add the empty container to the former
        k1 += k0;

        // verify now the number of solutions in both containers
        ASSERT_EQ (k0.size (), 0);
        ASSERT_EQ (k1.size (), 1);
    }
}

// Verify that a container with a single solution can be added to the solution
// of the k-shortest path problem
// ----------------------------------------------------------------------------
TEST_F (KSolutionFixture, AddSingleSolution) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // generate a random instance of the N-Pancake
        npancake_t start = randInstance (NB_DISCS);

        // initialize the unit cost model
        npancake_t::init ("unit");

        // and randomly generate a path from it with a random length
        std::pair<std::vector<npancake_t>, int> randWalk = randPath (start, 10 + rand () % (MAX_PATH_LENGTH - 10));

        // explicitly create other data for populating this solution
        int k = rand () % MAX_VALUE;
        std::vector<npancake_t> path = randWalk.first;
        npancake_t goal = path.back ();
        int z_exp = rand () % MAX_VALUE;
        int z_gen = rand () % MAX_VALUE;
        int h0 = rand () % MAX_VALUE;
        int cost = randWalk.second;
        size_t expansions = 1 + rand () % MAX_VALUE;
        double cpu_time = static_cast<double>(rand () % MAX_VALUE);
        int nbpaths = rand () % MAX_VALUE;
        std::string solver = randString (50);

        // Create a solution which stores all this information
        khs::solution_t<npancake_t, std::vector> solution (k, path, start, goal, z_exp, z_gen, 
                                                      h0, cost, expansions, cpu_time,
                                                      nbinconsistencies, air, nbpaths, solver, false);

        // Create a container for storing k solutions from a specific start to
        // another goal
        khs::ksolution_t<npancake_t, std::vector> ks {k, start, goal};

        // and add the solution randomly generated
        ks += solution;

        // verify now the number of solutions is equal to one
        ASSERT_EQ (ks.size (), 1);

        // and also that its unique solution is strictly equal to the one randomly generated
        ASSERT_EQ (ks[0], solution);
    }
}

// Verify that solutions that are equal are correctly classified by doctor
// ----------------------------------------------------------------------------
TEST_F (KSolutionFixture, DoctorCorrectSolution) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // Compute an arbitrary number of solutions, between 1 and 20, to a
        // problem of the 5-pancake using BELA*
        int k = 1 + (rand () % 20);
        npancake_t start = randInstance (5);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5};
        while (start == goal) {
            start = randInstance (5);
        }
        khs::bela<npancake_t> manager {k, start, goal, false};

        // initialize the static information of the n-pancake
        npancake_t::init ("unit");

        // and invoke the solver
        auto ksolution = manager.solveStar ();

        // and now verify this solution with itself
        ASSERT_TRUE (ksolution.doctor (ksolution));
    }
}

// Verify that containers with a different number of solutions are correctly
// classified by doctor
// ----------------------------------------------------------------------------
TEST_F (KSolutionFixture, DoctorDifferentSize) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // Compute an arbitrary number of solutions, between 1 and 20, to a
        // problem of the 5-pancake using BELA*
        int k = 1 + (rand () % 20);
        npancake_t start = randInstance (5);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5};
        while (start == goal) {
            start = randInstance (5);
        }
        khs::bela<npancake_t> manager {k, start, goal, false};

        // initialize the static information of the n-pancake
        npancake_t::init ("unit");

        // and invoke the solver
        auto ksolution = manager.solveStar ();

        // Make a copy of this container and remove one instance randomly chosen
        auto csolution = ksolution;
        csolution.remove (rand ()%ksolution.size ());

        // and now verify these solutions do not match
        ASSERT_FALSE (ksolution.doctor (csolution));
    }
}

// Verify that containers with a solution with different cost is correctly
// recognized by doctor
// ----------------------------------------------------------------------------
TEST_F (KSolutionFixture, DoctorDifferentCost) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // Compute 100 solutions to a problem of the 5-pancake using BELA*.
        // These number is selected to make it more likely that the first and
        // last solutions have different cost
        int k = 100;
        npancake_t start = randInstance (5);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5};
        while (start == goal) {
            start = randInstance (5);
        }
        khs::bela<npancake_t> manager {k, start, goal, false};

        // initialize the static information of the n-pancake
        npancake_t::init ("unit");

        // and invoke the solver and accept this case if and only if the first
        // and last solution have different cost
        auto ksolution = manager.solveStar ();
        if (ksolution[0].get_cost () == ksolution[ksolution.size ()-1].get_cost ()) {
            continue;
        }

        // Make a copy of this container, remove the last solution and add the
        // first one
        auto csolution = ksolution;
        csolution.remove (0);
        csolution += ksolution[0];

        // and now verify they have the same number of solutions (so this is not
        // the testcase shown above) but still these solutions do not match
        ASSERT_EQ (ksolution.size (), csolution.size ());
        ASSERT_FALSE (ksolution.doctor (csolution));
    }
}

// Verify that solutions generated by a solver which is required to generate
// simple solution paths are correctly diagnosed.
// ----------------------------------------------------------------------------
TEST_F (KSolutionFixture, DoctorSimplePath) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to find up to 10 different solutions between a
        // couple of random instances of the 5-Pancake. In some cases, all
        // solutions will be simple paths and, in other cases, there might be at
        // least one non-simple path
        int k = 10;
        npancake_t start = randInstance (5);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5};
        while (start == goal) {
            start = randInstance (5);
        }
        npancake_t::init ("unit");

        // use BELA* to find a single optimal solution
        khs::bela<npancake_t> manager {k, start, goal, false};
        auto ksolution = manager.solve ();

        // traverse all the solutions found and determine whether they are all
        // simple or not. In passing, create a new container with all solutions
        // generated but explicitly saying that all solutions are supposedly
        // generated by a manager which is required to generate only simple
        // solution paths.
        bool simple = true;
        auto solutions = ksolution.get_solutions ();
        khs::ksolution_t<npancake_t, std::vector> ssolution (k, start, goal);
        for (const auto& is : solutions) {

            // is this solution simple? if not, the container contains solutions
            // which are guaranteed to be non-simple
            if (simple && !isSimplePath(is.get_solution ())) {
                simple = false;
            }

            // and add this solution to the container
            ssolution += khs::solution_t<npancake_t, std::vector>(1, is.get_solution (), start, goal, 0, 0, 
                                                                  is.get_h0 (), is.get_cost (), is.get_expansions (), is.get_cpu_time (),
                                                                  is.get_nbpaths (), is.get_solver (), true);
        }

        // and now verify that if all solutions found are simple, then doctor
        // returns true, and if at least one solution path contains a loop then
        // doctor returns false
        ASSERT_EQ (simple, ssolution.doctor ());

        // in case that at least one solution contains a loop then ensure that
        // the error reported is correct
        if (!simple) {
            ASSERT_EQ (ssolution.get_error_code (), khs::solution_error::ERR_NON_SIMPLE_PATH);
        }
    }
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
