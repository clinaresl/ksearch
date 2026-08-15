// -*- coding: utf-8 -*-
// TSTbbela.cc
// -----------------------------------------------------------------------------
//
// Started on <mar 01-07-2025 19:43:17.087153559 (1751391797)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Unit tests of the baseline simplistic BELA*
//

#include "../../src/ksearch.h"
#include "../fixtures/TSTbbelafixture.h"

// Checks that the baseline simplistic BELA* search solver can be created. If
// the search algorithm is not invoked, it returns the default values
// ----------------------------------------------------------------------------
TEST_F (bBELAFixture, ExplicitConstructorNPancake) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // Create a manager to run the baseline simplistic BELA* search
        // algorithm to solve a random instance a random number of times. h=0 is
        // given for the original node because no search is performed
        int k = 1 + rand () % MAX_VALUES;
        npancake_t start = randInstance (NB_DISCS);
        npancake_t goal = randInstance (NB_DISCS);
        npancake_t::init ("unit");
        khs::bbela<npancake_t> manager {k, start, goal, false};

        // verify the variant 
        ASSERT_EQ (npancake_t::get_variant (), "unit");

        ASSERT_EQ (manager.get_start (), start);
        ASSERT_EQ (manager.get_goal (), goal);
        ASSERT_EQ (manager.get_k (), k);
        ASSERT_EQ (manager.get_expansions (), 0);

        // Check that the heuristic value is accurately computed. For this it is
        // necessary to init the internal tables providing a variant name
        ASSERT_EQ (manager.get_h0 (), start.h (goal));
    }
}

// Check that centroids can be effectively used for returning optimal and
// suboptimal simple paths in the simple grid
// -----------------------------------------------------------------------------
TEST_F (bBELAFixture, GetPathsSimpleGrid) {

    // create a manager to execute bBELA*
    int k = rand () % MAX_VALUES;
    simplegrid_t start = simplegrid_t (SIMPLE_GRID_LENGTH, 0, 0);
    simplegrid_t goal = simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::bbela<simplegrid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a simple grid
    khs::closed_t<khs::labelednode_t<simplegrid_t>> closed;
    populateClosed<khs::labelednode_t, simplegrid_t> (closed, SIMPLE_GRID_LENGTH);

    // First, paths of length SIMPLE_GRID_LENGTH must be discovered, and this
    // means that backward g-values should be propagated to all nodes in those
    // paths. Thus,use the edge (SIMPLE_GRID_LENGTH-1, 0)->(SIMPLE_GRID_LENGTH,
    // 0) as a centroid of all optimal paths getting to the goal through that
    // edge and set the backward g-value of the end vertex of the centroid
    update_gbvalue<khs::labelednode_t, simplegrid_t> (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0),
                                                      0,
                                                      closed);
    khs::centroid_t z0 = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, 0)),
                                          closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0)),
                                          SIMPLE_GRID_LENGTH);

    // compute all paths reprresented by this centroid
    khs::bucket_t<khs::centroid_t> centroids;
    khs::ksolution_t solutions = manager.get_paths (z0, closed, centroids);

    // and verify there is only one
    ASSERT_EQ (solutions.size (), 1);

    // verify also that every solution is correct ---and this verification also
    // involves verifying whether the path is simple
    for (auto i = 0 ; i < solutions.size () ; i++) {
        auto solution = solutions[i];
        ASSERT_TRUE (solution.get_simple ());
        ASSERT_TRUE (solution.doctor ());
    }

    // Next, consider all true centroids that must have been discovered in the
    // process of computing the optimal path
    ASSERT_EQ (centroids.size (), SIMPLE_GRID_LENGTH-2);
    while (centroids.size () > 0) {

        // pop the next centroid
        auto z = centroids.pop_front ();

        // and verify it represents only one single sub-optimal path
        khs::ksolution_t solutions = manager.get_paths (z, closed, centroids);
        ASSERT_EQ (solutions.size (), 1);
        ASSERT_EQ (z.get_cost (), SIMPLE_GRID_LENGTH+1);

        // verify also that every solution is correct ---and this verification
        // also involves verifying whether the path is simple
        for (auto i = 0 ; i < solutions.size () ; i++) {
            auto solution = solutions[i];
            ASSERT_TRUE (solution.get_simple ());
            ASSERT_TRUE (solution.doctor ());
        }
    }
}

// Check that centroids can be effectively used for returning optimal paths in
// the grid domain
// -----------------------------------------------------------------------------
TEST_F (bBELAFixture, GetOptimalPathsGrid) {

    // create a manager to execute bBELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SIMPLE_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1);
    khs::bbela<grid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::labelednode_t<grid_t>> closed;
    populateClosed<khs::labelednode_t, grid_t> (closed, SIMPLE_GRID_LENGTH);

    // First, paths of length 2*(SIMPLE_GRID_LENGTH-1) must be discovered, and
    // this means that backward g-values should be propagated to all nodes in
    // those paths. Thus,use the last horizontal and vertical edges to get to
    // the goal as centroids of all optimal paths getting to the goal through
    // them
    khs::centroid_t z0 = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-2, SIMPLE_GRID_LENGTH-1)),
                                          closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1)),
                                          2*(SIMPLE_GRID_LENGTH-1));

    // compute all paths reprresented by this centroid
    khs::bucket_t<khs::centroid_t> centroids;
    khs::ksolution_t solutions = manager.get_paths (z0, closed, centroids);

    // and verify there are up to the binomial coefficient of
    // 2*SIMPLE_GRID_LENGTH-3 choose SIMPLE_GRID_LENGTH - 2
    ASSERT_EQ (solutions.size (), binomial_coefficient (2*SIMPLE_GRID_LENGTH-3, SIMPLE_GRID_LENGTH-2));

    // verify also that every solution is correct ---and this verification also
    // involves verifying whether the path is simple
    for (auto i = 0 ; i < solutions.size () ; i++) {
        auto solution = solutions[i];
        ASSERT_TRUE (solution.get_simple ());
        ASSERT_TRUE (solution.doctor ());
    }

    // Repeat the experiment but this time using the last vertical edge to get
    // to the goal
    khs::centroid_t z1 = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-2)),
                                          closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1)),
                                          2*(SIMPLE_GRID_LENGTH-1));

    // compute all paths reprresented by this centroid
    solutions = manager.get_paths (z1, closed, centroids);

    // and verify there are up to the binomial coefficient of
    // 2*SIMPLE_GRID_LENGTH-3 choose SIMPLE_GRID_LENGTH - 2
    ASSERT_EQ (solutions.size (), binomial_coefficient (2*SIMPLE_GRID_LENGTH-3, SIMPLE_GRID_LENGTH-2));

    // verify also that every solution is correct ---and this verification also
    // involves verifying whether the path is simple
    for (auto i = 0 ; i < solutions.size () ; i++) {
        auto solution = solutions[i];
        ASSERT_TRUE (solution.get_simple ());
        ASSERT_TRUE (solution.doctor ());
    }
}

// Check that centroids can be effectively used for returning suboptimal simple
// paths in the grid domain
// -----------------------------------------------------------------------------
TEST_F (bBELAFixture, GetSuboptimalPathsGrid) {

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SIMPLE_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1);
    khs::bbela<grid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::labelednode_t<grid_t>> closed;
    populateClosed<khs::labelednode_t, grid_t> (closed, SIMPLE_GRID_LENGTH);

    // First, paths of length 2*(SIMPLE_GRID_LENGTH-1) must be discovered, and
    // this means that backward g-values should be propagated to all nodes in
    // those paths. Thus,use the last horizontal and vertical edges to get to
    // the goal as centroids of all optimal paths getting to the goal through
    // them and set the backward g-value of the end vertex of the centroid
    update_gbvalue<khs::labelednode_t, grid_t> (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1),
                                                0,
                                                closed);
    khs::centroid_t z0 = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-2, SIMPLE_GRID_LENGTH-1)),
                                          closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1)),
                                          2*(SIMPLE_GRID_LENGTH-1));

    // compute all paths reprresented by this centroid
    khs::bucket_t<khs::centroid_t> centroids;
    khs::ksolution_t solutions = manager.get_paths (z0, closed, centroids);

    // Repeat the experiment but this time using the last vertical edge to get
    // to the goal ---note the backward g-value of the goal has been already set
    khs::centroid_t z1 = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-2)),
                                          closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1)),
                                          2*(SIMPLE_GRID_LENGTH-1));

    // compute all paths reprresented by this centroid
    solutions = manager.get_paths (z1, closed, centroids);

    // While this is not at the core of this unit test, verify that the number
    // of generated centroids is correct, i.e., it is equal to 2*(S-1)^2 +
    // 2*(S-1) -2 = 2*(S-1)*S-2, with S bieng the SIMPLE_GRID_LENGTH. The term
    // "-2" comes from the fact that the goal state is not expanded and thus it
    // can not be the origin of any new centroid (either the one moving
    // horizontally or vertically)
    //
    // Actually, this verification ensures that centroids are discovered indeed
    // only once: The second "get_paths" actually revisits many nodes where a
    // centroid was found but, because the backward g-value is strictly the
    // same, the centroid is not re-discovered again
    ASSERT_EQ (centroids.size (), 2*(SIMPLE_GRID_LENGTH-1)*SIMPLE_GRID_LENGTH-2);

    // every centroid should represent a number of suboptimal paths, process
    // them all
    while (centroids.size () > 0 ) {

        // get the next centroid but this time using a different bucket for the
        // new centroids as we do not want to overwrite the previous ones
        khs::centroid_t z = centroids.pop_front ();
        khs::bucket_t<khs::centroid_t> zs;
        solutions = manager.get_paths (z, closed, zs);

        // discard those centroids that can not produce simple paths because
        // their suffix goes through the prefix or vice versa.
        int x0 = closed[z.get_start ()].get_state ().get_x ();
        int y0 = closed[z.get_start ()].get_state ().get_y ();
        int x1 = closed[z.get_end ()].get_state ().get_x ();
        int y1 = closed[z.get_end ()].get_state ().get_y ();
        if (x0 != 0 and y0 != 0 and
            (x0 != SIMPLE_GRID_LENGTH-1 or x1 != SIMPLE_GRID_LENGTH-1) and
            (y0 != SIMPLE_GRID_LENGTH-1 or y1 != SIMPLE_GRID_LENGTH-1) ) {

            // compute the number of prefixes and suffixes so that the expected
            // number of paths is their product
            size_t nbprefixes;
            size_t nbsuffixes;
            if (x0 == x1) {

                // in case of a vertical centroid, compute the number of
                // prefixes as the number of optimal (thus simple) paths until
                // (x0-1, y0), and compute the suffixes as the number of optimal
                // (thus simple) paths from (x1+1, y1)
                nbprefixes = binomial_coefficient (x0+y0-1, y0);
                nbsuffixes = binomial_coefficient (SIMPLE_GRID_LENGTH-x1-2+SIMPLE_GRID_LENGTH-y1-1, SIMPLE_GRID_LENGTH-x1-2);
            } else if (y0 == y1) {

                // in case of a horizontal centroid, compute the number of
                // prefixes as the number of optimal (thus simple) paths until
                // (x0, y0-1), and compute the suffixes as the number of optimal
                // (thus simple) paths from (x1, y1+1)
                nbprefixes = binomial_coefficient (x0+y0-1, x0);
                nbsuffixes = binomial_coefficient (SIMPLE_GRID_LENGTH-x1-1+SIMPLE_GRID_LENGTH-y1-2, SIMPLE_GRID_LENGTH-y1-2);
            }
            ASSERT_EQ (solutions.size (), nbprefixes*nbsuffixes);
        }
    }
}

// Checks that bBELA0 correctly solves instances where start=goal in the
// n-pancake
// ----------------------------------------------------------------------------
TEST_F (bBELAFixture, NPancakeBruteForceSameSolution) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to run the bBELA* search algorithm to solve a random
        // instance a random number of times. h=0 is given for the original node
        // because no search is performed
        int k = 1 + rand () % MAX_VALUES;
        npancake_t start = randInstance (NB_DISCS);
        khs::bbela<npancake_t> manager {k, start, start};

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

// Checks that bBELA* correctly solves instances where start=goal in the
// n-pancake with consistent heuristics
// ----------------------------------------------------------------------------
TEST_F (bBELAFixture, NPancakeConsistentSameSolution) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to run the BELA* search algorithm to solve a random
        // instance a random number of times. h=0 is given for the original node
        // because no search is performed
        int k = 1 + rand () % MAX_VALUES;
        npancake_t start = randInstance (NB_DISCS);
        npancake_t::init ("unit");
        khs::bbela<npancake_t> manager {k, start, start, false};

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

// Check that bBELA0 correctly finds one single solution between a random
// instance and the identity permutation in the 5-Pancake
// ----------------------------------------------------------------------------
TEST_F (bBELAFixture, NPancakeUnitBruteForceOne) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to find a single solution between a couple of
        // random instances of the 5-Pancake
        int k = 1;
        npancake_t start = randInstance (5);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5};
        while (start == goal) {
            start = randInstance (5);
        }
        npancake_t::init ("unit");
        khs::bbela<npancake_t> manager {k, start, goal};

        // verify the variant and whether the heuristic function is consistent
        // even if no heuristic function is used
        ASSERT_EQ (npancake_t::get_variant (), "unit");

        // and invoke the solver
        auto ksolution = manager.solve ();

        // verify the solution found contains one single simple solution
        ASSERT_EQ (ksolution.size (), k);
        for (auto j = 0 ; j < int (ksolution.size ()); j++) {
            ASSERT_TRUE (ksolution[j].get_simple ());
        }

        // and verify it is correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that bBELA0 correctly finds two single solutions between a random
// instance of the 5-Pancake and the identity permutation
// ----------------------------------------------------------------------------
TEST_F (bBELAFixture, NPancakeUnitBruteForceTwo) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to find two solutions between a couple of random
        // instances of the 5-Pancake which are guaranteed to be different
        int k = 2;
        npancake_t start = randInstance (5);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5};
        while (start == goal) {
            start = randInstance (5);
        }
        npancake_t::init ("unit");
        khs::bbela<npancake_t> manager {k, start, goal};

        // verify the variant and whether the heuristic function is consistent
        // even if no heuristic function is used
        ASSERT_EQ (npancake_t::get_variant (), "unit");

        // and invoke the solver
        auto ksolution = manager.solve ();

        // verify the solution found contains two simple solutions
        ASSERT_EQ (ksolution.size (), k);
        for (auto j = 0 ; j < int (ksolution.size ()); j++) {
            ASSERT_TRUE (ksolution[j].get_simple ());
        }

        // and verify they are correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that bBELA0 correctly finds an arbitrary number of solutions (10 <= k
// <= 20) between a random instance of the 5-Pancake and the identtity
// permutation
// ----------------------------------------------------------------------------
TEST_F (bBELAFixture, NPancakeUnitBruteForceArbitrary) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to find an arbitrary number of solutions between a
        // couple of random instances of the 5-Pancake which are guaranteed to
        // be different
        int k = 10 + (rand () % 11);
        npancake_t start = randInstance (5);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5};
        while (start == goal) {
            start = randInstance (5);
        }
        npancake_t::init ("unit");
        khs::bbela<npancake_t> manager {k, start, goal};

        // verify the variant and whether the heuristic function is consistent
        // even if no heuristic function is used
        ASSERT_EQ (npancake_t::get_variant (), "unit");

        // and invoke the solver
        auto ksolution = manager.solve ();

        // verify the solution found contains k simple solutions
        ASSERT_EQ (ksolution.size (), k);
        for (auto j = 0 ; j < int (ksolution.size ()); j++) {
            ASSERT_TRUE (ksolution[j].get_simple ());
        }

        // and verify they are correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that bBELA* correctly finds one single solution between a random
// instance of the 5-Pancake and the identity permutation using a consistent
// heuristic function
// ----------------------------------------------------------------------------
TEST_F (bBELAFixture, NPancakeUnitConsistentOne) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to find a single solution between a couple of
        // random instances of the 5-Pancake
        int k = 1;
        npancake_t start = randInstance (5);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5};
        while (start == goal) {
            start = randInstance (5);
        }
        npancake_t::init ("unit");
        khs::bbela<npancake_t> manager {k, start, goal, false};

        // verify the variant and whether the heuristic function is consistent
        ASSERT_EQ (npancake_t::get_variant (), "unit");

        // and invoke the solver
        auto ksolution = manager.solve ();

        // verify the solution found contains one single simple solution
        ASSERT_EQ (ksolution.size (), k);
        for (auto j = 0 ; j < int (ksolution.size ()); j++) {
            ASSERT_TRUE (ksolution[j].get_simple ());
        }

        // and verify it is correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that bBELA* correctly finds two single solutions between two instances
// of the 5-Pancake randomly generated and the identity permutation using a
// consistent heuristic function
// ----------------------------------------------------------------------------
TEST_F (bBELAFixture, NPancakeUnitConsistentTwo) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to find two solutions between a couple of random
        // instances of the 5-Pancake which are guaranteed to be different
        int k = 2;
        npancake_t start = randInstance (5);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5};
        while (start == goal) {
            start = randInstance (5);
        }
        npancake_t::init ("unit");
        khs::bbela<npancake_t> manager {k, start, goal, false};

        // verify the variant and whether the heuristic function is consistent
        ASSERT_EQ (npancake_t::get_variant (), "unit");

        // and invoke the solver
        auto ksolution = manager.solve ();

        // verify the solution found contains two simple solutions
        ASSERT_EQ (ksolution.size (), k);
        for (auto j = 0 ; j < int (ksolution.size ()); j++) {
            ASSERT_TRUE (ksolution[j].get_simple ());
        }

        // and verify they are correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that bBELA* correctly finds an arbitrary number of solutions (10 <= k <=
// 20) between a random instance of the 5-Pancake and the identity permutation
// using a consistent heuristic function
// ----------------------------------------------------------------------------
TEST_F (bBELAFixture, NPancakeUnitConsistentArbitrary) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to find an arbitrary number of solutions between
        // a couple of random instances of the 5-Pancake which are
        // guaranteed to be different
        int k = 10 + (rand () % 11);
        npancake_t start = randInstance (5);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5};
        while (start == goal) {
            start = randInstance (5);
        }
        npancake_t::init ("unit");
        khs::bbela<npancake_t> manager {k, start, goal, false};

        // verify the variant and whether the heuristic function is consistent
        ASSERT_EQ (npancake_t::get_variant (), "unit");

        // and invoke the solver
        auto ksolution = manager.solve ();

        // verify the solution found contains k simple solutions
        ASSERT_EQ (ksolution.size (), k);
        for (auto j = 0 ; j < int (ksolution.size ()); j++) {
            ASSERT_TRUE (ksolution[j].get_simple ());
        }

        // and verify they are correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that bBELA0 correctly finds one single solution between a random
// instance of the 5-Pancake and the identity permutation in the heavy-cost
// variant
// ----------------------------------------------------------------------------
TEST_F (bBELAFixture, NPancakeHeavyCostBruteForceOne) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to find a single solution between a couple of
        // random instances of the 5-Pancake
        int k = 1;
        npancake_t start = randInstance (5);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5};
        while (start == goal) {
            start = randInstance (5);
        }
        npancake_t::init ("heavy-cost");
        khs::bbela<npancake_t> manager {k, start, goal};

        // verify the variant and whether the heuristic function is consistent
        // even if no heuristic function is used
        ASSERT_EQ (npancake_t::get_variant (), "heavy-cost");

        // and invoke the solver
        auto ksolution = manager.solve ();

        // verify the solution found contains one single simple solution
        ASSERT_EQ (ksolution.size (), k);
        for (auto j = 0 ; j < int (ksolution.size ()); j++) {
            ASSERT_TRUE (ksolution[j].get_simple ());
        }

        // and verify it is correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that bBELA0 correctly finds two single solutions between a random
// instance of the 5-Pancake and the identity permutation in the heavy-cost
// variant
// ----------------------------------------------------------------------------
TEST_F (bBELAFixture, NPancakeHeavyCostBruteForceTwo) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to find two solutions between a couple of random
        // instances of the 5-Pancake which are guaranteed to be different
        int k = 2;
        npancake_t start = randInstance (5);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5};
        while (start == goal) {
            start = randInstance (5);
        }
        npancake_t::init ("heavy-cost");
        khs::bbela<npancake_t> manager {k, start, goal};

        // verify the variant and whether the heuristic function is consistent
        // even if no heuristic function is used
        ASSERT_EQ (npancake_t::get_variant (), "heavy-cost");

        // and invoke the solver
        auto ksolution = manager.solve ();

        // verify the solution found contains two simple solutions
        ASSERT_EQ (ksolution.size (), k);
        for (auto j = 0 ; j < int (ksolution.size ()); j++) {
            ASSERT_TRUE (ksolution[j].get_simple ());
        }

        // and verify they are correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that bBELA0 correctly finds an arbitrary number of solutions (10 <= k <=
// 20) between a random instance of the 5-Pancake and the identity permutation
// in the heavy-cost variant
// ----------------------------------------------------------------------------
TEST_F (bBELAFixture, NPancakeHeavyCostBruteForceArbitrary) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to find an arbitrary number of solutions between
        // a couple of random instances of the 5-Pancake which are
        // guaranteed to be different
        int k = 10 + (rand () % 11);
        npancake_t start = randInstance (5);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5};
        while (start == goal) {
            start = randInstance (5);
        }
        npancake_t::init ("heavy-cost");
        khs::bbela<npancake_t> manager {k, start, goal};

        // verify the variant and whether the heuristic function is consistent
        // even if no heuristic function is used
        ASSERT_EQ (npancake_t::get_variant (), "heavy-cost");

        // and invoke the solver
        auto ksolution = manager.solve ();

        // verify the solution found contains k simple solutions
        ASSERT_EQ (ksolution.size (), k);
        for (auto j = 0 ; j < int (ksolution.size ()); j++) {
            ASSERT_TRUE (ksolution[j].get_simple ());
        }

        // and verify they are correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that bBELA* correctly finds one single solution between one instance of
// the 5-Pancake randomly generated and the identity permutation in the
// heavy-cost variant using a consistent heuristic function
// ----------------------------------------------------------------------------
TEST_F (bBELAFixture, NPancakeHeavyCostConsistentOne) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to find a single solution between a couple of
        // random instances of the 5-Pancake
        int k = 1;
        npancake_t start = randInstance (5);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5};
        while (start == goal) {
            start = randInstance (5);
        }
        npancake_t::init ("heavy-cost");
        khs::bbela<npancake_t> manager {k, start, goal, false};

        // verify the variant and whether the heuristic function is consistent
        ASSERT_EQ (npancake_t::get_variant (), "heavy-cost");

        // and invoke the solver
        auto ksolution = manager.solve ();

        // verify the solution found contains one single simple solution
        ASSERT_EQ (ksolution.size (), k);
        for (auto j = 0 ; j < int (ksolution.size ()); j++) {
            ASSERT_TRUE (ksolution[j].get_simple ());
        }

        // and verify it is correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that bBELA* correctly finds two single solutions between a random
// instance of the 5-Pancake and the identity permutation in the heavy-cost
// variant using a consistent heuristic function
// ----------------------------------------------------------------------------
TEST_F (bBELAFixture, NPancakeHeavyCostConsistentTwo) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to find two solutions between a couple of random
        // instances of the 5-Pancake which are guaranteed to be different
        int k = 2;
        npancake_t start = randInstance (5);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5};
        while (start == goal) {
            start = randInstance (5);
        }
        npancake_t::init ("heavy-cost");
        khs::bbela<npancake_t> manager {k, start, goal, false};

        // verify the variant and whether the heuristic function is consistent
        ASSERT_EQ (npancake_t::get_variant (), "heavy-cost");

        // and invoke the solver
        auto ksolution = manager.solve ();

        // verify the solution found contains two simple solutions
        ASSERT_EQ (ksolution.size (), k);
        for (auto j = 0 ; j < int (ksolution.size ()); j++) {
            ASSERT_TRUE (ksolution[j].get_simple ());
        }

        // and verify they are correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}

// Check that bBELA* correctly finds an arbitrary number of solutions (10 <= k <=
// 20) between a random instance of the 5-Pancake and the identity permutation
// in the heavy-cost variant when using a consistent heuristic function
// ----------------------------------------------------------------------------
TEST_F (bBELAFixture, NPancakeHeavyCostConsistentArbitrary) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to find an arbitrary number of solutions between a
        // couple of random instances of the 5-Pancake which are guaranteed to
        // be different
        int k = 10 + (rand () % 11);
        npancake_t start = randInstance (5);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5};
        while (start == goal) {
            start = randInstance (5);
        }
        npancake_t::init ("heavy-cost");
        khs::bbela<npancake_t> manager {k, start, goal, false};

        // verify the variant and whether the heuristic function is consistent
        ASSERT_EQ (npancake_t::get_variant (), "heavy-cost");

        // and invoke the solver
        auto ksolution = manager.solve ();

        // verify the solution found contains k simple solutions
        ASSERT_EQ (ksolution.size (), k);
        for (auto j = 0 ; j < int (ksolution.size ()); j++) {
            ASSERT_TRUE (ksolution[j].get_simple ());
        }

        // and verify they are correct
        ASSERT_TRUE (ksolution.doctor ());
    }
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
