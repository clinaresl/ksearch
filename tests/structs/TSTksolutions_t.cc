// -*- coding: utf-8 -*-
// TSTksolutions_t.cc
// -----------------------------------------------------------------------------
//
// Started on <mié 09-08-2023 14:45:08.869068862 (1691585108)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

//
// Unit tests for testing the ksolutions_t class
//

#include<vector>

#include "../fixtures/TSTksolutionsfixture.h"
#include "../../src/ksearch.h"

// checks that attributes of collections of solutions are properly stored
// ----------------------------------------------------------------------------
TEST_F (KSolutionsFixture, AttributesSolutions) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a null container and verify that the size is null
        khs::ksolutions_t<int> ksolutions;
        ASSERT_EQ (ksolutions.size (), 0);

        // set the domain, variant and solver to random strings and verify they
        // are correct
        auto domain = randString (MIN_NB_CHARS + rand () % MAX_NB_CHARS);
        auto variant = randString (MIN_NB_CHARS + rand () % MAX_NB_CHARS);
        ksolutions.set_domain (domain);
        ksolutions.set_variant (variant);

        // verify these are correct
        ASSERT_EQ (domain, ksolutions.get_domain ());
        ASSERT_EQ (variant, ksolutions.get_variant ());
    }
}

// checks that solutions of the k-shortest path problem of the N-Pancake can be
// added one by one
// ----------------------------------------------------------------------------
TEST_F (KSolutionsFixture, AddSingleNPancakeSolution) {

    for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

        // create a null container
        khs::ksolutions_t<npancake_t> solutions;

        // randomly determine how many different instances of the k-shortest
        // path problem to consider
        int nbInstances = 1 + (rand () % MAX_VALUES);

        // generate random solutions to every k-shortest path problem and add
        // them one by one to the container of solutions
        for (auto i = 0 ; i < nbInstances ; i++) {

            // randomly create a solution to this k shortest-path problem where
            // the parameter k is randomly chosen, but is guaranteed to be at
            // least 1
            int k = 1 + (rand () % MAX_VALUES);
            khs::ksolution_t<npancake_t, std::vector> ksolution = randKSolution (k);

            // Add this solution to the container of solutions
            solutions += ksolution;

            // verify now that the size of the container has increased by one
            ASSERT_EQ (solutions.size (), 1+i);
        }
    }
}

// checks that a set of solutions of the k-shortest path problem of the
// N-Pancake can be added to a container of solutions
// ----------------------------------------------------------------------------
TEST_F (KSolutionsFixture, AddMultipleNPancakeSolution) {

    for (auto i = 0 ; i < NB_TESTS/10000 ; i++) {

        // create a null container
        khs::ksolutions_t<npancake_t> solutions;

        // randomly determine how many different instances of the k-shortest
        // path problem to consider
        int nbInstances = 1 + (rand () % MAX_VALUES);

        // generate random solutions to every k-shortest path problem and add
        // them one by one to the container of solutions
        for (auto i = 0 ; i < nbInstances ; i++) {

            // randomly create a solution to this k shortest-path problem where
            // the parameter k is randomly chosen, but is guaranteed to be at
            // least 1
            int k = 1 + (rand () % MAX_VALUES);
            khs::ksolution_t<npancake_t, std::vector> ksolution = randKSolution (k);

            // Add this solution to the container of solutions
            solutions += ksolution;
        }

        // now, create another container and add all the solutions in the former
        // one
        khs::ksolutions_t<npancake_t> final;
        final += solutions;

        // and verify the size of both containers is the same
        ASSERT_EQ (solutions.size (), final.size ());

        // verify also that solutions are equal and they are stored in the same
        // order
        for (auto iksolution = 0 ; iksolution < final.size () ; iksolution++) {

            // first, verify that these two solutions to the k-shortest path
            // problem contain the sanem number of instances
            ASSERT_EQ (solutions[iksolution].size (), final[iksolution].size ());

            // verify every single solution to this instance of the k-shortest
            // path problem is equal in both containers
            for (auto jsolution = 0 ; jsolution < final[iksolution].size () ; jsolution++) {
                ASSERT_EQ (solutions[iksolution][jsolution], final[iksolution][jsolution]);
            }
        }
    }
}

// Verify that solutions that are equal are correctly classified by doctor
// ----------------------------------------------------------------------------
TEST_F (KSolutionsFixture, DoctorCorrectSolution) {

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

        // and now verify this container contains two sets of solutions and that
        // they do match
        khs::ksolutions_t<npancake_t> solutions;
        solutions += ksolution;
        solutions += ksolution;
        ASSERT_EQ (solutions.size (), 2);
        ASSERT_TRUE (solutions.doctor ());

        // and finally, verify there are no error solutions
        ASSERT_EQ (solutions.get_error_solutions().size (), 0);
    }
}

// Verify that containers with a different number of solutions are correctly
// classified by doctor
// ----------------------------------------------------------------------------
TEST_F (KSolutionsFixture, DoctorDifferentSize) {

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
        khs::ksolutions_t<npancake_t> solutions;
        solutions += ksolution;
        solutions += csolution;
        ASSERT_EQ (solutions.size (), 2);
        ASSERT_FALSE (solutions.doctor ());

        // and finally, verify there is one error solution
        ASSERT_EQ (solutions.get_error_solutions().size (), 1);
    }
}

// Verify that containers with a solution with different cost is correctly
// recognized by doctor
// ----------------------------------------------------------------------------
TEST_F (KSolutionsFixture, DoctorDifferentCost) {

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
        khs::ksolutions_t<npancake_t> solutions;
        solutions += ksolution;
        solutions += csolution;
        ASSERT_EQ (solutions.size (), 2);
        ASSERT_FALSE (solutions.doctor ());

        // and finally, verify there is one error solution
        ASSERT_EQ (solutions.get_error_solutions().size (), 1);
    }
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
