// -*- coding: utf-8 -*-
// TSTksolutions_t.cc
// -----------------------------------------------------------------------------
//
// Started on <mié 09-08-2023 14:45:08.869068862 (1691585108)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Unit tests for testing the ksolutions_t class
//

#include<vector>

#include "../fixtures/TSTksolutionsfixture.h"

using namespace std;

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
            khs::ksolution_t<npancake_t, vector> ksolution = randKSolution (k);

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
            khs::ksolution_t<npancake_t, vector> ksolution = randKSolution (k);

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


// Local Variables:
// mode:cpp
// fill-column:80
// End:
