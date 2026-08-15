// -*- coding: utf-8 -*-
// TSTsbela.cc
// -----------------------------------------------------------------------------
//
// Started on <mié 04-06-2025 14:05:32.310750944 (1749038732)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Unit tests of the sBELA* search algorithm
//

#include "../../src/ksearch.h"
#include "../fixtures/TSTsbelafixture.h"

// Check that centroids with null prefixes return only one path in the simple
// grid domain, the start state itself, which contains no pids because the
// suffix computation is not run
// ----------------------------------------------------------------------------
TEST_F (sBELAFixture, NullPrefixSimpleGrid) {

    khs::bucket_t<khs::centroid_t> centroids;

    // First, populate a closed list with the expansions of all nodes in the
    // state space of simple grid
    khs::closed_t<khs::idnode_t<simplegrid_t>> closed;
    populateClosed<khs::idnode_t, simplegrid_t> (closed, SIMPLE_GRID_LENGTH);

    // create a manager to execute sBELA*
    int k = rand () % MAX_VALUES;
    simplegrid_t start = simplegrid_t (SIMPLE_GRID_LENGTH, 0, 0);
    simplegrid_t goal = simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::sbela<simplegrid_t> manager {k, start, goal};

    // even if it is not strictly speaking a centroid, take the edges (0, 0) ->
    // (1, 0) and (0, 0) -> (1, 1) and compute all its optimal prefixes. Note
    // that both edges have the same cost, 1
    khs::centroid_t z0 = khs::centroid_t (closed.find (start),
                                          closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, 1, 0)),
                                          1);
    std::vector<std::unordered_set<size_t>> pids;
    std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z0, centroids, pids);

    // verify there is only one prefix, which consists of the start state itself
    ASSERT_TRUE (prefixes.size () == 1);
    ASSERT_TRUE (prefixes[0].size () == 1);
    ASSERT_TRUE (closed[prefixes[0][0]].get_state () == start);

    // also, because no suffixes have been requested, the only node in the
    // prefixes should have no sid assigned
    ASSERT_EQ (closed[prefixes[0][0]].get_pids ().size (), 0);

    khs::centroid_t z1 = khs::centroid_t (closed.find (start),
                                          closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, 1, 1)),
                                          1);
    prefixes = manager.get_prefixes (closed, z1, centroids, pids);

    // verify there is only one prefi, which consists of the start state itselfx
    ASSERT_TRUE (prefixes.size () == 1);
    ASSERT_TRUE (prefixes[0].size () == 1);
    ASSERT_TRUE (closed[prefixes[0][0]].get_state () == start);

    // also, because no suffixes have been requested, the only node in the
    // prefixes should have no sid assigned
    ASSERT_EQ (closed[prefixes[0][0]].get_pids ().size (), 0);
}

// Check that centroids with null prefixes return only one path in the grid
// domain, the start state itself, which contains no pids because the suffix
// computation is not run
// ----------------------------------------------------------------------------
TEST_F (sBELAFixture, NullPrefixGrid) {

    khs::bucket_t<khs::centroid_t> centroids;

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::idnode_t<grid_t>> closed;
    populateClosed<khs::idnode_t, grid_t> (closed, SIMPLE_GRID_LENGTH);

    // create a manager to execute sBELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SIMPLE_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::sbela<grid_t> manager {k, start, goal};

    // even if it is not strictly speaking a centroid, take the edges (0, 0) ->
    // (1, 0) and (0, 0) -> (0, 1) and compute all its optimal prefixes. Note
    // that both edges have the same cost, 1
    khs::centroid_t z0 = khs::centroid_t (closed.find (start),
                                          closed.find (grid_t (SIMPLE_GRID_LENGTH, 1, 0)),
                                          1);
    std::vector<std::unordered_set<size_t>> pids;
    std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z0, centroids, pids);

    // verify there is only one prefix, which consists of the start state itself
    ASSERT_TRUE (prefixes.size () == 1);
    ASSERT_TRUE (prefixes[0].size () == 1);
    ASSERT_TRUE (closed[prefixes[0][0]].get_state () == start);

    // also, because no suffixes have been requested, the only node in the
    // prefixes should have no sid assigned
    ASSERT_EQ (closed[prefixes[0][0]].get_pids ().size (), 0);

    khs::centroid_t z1 = khs::centroid_t (closed.find (start),
                                          closed.find (grid_t (SIMPLE_GRID_LENGTH, 0, 1)),
                                          1);
    prefixes = manager.get_prefixes (closed, z1, centroids, pids);

    // verify there is only one prefi, which consists of the start state itselfx
    ASSERT_TRUE (prefixes.size () == 1);
    ASSERT_TRUE (prefixes[0].size () == 1);
    ASSERT_TRUE (closed[prefixes[0][0]].get_state () == start);

    // also, because no suffixes have been requested, the only node in the
    // prefixes should have no sid assigned
    ASSERT_EQ (closed[prefixes[0][0]].get_pids ().size (), 0);
}

// Verify that the number of non-null prefixes is correct in the simple grid
// domain, which contain no pids because the suffix computation is not run.
// ----------------------------------------------------------------------------
TEST_F (sBELAFixture, MultiplePrefixSimpleGrid) {

    khs::bucket_t<khs::centroid_t> centroids;

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a simple grid
    khs::closed_t<khs::idnode_t<simplegrid_t>> closed;
    populateClosed<khs::idnode_t, simplegrid_t> (closed, SIMPLE_GRID_LENGTH);

    // create a manager to execute sBELA*
    int k = rand () % MAX_VALUES;
    simplegrid_t start = simplegrid_t (SIMPLE_GRID_LENGTH, 0, 0);
    simplegrid_t goal = simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::sbela<simplegrid_t> manager {k, start, goal};

    // traverse all edges with non-null prefixes in the graph and verify all
    // solutions found

    // First, nodes along the lower line have only one prefix. This is true even
    // for the last edge getting to the goal
    for (auto i = 2 ; i < SIMPLE_GRID_LENGTH ; i++) {

        // note the edge (i, 0) -> (i+1, 0) is a quasi-centroid as it belongs to
        // the optimal path to get to (i+1,0). Nevertheless, get_prefixes should
        // work much the same
        khs::centroid_t z = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i, 0)),
                                             closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, 0)),
                                             1);
        std::vector<std::unordered_set<size_t>> pids;
        std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z, centroids, pids);

        // verify there is only one prefix
        ASSERT_TRUE (prefixes.size () == 1);

        // verify the prefix is correct, i.e., it has the right number of nodes
        // and everyone is a descendant of the previous one
        ASSERT_TRUE (prefixes[0].size () == i+1);
        for (auto j = 0 ; j <= i ; j++) {
            ASSERT_TRUE (closed[prefixes[0][j]].get_state ().get_x () == j);
            ASSERT_TRUE (closed[prefixes[0][j]].get_state ().get_y () == 0);
        }

        // also, because no suffixes have been requested, all prefixes should
        // have no sid assigned
        ASSERT_TRUE (correctPids (pids, prefixes, closed));
    }

    // Next, nodes along the upper line have only one prefix
    for (auto i = 2 ; i < SIMPLE_GRID_LENGTH ; i++) {

        // note the edge (i, 1) -> (i+1, 1) is a quasi-centroid as it belongs to
        // the optimal path to get to (i+1,1). Nevertheless, get_prefixes should
        // work much the same. Also, note that the last vertex (9, 1) goes to
        // the goal in the lower line
        int y = (i == SIMPLE_GRID_LENGTH-1) ? 0 : 1;
        khs::centroid_t z = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i, 1)),
                                             closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, y)),
                                             1);
        std::vector<std::unordered_set<size_t>> pids;
        std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z, centroids, pids);

        // verify there is only one prefix
        ASSERT_TRUE (prefixes.size () == 1);

        // verify the prefix is correct, i.e., it has the right number of nodes
        // and everyone is a descendant of the previous one
        ASSERT_TRUE (prefixes[0].size () == i+1);
        for (auto j = 0 ; j <= i ; j++) {
            ASSERT_TRUE (closed[prefixes[0][j]].get_state ().get_x () == j);

            // note that every path starts in (0,0) even if it then goes through
            // the upper line
            if (j==0) {
                ASSERT_TRUE (closed[prefixes[0][j]].get_state ().get_y () == 0);
            } else {
                ASSERT_TRUE (closed[prefixes[0][j]].get_state ().get_y () == 1);
            }
        }

        // also, because no suffixes have been requested, all prefixes should
        // have no sid assigned
        ASSERT_TRUE (correctPids (pids, prefixes, closed));
    }

    // Next, consider the edges going from the upper line to the lower one.
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH ; i++) {

        // the edge (i, 1) -> (i+1, 0) is a true centroid indeed because its
        // cost is equal to 2, so that paths to (i+1, 0) through this edge are
        // known to be suboptimal.
        khs::centroid_t z {closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i, 1)),
                           closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, 0)),
                           2};
        std::vector<std::unordered_set<size_t>> pids;
        std::vector<std::vector<size_t>> prefixes = manager.get_prefixes(closed, z, centroids, pids);

        // verify there is only one prefix
        ASSERT_TRUE (prefixes.size () == 1);

        // verify the prefix is correct, i.e., it has the right number of nodes
        // and everyone is a descendant of the previous one
        ASSERT_TRUE (prefixes[0].size () == i+1);
        for (auto j = 0 ; j <= i ; j++) {
            ASSERT_TRUE (closed[prefixes[0][j]].get_state ().get_x () == j);

            // note that every path starts in (0,0) even if it then goes through
            // the upper line
            if (j==0) {
                ASSERT_TRUE (closed[prefixes[0][j]].get_state ().get_y () == 0);
            } else {
                ASSERT_TRUE (closed[prefixes[0][j]].get_state ().get_y () == 1);
            }
        }

        // also, because no suffixes have been requested, all prefixes should
        // have no sid assigned
        ASSERT_TRUE (correctPids (pids, prefixes, closed));
    }
}

// Verify that the number of non-null prefixes is correct in the grid domain,
// which contain no pids because the suffix computation is not run.
// ----------------------------------------------------------------------------
TEST_F (sBELAFixture, MultiplePrefixGrid) {

    khs::bucket_t<khs::centroid_t> centroids;

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::idnode_t<grid_t>> closed;
    populateClosed<khs::idnode_t, grid_t> (closed, SIMPLE_GRID_LENGTH);

    // create a manager to execute sBELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SIMPLE_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::sbela<grid_t> manager {k, start, goal};

    // In all the following tests, the overall cost of the centroids is equal to
    // the cost of an optimal path between the left bottom corner to the upper
    // right corner

    // Ensure that all nodes on optimal paths have correct gb-values. This is
    // necessary because the following loop will also try true centroids and for
    // them, to correctly compute the prefixes it is necessary for the optimal
    // gb-values to exist ---or otherwise more prefixes might be generated
    khs::centroid_t opth {closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-2, SIMPLE_GRID_LENGTH-1)),
        closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1)),
        2*(SIMPLE_GRID_LENGTH-1)};
    manager.get_paths (opth, closed, centroids);

    khs::centroid_t optv {closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-2)),
        closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1)),
        2*(SIMPLE_GRID_LENGTH-1)};
    manager.get_paths (optv, closed, centroids);

    // Every node along the left line have only one optimal prefix to get to the
    // start state
    for (auto i = 0 ; i < SIMPLE_GRID_LENGTH -1 ; i++) {
        khs::centroid_t z {closed.find (grid_t (SIMPLE_GRID_LENGTH, 0, i)),
                           closed.find (grid_t (SIMPLE_GRID_LENGTH, 0, i+1)),
                           2*(SIMPLE_GRID_LENGTH-1)};
        std::vector<std::unordered_set<size_t>> pids;
        std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z, centroids, pids);

        // verify there is only one path
        ASSERT_TRUE (prefixes.size () == 1);

        // verify the prefix is correct, i.e., it has the right number of nodes
        // and everyone is a descendant of the previous one
        ASSERT_TRUE (prefixes[0].size () == i+1);
        for (auto j = 0 ; j <= i ; j++) {
            ASSERT_TRUE (closed[prefixes[0][j]].get_state ().get_x () == 0);
            ASSERT_TRUE (closed[prefixes[0][j]].get_state ().get_y () == j);
        }

        // also, because no suffixes have been requested, all prefixes should
        // have no sid assigned
        ASSERT_TRUE (correctPids (pids, prefixes, closed));

        // verify that the only prefix computed has no pids since no suffix
        // computation has been performed.
        ASSERT_EQ (pids.size (), 1);
        ASSERT_EQ (pids[0].size (), 0);
    }

    // Every node along the lower line have only one optimal prefix to get to the
    // start state
    for (auto i = 0 ; i < SIMPLE_GRID_LENGTH -1 ; i++) {
        khs::centroid_t z {closed.find (grid_t (SIMPLE_GRID_LENGTH, i, 0)),
                           closed.find (grid_t (SIMPLE_GRID_LENGTH, i+1, 0)),
                           2*(SIMPLE_GRID_LENGTH-1)};
        std::vector<std::unordered_set<size_t>> pids;
        std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z, centroids, pids);

        // verify there is only one path
        ASSERT_TRUE (prefixes.size () == 1);

        // verify the prefix is correct, i.e., it has the right number of nodes
        // and everyone is a descendant of the previous one
        ASSERT_TRUE (prefixes[0].size () == i+1);
        for (auto j = 0 ; j <= i ; j++) {
            ASSERT_TRUE (closed[prefixes[0][j]].get_state ().get_x () == j);
            ASSERT_TRUE (closed[prefixes[0][j]].get_state ().get_y () == 0);
        }

        // also, because no suffixes have been requested, all prefixes should
        // have no sid assigned
        ASSERT_TRUE (correctPids (pids, prefixes, closed));

        // verify that the only prefix computed has no pids since no suffix
        // computation has been performed.
        ASSERT_EQ (pids.size (), 1);
        ASSERT_EQ (pids[0].size (), 0);
    }

    // now, all nodes, but the ones considered previously have a significant
    // number of optimal paths. This number of optimal prefixes follows a
    // binomial distribution and none of them should have pids because no suffix
    // computation has been performed first.
    std::vector<std::vector<size_t>> prefixes;
    for (auto i = 0 ; i < SIMPLE_GRID_LENGTH ; i++) {
        for (auto j = 0 ; j < SIMPLE_GRID_LENGTH ; j++) {

            // (i, j) -> (i+1, j)
            if (i < SIMPLE_GRID_LENGTH-1) {
                khs::centroid_t east {closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                    closed.find (grid_t (SIMPLE_GRID_LENGTH, i+1, j)),
                    2*(SIMPLE_GRID_LENGTH-1)};
                std::vector<std::unordered_set<size_t>> pids;
                std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, east, centroids, pids);

                // verify the number of optimal paths is equal to the binomial
                // coefficient of (i+j) choose j
                ASSERT_EQ (prefixes.size (), binomial_coefficient (i+j, j));

                // check that every prefix has an unordered set with the pids
                // contained in it.
                ASSERT_TRUE (correctPids (pids, prefixes, closed));

                // verify next that every unordered set contains no pids
                for (const auto& ipid : pids) {
                    ASSERT_EQ (ipid.size (), 0);
                }
            }

            // (i, j) -> (i, j+1)
            if (j < SIMPLE_GRID_LENGTH-1) {
                khs::centroid_t north {closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                    closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j+1)),
                    2*(SIMPLE_GRID_LENGTH-1)};
                std::vector<std::unordered_set<size_t>> pids;
                std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, north, centroids, pids);

                // verify the number of optimal paths is equal to the binomial
                // coefficient of (i+j) choose i
                ASSERT_EQ (prefixes.size (), binomial_coefficient (i+j, i));

                // check that every prefix has an unordered set with the pids
                // contained in it.
                ASSERT_TRUE (correctPids (pids, prefixes, closed));

                // verify next that every unordered set contains no pids
                for (const auto& ipid : pids) {
                    ASSERT_EQ (ipid.size (), 0);
                }
            }

            // (i, j) -> (i-1, j)
            if (i > 0 and j > 0) {
                khs::centroid_t west {closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                    closed.find (grid_t (SIMPLE_GRID_LENGTH, i-1, j)),
                    2*(SIMPLE_GRID_LENGTH-1)};
                std::vector<std::unordered_set<size_t>> pids;
                prefixes = manager.get_prefixes (closed, west, centroids, pids);

                // verify the number of optimal paths is equal to the binomial
                // coefficient of (i+j-1) choose j-1. 1 is substracted because no
                // prefix must contain the end vertex of the centroid
                ASSERT_EQ (prefixes.size (), binomial_coefficient (i+j-1, j-1));

                // check that every prefix has an unordered set with the pids
                // contained in it.
                ASSERT_TRUE (correctPids (pids, prefixes, closed));

                // verify next that every unordered set contains no pids
                for (const auto& ipid : pids) {
                    ASSERT_EQ (ipid.size (), 0);
                }
            }

            // (i, j) -> (i, j-1)
            if (i > 0 and j > 0) {
                khs::centroid_t south {closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                    closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j-1)),
                    2*(SIMPLE_GRID_LENGTH-1)};
                std::vector<std::unordered_set<size_t>> pids;
                prefixes = manager.get_prefixes (closed, south, centroids, pids);

                // verify the number of optimal paths is equal to the binomial
                // coefficient of (i+j-1) choose i-1. 1 is substracted because no
                // prefix must contain the end vertex of the centroid
                ASSERT_EQ (prefixes.size (), binomial_coefficient (i+j-1, i-1));

                // check that every prefix has an unordered set with the pids
                // contained in it.
                ASSERT_TRUE (correctPids (pids, prefixes, closed));

                // verify next that every unordered set contains no pids
                for (const auto& ipid : pids) {
                    ASSERT_EQ (ipid.size (), 0);
                }
            }
        }
    }
}

// Check that quasi-centroids that contain no suffix correctly return only the
// goal state in the simple grid and that repeated invocations to the suffix
// computation correctly update the suffix id.
//
// Also verify that the unwound flag is set to true upon termination on all
// nodes
// -----------------------------------------------------------------------------
TEST_F (sBELAFixture, NullSuffixSimpleGrid) {

    // create a manager to execute sBELA*
    int k = rand () % MAX_VALUES;
    simplegrid_t start = simplegrid_t (SIMPLE_GRID_LENGTH, 0, 0);
    simplegrid_t goal = simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::sbela<simplegrid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a simple grid
    khs::closed_t<khs::idnode_t<simplegrid_t>> closed;
    populateClosed<khs::idnode_t, simplegrid_t> (closed, SIMPLE_GRID_LENGTH);

    // use the edge (SIMPLE_GRID_LENGTH-1, 0)->(SIMPLE_GRID_LENGTH, 0) as a
    // centroid of all optimal paths getting to the goal through that edge and
    // set the backward g-value of the end vertex of the centroid
    update_gbvalue<khs::idnode_t, simplegrid_t> (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0),
                                                 0,
                                                 closed);
    khs::centroid_t z = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, 0)),
                                         closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0)),
                                         SIMPLE_GRID_LENGTH);
    khs::bucket_t<khs::centroid_t> centroids;
    std::vector<std::unordered_set<size_t>> pids;
    std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z, centroids, pids);

    // next, compute the suffixes of the last edge (SIMPLE_GRID_LENGTH-1,
    // 0)->(SIMPLE_GRID_LENGTH, 0) up to 10 times, and check that the goal state
    // is given a unique suffix id each time which is the previous suffix id
    // plus one, starting with 0.
    for (auto i  = 0 ; i < NB_TESTS/1'000 ; i++) {
        std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z);

        // As a result, the backward g-value of the goal should be updated to 0, and
        // it should contain only one
        auto ptr = closed.find (goal);
        ASSERT_EQ (closed[ptr].get_gb ().size (), 1);
        ASSERT_EQ (closed[ptr].get_gb ()[0], 0);

        // verify also that the suffix is null, i.e., it consists of only one path
        // that contains the goal state
        ASSERT_EQ (suffixes.size (), 1);
        ASSERT_EQ (suffixes[0].size (), 1);
        ASSERT_EQ (closed[suffixes[0][0]], goal);

        // finally, browse the entire closed list and verify that only the goal
        // state has been assigned a pid, which has to be strictly equal to i,
        // and that all items have the unwound flag correctly set
        for (auto j = 0 ; j < closed.size () ; j++) {

            if (closed[j].get_pids ().size () > 0) {

                // verify this is the goal state
                ASSERT_EQ (closed[j].get_state (), goal);

                // and that it only contains a single pid which is strictly
                // equal to i
                ASSERT_EQ (closed[j].get_pids ().size (), 1);
                ASSERT_EQ (closed[j].get_pids ()[0], i);
                ASSERT_EQ (closed[j].get_pids ()[0], manager.get_sc0 ());
            }

            ASSERT_TRUE (closed[j].get_unwound ());
        }
    }
}

// Check that quasi-centroids that contain no suffix correctly return only the
// goal state in the grid domain and that repeated invocations to the suffix
// computation correctly update the suffix id.
//
// Also verify that the unwound flag is set to true upon termination on all
// nodes
// -----------------------------------------------------------------------------
TEST_F (sBELAFixture, NullSuffixGrid) {

    // create a manager to execute sBELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SIMPLE_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1);
    khs::sbela<grid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::idnode_t<grid_t>> closed;
    populateClosed<khs::idnode_t, grid_t> (closed, SIMPLE_GRID_LENGTH);

    // use the last horizontal edge leading to the goal as a centroid of all
    // optimal paths getting to the goal through that edge and set the backward
    // g-value of the end vertex of the centroid
    update_gbvalue<khs::idnode_t, grid_t> (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1),
                                           0,
                                           closed);
    khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-2, SIMPLE_GRID_LENGTH-1)),
                                         closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1)),
                                         2*(SIMPLE_GRID_LENGTH-1));
    khs::bucket_t<khs::centroid_t> centroids;
    std::vector<std::unordered_set<size_t>> pids;
    std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z, centroids, pids);

    // next, compute the suffixes of the last horizontal edge getting to the
    // goal up to 10 times, and check that the goal state is given a unique suffix
    // id each time which is the previous suffix id plus one, starting with 0.
    for (auto i  = 0 ; i < NB_TESTS/1'000 ; i++) {
        std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z);

        // As a result, the backward g-value of the goal should be updated to 0, and
        // it should contain only one
        auto ptr = closed.find (goal);
        ASSERT_EQ (closed[ptr].get_gb ().size (), 1);
        ASSERT_EQ (closed[ptr].get_gb ()[0], 0);

        // verify also that the suffix is null, i.e., it consists of only one path
        // that contains the goal state
        ASSERT_EQ (suffixes.size (), 1);
        ASSERT_EQ (suffixes[0].size (), 1);
        ASSERT_EQ (closed[suffixes[0][0]], goal);

        // finally, browse the entire closed list and verify that only the goal
        // state has been assigned a pid, which has to be strictly equal to i,
        // and that all items have the unwound flag correctly set
        for (auto j = 0 ; j < closed.size () ; j++) {

            if (closed[j].get_pids ().size () > 0) {

                // verify this is the goal state
                ASSERT_EQ (closed[j].get_state (), goal);

                // and that it only contains a single pid which is strictly
                // equal to i
                ASSERT_EQ (closed[j].get_pids ().size (), 1);
                ASSERT_EQ (closed[j].get_pids ()[0], i);
                ASSERT_EQ (closed[j].get_pids ()[0], manager.get_sc0 ());
            }

            ASSERT_TRUE (closed[j].get_unwound ());
        }
    }
}

// Check that quasi-centroids that contain a single non-null suffix correctly
// compute it in a simple grid and that repeated invocations to the suffix
// computation correctly update the suffix id.
//
// Also verify that the unwound flag is set to true upon termination on all
// nodes
// -----------------------------------------------------------------------------
TEST_F (sBELAFixture, SingleSuffixSimpleGrid) {

    // create a manager to execute sBELA*
    int k = rand () % MAX_VALUES;
    simplegrid_t start = simplegrid_t (SIMPLE_GRID_LENGTH, 0, 0);
    simplegrid_t goal = simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::sbela<simplegrid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a simple grid
    khs::closed_t<khs::idnode_t<simplegrid_t>> closed;
    populateClosed<khs::idnode_t, simplegrid_t> (closed, SIMPLE_GRID_LENGTH);

    // use the edge (SIMPLE_GRID_LENGTH-1, 0)->(SIMPLE_GRID_LENGTH) as a
    // quasi-centroid of all optimal paths getting to the goal through that edge
    // and set the backward g-value of the end vertex of the centroid
    update_gbvalue<khs::idnode_t, simplegrid_t> (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0),
                                                 0,
                                                 closed);
    khs::centroid_t z0 = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, 0)),
                                          closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0)),
                                          SIMPLE_GRID_LENGTH);
    khs::bucket_t<khs::centroid_t> centroids;
    std::vector<std::unordered_set<size_t>> pids;
    std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z0, centroids, pids);

    // next, compute the suffixes of the last horizontal edge to get to the
    // goal. This step is necessary to update the backward g-value of all nodes
    // to be examined next
    std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z0);

    // next, consider all horizontal edges in the lower line as quasi-centroids
    for (auto i=0 ; i < SIMPLE_GRID_LENGTH ; i++) {

        // consider the edge (i, 0)->(i+1, 0) and compute its suffixes
        update_gbvalue<khs::idnode_t, simplegrid_t> (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, 0),
                                                     SIMPLE_GRID_LENGTH - i - 1,
                                                     closed);
        khs::centroid_t z1 = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i, 0)),
                                              closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, 0)),
                                              SIMPLE_GRID_LENGTH);

        // and compute the suffixes from it
        suffixes = manager.get_suffixes (closed, z1);

        // first, verify that the number of backward g-values of the goal is
        // still 1 and it is equal to 0
        auto ptr = closed.find (goal);
        ASSERT_EQ (closed[ptr].get_gb ().size (), 1);
        ASSERT_EQ (closed[ptr].get_gb ()[0], 0);

        // secondly, verify there is only one suffix which consits of all nodes
        // with x-values in the range [i+1, SIMPLE_GRID_LENGTH] and y=0, and
        // also that they all share the same pid which has to be strictly equal
        // to i+1 (+1 because before entering this loop another get_suffix was
        // computed to propagate all backward g-values and that consumed an
        // identifier for its suffix id)
        ASSERT_EQ (suffixes.size (), 1);
        for (auto j = 0 ; j < suffixes[0].size () ;j++) {
            ASSERT_EQ (closed[suffixes[0][j]].get_state ().get_x (), j+i+1);
            ASSERT_EQ (closed[suffixes[0][j]].get_state ().get_y (), 0);
            ASSERT_EQ (closed[suffixes[0][j]].get_pids ().size (), 1);
            ASSERT_EQ (closed[suffixes[0][j]].get_pids ()[0], i+1);
        }

        // Before leaving, traverse the entire closed list and ensure that all
        // unwound flags are correctly set
        for (auto j = 0 ; j < closed.size () ; j++) {
            ASSERT_TRUE (closed[j].get_unwound ());
        }
    }

    // Also, all the diagonal edges (i, 1)->(i+1, 0) should create a single
    // suffix
    for (auto i=1 ; i < SIMPLE_GRID_LENGTH-1 ; i++) {

        // consider the edge (i, 1)->(i+1, 0) and compute its suffixes
        update_gbvalue<khs::idnode_t, simplegrid_t> (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, 0),
                                                     SIMPLE_GRID_LENGTH - i + 1,
                                                     closed);
        khs::centroid_t z1 = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i, 1)),
                                              closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, 0)),
                                              SIMPLE_GRID_LENGTH+1);

        // and compute the suffixes from it
        suffixes = manager.get_suffixes (closed, z1);

        // first, verify that the number of backward g-values of the goal is
        // still 1 and it is equal to 0
        auto ptr = closed.find (goal);
        ASSERT_EQ (closed[ptr].get_gb ().size (), 1);
        ASSERT_EQ (closed[ptr].get_gb ()[0], 0);

        // secondly, verify there is only one suffix which consits of all nodes
        // with x-values in the range [i+1, SIMPLE_GRID_LENGTH] and y=0. Also,
        // verify they all share the same suffix id which has to be strictly
        // equal to i+10 (+10 because 10 paths have been previously computed
        // using the same solver)
        ASSERT_EQ (suffixes.size (), 1);
        for (auto j = 0 ; j < suffixes[0].size () ;j++) {
            ASSERT_EQ (closed[suffixes[0][j]].get_state ().get_x (), j+i+1);
            ASSERT_EQ (closed[suffixes[0][j]].get_state ().get_y (), 0);
            ASSERT_EQ (closed[suffixes[0][j]].get_pids ().size (), 1);
            ASSERT_EQ (closed[suffixes[0][j]].get_pids ()[0], i+10);
            ASSERT_EQ (closed[suffixes[0][j]].get_pids ()[0], manager.get_sc0 ());
        }

        // Before leaving, traverse the entire closed list and ensure that all
        // unwound flags are correctly set
        for (auto j = 0 ; j < closed.size () ; j++) {
            ASSERT_TRUE (closed[j].get_unwound ());
        }
    }
}

// Check that quasi-centroids that contain a single non-null suffix correctly
// compute it in the grid domain and that repeated invocations to the suffix
// computation correctly update the suffix id.
//
// Also verify that the unwound flag is set to true upon termination on all
// nodes
// -----------------------------------------------------------------------------
TEST_F (sBELAFixture, SingleSuffixGrid) {

    // create a manager to execute sBELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SIMPLE_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1);
    khs::sbela<grid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::idnode_t<grid_t>> closed;
    populateClosed<khs::idnode_t, grid_t> (closed, SIMPLE_GRID_LENGTH);

    // use the last horizontal edge getting to the goal as a quasi-centroid of
    // all optimal paths getting to the goal through that edge and set the
    // backward g-value of the end vertex of the centroid
    update_gbvalue<khs::idnode_t, grid_t> (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1),
                                           0,
                                           closed);
    khs::centroid_t z0 = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-2, SIMPLE_GRID_LENGTH-1)),
                                          closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1)),
                                          2*(SIMPLE_GRID_LENGTH-1));
    khs::bucket_t<khs::centroid_t> centroids;
    std::vector<std::unordered_set<size_t>> pids;
    std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z0, centroids, pids);

    // next, compute the suffixes of the last horizontal edge to get to the
    // goal. This step is necessary to update the backward g-value of the goal.
    std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z0);

    // next, consider all horizontal edges in the upper line as quasi-centroids
    // and verify that all suffixes are correctly computed
    for (auto i=0 ; i < SIMPLE_GRID_LENGTH-2 ; i++) {

        // consider the edge (i, SIMPLE_GRID_LENGTH-1)->(i+1,
        // SIMPLE_GRID_LENGTH-1) and compute its suffixes
        update_gbvalue<khs::idnode_t, grid_t> (grid_t (SIMPLE_GRID_LENGTH, i+1, SIMPLE_GRID_LENGTH-1),
                                               SIMPLE_GRID_LENGTH - i -1,
                                               closed);
        khs::centroid_t z1 = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, i, SIMPLE_GRID_LENGTH-1)),
                                              closed.find (grid_t (SIMPLE_GRID_LENGTH, i+1, SIMPLE_GRID_LENGTH-1)),
                                              2*(SIMPLE_GRID_LENGTH-1));

        // and compute the suffixes from it
        suffixes = manager.get_suffixes (closed, z1);

        // first, verify that the number of backward g-values of the goal is
        // still 1 and it is equal to 0
        auto ptr = closed.find (goal);
        ASSERT_EQ (closed[ptr].get_gb ().size (), 1);
        ASSERT_EQ (closed[ptr].get_gb ()[0], 0);

        // secondly, verify there is only one suffix which consits of all nodes
        // with x-values in the range [i+1, SIMPLE_GRID_LENGTH-1] and
        // y=SIMPLE_GRID_LENGTH-1. Also, verify they all share the same suffix
        // id which has to be strictly equal to i+1 (+1 because before entering
        // this loop one suffix computation was performed)
        ASSERT_EQ (suffixes.size (), 1);
        for (auto j = 0 ; j < suffixes[0].size () ; j++) {
            ASSERT_EQ (closed[suffixes[0][j]].get_state ().get_x (), j+i+1);
            ASSERT_EQ (closed[suffixes[0][j]].get_state ().get_y (), SIMPLE_GRID_LENGTH-1);
            ASSERT_EQ (closed[suffixes[0][j]].get_pids ().size (), 1);
            ASSERT_EQ (closed[suffixes[0][j]].get_pids ()[0], i+1);
            ASSERT_EQ (closed[suffixes[0][j]].get_pids ()[0], manager.get_sc0 ());
        }

        // Before leaving, traverse the entire closed list and ensure that all
        // unwound flags are correctly set
        for (auto j = 0 ; j < closed.size () ; j++) {
            ASSERT_TRUE (closed[j].get_unwound ());
        }
    }

    // use the last vertical edge getting to the goal as a quasi-centroid of all
    // optimal paths getting to the goal through that edge ---note the backward
    // g-value of the end vertex of the centroid has been already set
    khs::centroid_t z1 = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-2)),
                                          closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1)),
                                          2*(SIMPLE_GRID_LENGTH-1));
    pids.clear ();
    prefixes = manager.get_prefixes (closed, z1, centroids, pids);

    // next, compute the suffixes of the last vertical edge to get to the goal.
    // This step is necessary to update the backward g-value of the goal.
    suffixes = manager.get_suffixes (closed, z1);

    // next, consider all vertical edges in the right line as quasi-centroids
    // and verify that all suffixes are correctly computed
    for (auto j=0 ; j < SIMPLE_GRID_LENGTH-2 ; j++) {

        // consider the edge (SIMPLE_GRID_LENGTH-1, j)->(SIMPLE_GRID_LENGTH-1,
        // j+1) and compute its suffixes
        update_gbvalue<khs::idnode_t, grid_t> (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, j+1),
                                               SIMPLE_GRID_LENGTH - j - 2,
                                               closed);
        khs::centroid_t z1 = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, j)),
                                              closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, j+1)),
                                              2*(SIMPLE_GRID_LENGTH-1));

        // and compute the suffixes from it
        suffixes = manager.get_suffixes (closed, z1);

        // first, verify that the number of backward g-values of the goal is
        // still 1 and it is equal to 0
        auto ptr = closed.find (goal);
        ASSERT_EQ (closed[ptr].get_gb ().size (), 1);
        ASSERT_EQ (closed[ptr].get_gb ()[0], 0);

        // secondly, verify there is only one suffix which consits of all nodes
        // with y-values in the range [j+1, SIMPLE_GRID_LENGTH-1] and
        // x=SIMPLE_GRID_LENGTH-1. Also, verify they all share the same suffix
        // id which has to be strictly equal to j+10 (+10 because before
        // entering this loop two suffix computations were performed)
        ASSERT_EQ (suffixes.size (), 1);
        for (auto i = 0 ; i < suffixes[0].size () ; i++) {
            ASSERT_EQ (closed[suffixes[0][i]].get_state ().get_x (), SIMPLE_GRID_LENGTH-1);
            ASSERT_EQ (closed[suffixes[0][i]].get_state ().get_y (), j+i+1);
            ASSERT_EQ (closed[suffixes[0][i]].get_pids ().size (), 1);
            ASSERT_EQ (closed[suffixes[0][i]].get_pids ()[0], j+10);
            ASSERT_EQ (closed[suffixes[0][i]].get_pids ()[0], manager.get_sc0 ());
        }

        // Before leaving, traverse the entire closed list and ensure that all
        // unwound flags are correctly set
        for (auto j = 0 ; j < closed.size () ; j++) {
            ASSERT_TRUE (closed[j].get_unwound ());
        }
    }
}

// Check that quasi-centroids that contain multiple non-null suffixes are
// correctly computed in a simple grid and that repeated invocations to the
// suffix computation correctly update the suffix id.
//
// Also verify that the unwound flag is set to true upon termination on all
// nodes
// -----------------------------------------------------------------------------
TEST_F (sBELAFixture, MultipleSuffixSimpleGrid) {

    // create a manager to execute sBELA*
    int k = rand () % MAX_VALUES;
    simplegrid_t start = simplegrid_t (SIMPLE_GRID_LENGTH, 0, 0);
    simplegrid_t goal = simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::sbela<simplegrid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a simple grid
    khs::closed_t<khs::idnode_t<simplegrid_t>> closed;
    populateClosed<khs::idnode_t, simplegrid_t> (closed, SIMPLE_GRID_LENGTH);

    // First, paths of length SIMPLE_GRID_LENGTH must be discovered, and this
    // means that backward g-values should be propagated to all nodes in those
    // paths. Thus,use the edge (SIMPLE_GRID_LENGTH-1, 0)->(SIMPLE_GRID_LENGTH,
    // 0) as a centroid of all optimal paths getting to the goal through that
    // edge and set the backward g-value of the end vertex of the centroid
    update_gbvalue<khs::idnode_t, simplegrid_t> (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0),
                                                 0,
                                                 closed);
    khs::centroid_t z0 = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, 0)),
                                          closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0)),
                                          SIMPLE_GRID_LENGTH);
    khs::bucket_t<khs::centroid_t> centroids;
    std::vector<std::unordered_set<size_t>> pids;
    std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z0, centroids, pids);
    std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z0);

    // Secondly, to propagate the backward g-values to nodes in the upper line
    // it is necessary now to process a true centroid
    // (SIMPLE_GRID_LENGTH-1,1)-(SIMPLE_GRID_LENGTH,0) ---note that the backward
    // g-value of the end vertex of this centroid is already set
    khs::centroid_t z1 = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, 1)),
                                          closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0)),
                                          SIMPLE_GRID_LENGTH+1);
    prefixes = manager.get_prefixes (closed, z1, centroids, pids);

    // At this point, any of the edges (i,1)->(i+1,1) for i in (0,
    // SIMPLE_GRID_LENGTH-1) should lead to several suffixes
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH-1 ; i++) {

        update_gbvalue<khs::idnode_t, simplegrid_t> (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, 1),
                                                     SIMPLE_GRID_LENGTH - i + 1,
                                                     closed);
        khs::centroid_t z = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i, 1)),
                                             closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, 1)),
                                             1+SIMPLE_GRID_LENGTH);
        khs::bucket_t<khs::centroid_t> centroids;
        std::vector<std::unordered_set<size_t>> pids;
        std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z, centroids, pids);
        std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z);

        // Next, verify the number of suffixes is correct
        ASSERT_EQ (suffixes.size (), SIMPLE_GRID_LENGTH-i-1);

        // Verify that no path contains the start vertex
        auto start = simplegrid_t (SMALL_GRID_LENGTH, i, 1);
        ASSERT_FALSE (hasStart (suffixes, closed.find (start)));

        // and also that every suffix is different from all the others. Thus, it
        // is being verified both that the number of suffixes is correct and,
        // secondly, that they are all different so that they have to be correct
        ASSERT_FALSE (equalPaths (suffixes));

        // and verify also they are correctly identified starting with the
        // first suffix id
        ASSERT_TRUE (isIdentified (suffixes, closed, manager.get_sc0 ()));

        // Also that all paths are simple
        for (auto k = 0 ; k < suffixes.size () ; k++) {
            ASSERT_TRUE (isSimple (suffixes[k]));
        }

        // Before leaving, traverse the entire closed list and ensure that all
        // unwound flags are correctly set
        for (auto j = 0 ; j < closed.size () ; j++) {
            ASSERT_TRUE (closed[j].get_unwound ());
        }
    }
}

// Check that quasi-centroids that contain various non-null suffixes are
// correctly computed in the grid domain and that repeated invocations to the
// suffix computation correctly update the suffix id.
//
// Also verify that the unwound flag is set to true upon termination on all
// nodes
// -----------------------------------------------------------------------------
TEST_F (sBELAFixture, MultipleSuffixGrid) {

    // create a manager to execute sBELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SIMPLE_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1);
    khs::sbela<grid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::idnode_t<grid_t>> closed;
    populateClosed<khs::idnode_t, grid_t> (closed, SIMPLE_GRID_LENGTH);

    // First, paths of length 2*(SIMPLE_GRID_LENGTH-1) must be discovered, and
    // this means that backward g-values should be propagated to all nodes in
    // those paths. Thus, use the last horizontal and vertical edge to get to
    // the goal as quasi-centroids of all optimal paths getting to the goal
    // through those edges and set the backward g-value of the end vertex of the
    // centroid
    update_gbvalue<khs::idnode_t, grid_t> (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1),
                                           0,
                                           closed);
    khs::centroid_t z0 = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-2, SIMPLE_GRID_LENGTH-1)),
                                          closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1)),
                                          2*(SIMPLE_GRID_LENGTH-1));
    khs::bucket_t<khs::centroid_t> centroids;
    std::vector<std::unordered_set<size_t>> pids;
    std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z0, centroids, pids);

    // Compute the suffixes of this quasi-centroid to ensure that the backward
    // g-value of the goal state gets updated in CLOSED
    std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z0);

    khs::centroid_t z1 = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-2)),
                                          closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1)),
                                          2*(SIMPLE_GRID_LENGTH-1));
    prefixes = manager.get_prefixes (closed, z1, centroids, pids);

    // Compute the suffixes of this quasi-centroid to ensure that the backward
    // g-value of the goal state gets updated in CLOSED ---though this is
    // redundand because it was already done above for the other centroid
    suffixes = manager.get_suffixes (closed, z1);

    // Next, verify the number of suffixes of any edge (i, j)->(i+1, j), i in
    // [1, SIMPLE_GRID_LENGTH-3], j in [1, SIMPLE_GRID_LENGTH-2] is correct
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH-2 ; i++) {
        for (auto j = 1 ; j < SIMPLE_GRID_LENGTH-1 ; j++) {

            // create the centroid and compute all its suffixes
            update_gbvalue<khs::idnode_t, grid_t> (grid_t (SIMPLE_GRID_LENGTH, i+1, j),
                                                   2*SIMPLE_GRID_LENGTH - i - j - 3,
                                                   closed);
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SIMPLE_GRID_LENGTH, i+1, j)),
                                                 2*(SIMPLE_GRID_LENGTH-1));
            std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z);

            // Next, verify the number of suffixes is correct, i.e., it is equal
            // to the binomial coefficient (2*SIMPLE_GRID_LENGTH - (i + j + 3))
            // choose (SIMPLE_GRID_LENGTH - i - 2)
            ASSERT_EQ (suffixes.size (), binomial_coefficient (2*SIMPLE_GRID_LENGTH - (i + j + 3),
                                                               SIMPLE_GRID_LENGTH - i - 2));

            // Verify that no path contains the start vertex
            auto start = grid_t (SMALL_GRID_LENGTH, i, j);
            ASSERT_FALSE (hasStart (suffixes, closed.find (start)));

            // and also that every suffix is different from all the others. Thus, it
            // is being verified both that the number of suffixes is correct and,
            // secondly, that they are all different so that they have to be correct
            ASSERT_FALSE (equalPaths (suffixes));

            // and verify also they are correctly identified starting with the
            // first suffix id
            ASSERT_TRUE (isIdentified (suffixes, closed, manager.get_sc0 ()));

            // Also that all paths are simple
            for (auto k = 0 ; k < suffixes.size () ; k++) {
                ASSERT_TRUE (isSimple (suffixes[k]));
            }

            // Before leaving, traverse the entire closed list and ensure that all
            // unwound flags are correctly set
            for (auto j = 0 ; j < closed.size () ; j++) {
                ASSERT_TRUE (closed[j].get_unwound ());
            }
        }
    }

    // Well, repeat again but this time considering centroids (i, j)->(i, j+1)
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH-1 ; i++) {
        for (auto j = 1 ; j < SIMPLE_GRID_LENGTH-2 ; j++) {

            // create the centroid and compute all its suffixes
            update_gbvalue<khs::idnode_t, grid_t> (grid_t (SIMPLE_GRID_LENGTH, i, j+1),
                                                   2*SIMPLE_GRID_LENGTH - i - j - 3,
                                                   closed);
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j+1)),
                                                 2*(SIMPLE_GRID_LENGTH-1));
            std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z);

            // Next, verify the number of suffixes is correct, i.e., it is equal
            // to the binomial coefficient (2*SIMPLE_GRID_LENGTH - (i + j + 3))
            // choose (SIMPLE_GRID_LENGTH - i - 2)
            ASSERT_EQ (suffixes.size (), binomial_coefficient (2*SIMPLE_GRID_LENGTH - (i + j + 3),
                                                               SIMPLE_GRID_LENGTH - j - 2));

            // Verify that no path contains the start vertex
            auto start = grid_t (SMALL_GRID_LENGTH, i, j);
            ASSERT_FALSE (hasStart (suffixes, closed.find (start)));

            // and also that every suffix is different from all the others. Thus, it
            // is being verified both that the number of suffixes is correct and,
            // secondly, that they are all different so that they have to be correct
            ASSERT_FALSE (equalPaths (suffixes));

            // and verify also they are correctly identified starting with the
            // first suffix id
            ASSERT_TRUE (isIdentified (suffixes, closed, manager.get_sc0 ()));

            // Also that all paths are simple
            for (auto k = 0 ; k < suffixes.size () ; k++) {
                ASSERT_TRUE (isSimple (suffixes[k]));
            }

            // Before leaving, traverse the entire closed list and ensure that all
            // unwound flags are correctly set
            for (auto j = 0 ; j < closed.size () ; j++) {
                ASSERT_TRUE (closed[j].get_unwound ());
            }
        }
    }
}

// Check that centroids that contain various non-null suffixes to be used for
// computing suboptimal solutions with a difference with the optimal cost equal
// to 2, are correctly computed in the grid domain and that repeated invocations
// to the suffix computation correctly update the suffix id.
//
// Also verify that the unwound flag is set to true upon termination on all
// nodes
// -----------------------------------------------------------------------------
TEST_F (sBELAFixture, MultipleSuffixSuboptimal2Grid) {

    // create a manager to execute sBELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SMALL_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, SMALL_GRID_LENGTH-1);
    khs::sbela<grid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::idnode_t<grid_t>> closed;
    populateClosed<khs::idnode_t, grid_t> (closed, SMALL_GRID_LENGTH);

    // First, paths of length 2*(SMALL_GRID_LENGTH-1) must be discovered, and
    // this means that backward g-values should be propagated to all nodes in
    // those paths. Thus, use the last horizontal and vertical edge to get to
    // the goal as quasi-centroids of all optimal paths getting to the goal
    // through those edges and set the backward g-value of the end vertex of the
    // centroid
    update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, SMALL_GRID_LENGTH-1),
                                           0,
                                           closed);
    khs::centroid_t z0 = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-2, SMALL_GRID_LENGTH-1)),
                                          closed.find (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, SMALL_GRID_LENGTH-1)),
                                          2*(SMALL_GRID_LENGTH-1));
    khs::bucket_t<khs::centroid_t> centroids;
    std::vector<std::unordered_set<size_t>> pids;
    std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z0, centroids, pids);

    khs::centroid_t z1 = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, SMALL_GRID_LENGTH-2)),
                                          closed.find (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, SMALL_GRID_LENGTH-1)),
                                          2*(SMALL_GRID_LENGTH-1));
    prefixes = manager.get_prefixes (closed, z1, centroids, pids);

    // Next, verify the number of suffixes in the top row is zero. For this,
    // consider all true centroids (i, SMALL_GRID_LENGTH-1) -> (i-1,
    // SMALL_GRID_LENGTH-1)
    for (auto i = 1 ; i < SMALL_GRID_LENGTH ; i++) {

        // create the centroid and compute all its suffixes
        update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, i-1, SMALL_GRID_LENGTH-1),
                                               SMALL_GRID_LENGTH-i,
                                               closed);
        khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, i, SMALL_GRID_LENGTH-1)),
                                             closed.find (grid_t (SMALL_GRID_LENGTH, i-1, SMALL_GRID_LENGTH-1)),
                                             2*SMALL_GRID_LENGTH);

        std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z);

        // The only suffix that would be generated with this centroid must
        // contain the start vertex of the centroid. That would result in a loop
        // if prefixes are added. This condition is already verified in the
        // computation of simple suffixes and thus, verify that no suffix at all
        // has been generated
        ASSERT_EQ (suffixes.size (), 0);

        // Before leaving, traverse the entire closed list and ensure that all
        // unwound flags are correctly set
        for (auto k = 0 ; k < closed.size () ; k++) {
            ASSERT_TRUE (closed[k].get_unwound ());
        }
    }

    // Now, verify that the suffixes of all the other horizontal centroids are
    // correctly computed
    for (auto i = 1 ; i < SMALL_GRID_LENGTH ; i++) {
        for (auto j = 0 ; j < SMALL_GRID_LENGTH-1 ; j++) {

            // create the centroid and compute its suffixes
            update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, i-1, j),
                                                   2*SMALL_GRID_LENGTH - i - j - 1,
                                                   closed);
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SMALL_GRID_LENGTH, i-1, j)),
                                                 2*SMALL_GRID_LENGTH);
            std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z);

            // The number of simple suffixes equals the binomial coefficient
            // (2N-i-j-2, N-i) because: first, all suffixes using the starting
            // vertex of the centroid are automatically dismished by the suffix
            // computation; second, the number of simple suffixes equals the
            // number of optimal paths from the vertex immediately above the
            // ending vertex of this centroid
            ASSERT_EQ (suffixes.size (), binomial_coefficient (2*SMALL_GRID_LENGTH-i-j-2, SMALL_GRID_LENGTH-i));

            // Verify that no path contains the start vertex
            auto start = grid_t (SMALL_GRID_LENGTH, i, j);
            ASSERT_FALSE (hasStart (suffixes, closed.find (start)));

            // Verify that all suffixes are different
            ASSERT_FALSE (equalPaths (suffixes));

            // and verify also they are correctly identified starting with the
            // first suffix id
            ASSERT_TRUE (isIdentified (suffixes, closed, manager.get_sc0 ()));

            // Also that all paths are simple
            for (auto k = 0 ; k < suffixes.size () ; k++) {
                ASSERT_TRUE (isSimple (suffixes[k]));
            }

            // Before leaving, traverse the entire closed list and ensure that all
            // unwound flags are correctly set
            for (auto k = 0 ; k < closed.size () ; k++) {
                ASSERT_TRUE (closed[k].get_unwound ());
            }
        }
    }

    // Next, verify the number of suffixes in the right column is zero. For this,
    // consider all true centroids (SMALL_GRID_LENGTH-1, j) ->
    // (SMALL_GRID_LENGTH-1, j-1)
    for (auto j = 1 ; j < SMALL_GRID_LENGTH ; j++) {

        // create the centroid and compute all its suffixes
        update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, j-1),
                                               SMALL_GRID_LENGTH-j,
                                               closed);
        khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, j)),
                                             closed.find (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, j-1)),
                                             2*SMALL_GRID_LENGTH);
        std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z);

        // It turns out that the only suffixes that use this centroid actually
        // use the starting vertex of the centroid and thus, they must be
        // discarded automatically by the suffix computation
        ASSERT_EQ (suffixes.size (), 0);

        // Before leaving, traverse the entire closed list and ensure that all
        // unwound flags are correctly set
        for (auto k = 0 ; k < closed.size () ; k++) {
            ASSERT_TRUE (closed[k].get_unwound ());
        }
    }

    // Now, verify that the suffixes of all the other vertical centroids are
    // correctly computed
    for (auto i = 0 ; i < SMALL_GRID_LENGTH - 1 ; i++) {
        for (auto j = 1 ; j < SMALL_GRID_LENGTH ; j++) {

            // create the centroid and compute its suffixes
            update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, i, j-1),
                                                   2*SMALL_GRID_LENGTH - i - j -1,
                                                   closed);
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SMALL_GRID_LENGTH, i, j-1)),
                                                 2*SMALL_GRID_LENGTH);
            std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z);

            // verify the number of simple suffixes generated. It should be
            // equal to the binomial coefficient of (2*N-i-j-1) over (N-j)
            ASSERT_EQ (suffixes.size (), binomial_coefficient (2*SMALL_GRID_LENGTH-i-j-2, SMALL_GRID_LENGTH-j));

            // Verify that no path contains the start vertex
            auto start = grid_t (SMALL_GRID_LENGTH, i, j);
            ASSERT_FALSE (hasStart (suffixes, closed.find (start)));

            // Verify that all suffixes are different
            ASSERT_FALSE (equalPaths (suffixes));

            // and verify also they are correctly identified starting with the
            // first suffix id
            ASSERT_TRUE (isIdentified (suffixes, closed, manager.get_sc0 ()));

            // Also that all paths are simple
            for (auto k = 0 ; k < suffixes.size () ; k++) {
                ASSERT_TRUE (isSimple (suffixes[k]));
            }

            // Before leaving, traverse the entire closed list and ensure that all
            // unwound flags are correctly set
            for (auto k = 0 ; k < closed.size () ; k++) {
                ASSERT_TRUE (closed[k].get_unwound ());
            }
        }
    }
}

// Check that centroids that contain various non-null suffixes to be used for
// computing suboptimal solutions with a difference with the optimal cost equal
// to 4, are correctly computed in the grid domain and that repeated invocations
// to the suffix computation correctly update the suffix id. Note that with a
// difference of cost with the optimal solution equal to 4, there must be a
// number of suffixes that have to be discarded because they are not simple.
//
// Also verify that the unwound flag is set to true upon termination on all
// nodes
// -----------------------------------------------------------------------------
TEST_F (sBELAFixture, MultipleSuffixSuboptimal4Grid) {

    // create a manager to execute sBELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SMALL_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, SMALL_GRID_LENGTH-1);
    khs::sbela<grid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::idnode_t<grid_t>> closed;
    populateClosed<khs::idnode_t, grid_t> (closed, SMALL_GRID_LENGTH);

    // First, paths of length 2*(SMALL_GRID_LENGTH-1) must be discovered, and
    // this means that backward g-values should be propagated to all nodes in
    // those paths. Thus, use the last horizontal and vertical edge to get to
    // the goal as quasi-centroids of all optimal paths getting to the goal
    // through those edges and set the backward g-value of the end vertex of the
    // centroid
    update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, SMALL_GRID_LENGTH-1),
                                           0,
                                           closed);
    khs::centroid_t z0 = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-2, SMALL_GRID_LENGTH-1)),
                                          closed.find (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, SMALL_GRID_LENGTH-1)),
                                          2*(SMALL_GRID_LENGTH-1));
    khs::bucket_t<khs::centroid_t> centroids;
    std::vector<std::unordered_set<size_t>> pids;
    std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z0, centroids, pids);

    khs::centroid_t z1 = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, SMALL_GRID_LENGTH-2)),
                                          closed.find (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, SMALL_GRID_LENGTH-1)),
                                          2*(SMALL_GRID_LENGTH-1));
    prefixes = manager.get_prefixes (closed, z1, centroids, pids);

    // In order to be able to generate suboptimal paths with a difference of
    // cost with the optimal solution equal to 4 it is necessary to propagate
    // more gb-values. For this, use all vertical and horizontal centroids and
    // compute its prefixes. As a result, every node (but the goal node) will
    // have two gb-values with the same parity.

    // First, compute the prefixes of all horizontal centroids
    for (auto i = 1 ; i < SMALL_GRID_LENGTH ; i++) {
        for (auto j = 0 ; j < SMALL_GRID_LENGTH-1 ; j++) {

            // create the centroid and compute its suffixes
            update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, i-1, j),
                                                   2*SMALL_GRID_LENGTH - i - j -1,
                                                   closed);
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SMALL_GRID_LENGTH, i-1, j)),
                                                 2*SMALL_GRID_LENGTH);
            std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z, centroids, pids);
        }
    }

    // Second, compute the prefixes of all vertical centroids
    for (auto i = 0 ; i < SMALL_GRID_LENGTH - 1 ; i++) {
        for (auto j = 1 ; j < SMALL_GRID_LENGTH ; j++) {

            // create the centroid and compute its suffixes
            update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, i, j-1),
                                                   2*SMALL_GRID_LENGTH - i - j - 1,
                                                   closed);
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SMALL_GRID_LENGTH, i, j-1)),
                                                 2*SMALL_GRID_LENGTH);
            std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z, centroids, pids);
        }
    }

    // Everything's almost in place! Still, many nodes have not a backward
    // g-value which allows the computation the computation of all suffixes with
    // a cost equal to 2*(1+SMALL_GRID_LENGTH), but many will be properly
    // computed

    // First, centroids (i, j) -> (i-1, j) where the top horizontal centroids
    // are disregarded because another unit tests already proved that they
    // generate 0 suffixes
    for (auto i = 1 ; i < SMALL_GRID_LENGTH; i++) {
        for (auto j = 0 ; j < SMALL_GRID_LENGTH-1 ; j++) {

            // create the centroid and compute its suffixes
            update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, i-1, j),
                                                   2*SMALL_GRID_LENGTH -i - j + 1,
                                                   closed);
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SMALL_GRID_LENGTH, i-1, j)),
                                                 2*(1+SMALL_GRID_LENGTH));
            std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z);

            // I'm not so good as for being able to compute the number of simple
            // suffixes. Thus, the (other) usual verificates are carried out.
            // verify at least that the number of suffixes is congruent with the
            // suffix counter
            ASSERT_EQ (suffixes.size (), manager.get_sc () - manager.get_sc0 ());

            // Verify that no path contains the start vertex
            auto start = grid_t (SMALL_GRID_LENGTH, i, j);
            ASSERT_FALSE (hasStart (suffixes, closed.find (start)));

            // Verify that all suffixes are different
            ASSERT_FALSE (equalPaths (suffixes));

            // and verify also they are correctly identified starting with the
            // first suffix id
            ASSERT_TRUE (isIdentified (suffixes, closed, manager.get_sc0 ()));

            // Also that all paths are simple
            for (auto k = 0 ; k < suffixes.size () ; k++) {
                ASSERT_TRUE (isSimple (suffixes[k]));
            }

            // Before leaving, traverse the entire closed list and ensure that all
            // unwound flags are correctly set
            for (auto k = 0 ; k < closed.size () ; k++) {
                ASSERT_TRUE (closed[k].get_unwound ());
            }
        }
    }

    // Second, centroids (i, j) -> (i, j-.1) where the left vertical centroids
    // are disregarded because another unit tests already proved that they
    // generate 0 suffixes
    for (auto i = 0 ; i < SMALL_GRID_LENGTH-1; i++) {
        for (auto j = 1 ; j < SMALL_GRID_LENGTH ; j++) {

            // create the centroid and compute its suffixes
            update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, i, j-1),
                                                   2*SMALL_GRID_LENGTH -i - j + 1,
                                                   closed);
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SMALL_GRID_LENGTH, i, j-1)),
                                                 2*(1+SMALL_GRID_LENGTH));
            std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z);

            // I'm not so good as for being able to compute the number of simple
            // suffixes. Thus, the (other) usual verificates are carried out.
            // verify at least that the number of suffixes is congruent with the
            // suffix counter
            ASSERT_EQ (suffixes.size (), manager.get_sc () - manager.get_sc0 ());

            // Verify that no path contains the start vertex
            start = grid_t (SMALL_GRID_LENGTH, i, j);
            ASSERT_FALSE (hasStart (suffixes, closed.find (start)));

            // Verify that all suffixes are different
            ASSERT_FALSE (equalPaths (suffixes));

            // and verify also they are correctly identified starting with the
            // first suffix id
            ASSERT_TRUE (isIdentified (suffixes, closed, manager.get_sc0 ()));

            // Also that all paths are simple
            for (auto k = 0 ; k < suffixes.size () ; k++) {
                ASSERT_TRUE (isSimple (suffixes[k]));
            }

            // Before leaving, traverse the entire closed list and ensure that all
            // unwound flags are correctly set
            for (auto k = 0 ; k < closed.size () ; k++) {
                ASSERT_TRUE (closed[k].get_unwound ());
            }
        }
    }
}

// Check that the computation of prefixes following the suffix computation of
// centroids delivers no suffix ids in the prefixes when computing optimal
// paths.
// -----------------------------------------------------------------------------
TEST_F (sBELAFixture, MultiplePrefixSuffixGrid) {

    // create a manager to execute sBELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SIMPLE_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1);
    khs::sbela<grid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::idnode_t<grid_t>> closed;
    populateClosed<khs::idnode_t, grid_t> (closed, SIMPLE_GRID_LENGTH);

    // First, paths of length 2*(SIMPLE_GRID_LENGTH-1) must be discovered, and
    // this means that backward g-values should be propagated to all nodes in
    // those paths. Thus, use the last horizontal and vertical edge to get to
    // the goal as quasi-centroids of all optimal paths getting to the goal
    // through those edges and set the backward g-value of the end vertex of the
    // centroid
    update_gbvalue<khs::idnode_t, grid_t> (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1),
                                           0,
                                           closed);
    khs::centroid_t z0 = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-2, SIMPLE_GRID_LENGTH-1)),
                                          closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1)),
                                          2*(SIMPLE_GRID_LENGTH-1));
    khs::bucket_t<khs::centroid_t> centroids;
    std::vector<std::unordered_set<size_t>> pids;
    std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z0, centroids, pids);

    khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-2)),
                                         closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1)),
                                         2*(SIMPLE_GRID_LENGTH-1));
    prefixes = manager.get_prefixes (closed, z, centroids, pids);

    // check that all prefixes computed for each centroid (i, j)->(i+1, j) are
    // valid and, in particular, that they contain no suffix ids.
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH-2 ; i++) {
        for (auto j = 1 ; j < SIMPLE_GRID_LENGTH-1 ; j++) {

            // create the centroid and compute all its suffixes and prefixes
            update_gbvalue<khs::idnode_t, grid_t> (grid_t (SIMPLE_GRID_LENGTH, i+1, j),
                                                   2*SIMPLE_GRID_LENGTH - i - j - 1,
                                                   closed);
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SIMPLE_GRID_LENGTH, i+1, j)),
                                                 2*(SIMPLE_GRID_LENGTH-1));
            std::vector<std::unordered_set<size_t>> pids;
            std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z);
            std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z, centroids, pids);

            // Other unit tests already verified that the prefixes and suffixes
            // are correct. We want now to verify that the unordered sets of the
            // prefixes contain no valid pid since it is impossible to have
            // loops. Note that nodes in closed might have pids already assigned
            // so that we give the value of the first suffix counter to compute
            // only those which are valid from closed.
            ASSERT_TRUE (correctPids (pids, prefixes, closed, manager.get_sc0 ()));
            for (const auto& ipid: pids) {
                ASSERT_EQ (ipid.size (), 0);
            }
        }
    }

    // Well, repeat again but this time considering centroids (i, j)->(i, j+1)
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH-1 ; i++) {
        for (auto j = 1 ; j < SIMPLE_GRID_LENGTH-2 ; j++) {

            // create the centroid and compute all its suffixes and prefixes
            update_gbvalue<khs::idnode_t, grid_t> (grid_t (SIMPLE_GRID_LENGTH, i, j+1),
                                                   2*SIMPLE_GRID_LENGTH - i - j - 1,
                                                   closed);
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j+1)),
                                                 2*(SIMPLE_GRID_LENGTH-1));
            std::vector<std::unordered_set<size_t>> pids;
            std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z);
            std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z, centroids, pids);

            // and verify the unordered sets computed in the prefixes
            ASSERT_TRUE (correctPids (pids, prefixes, closed, manager.get_sc0 ()));
            for (const auto& ipid: pids) {
                ASSERT_EQ (ipid.size (), 0);
            }
        }
    }
}

// Check the suffix ids computed by the prefix construction when considering a
// difference with the cost of the optimal paths equal to 2:
//
// 1. When using quasi-centroids no suffix ids are returned along with the
//    prefixes.
//
//    In fact, when computing these sort of suboptimal paths in a grid without
//    obstacles, it is impossible for the prefixes and suffixes to intersect,
//    and thus the prefixes should not contain any suffix ids.
//
// 2. When using true centroids no suffix ids are returned along with the
//    prefixes.
//
//    The reason is because the only prefixes that might contain some suffix ids
//    must use the ending vertex of the centroid, and the prefix computation
//    explicitly avoids that.
//    -----------------------------------------------------------------------------
TEST_F (sBELAFixture, MultiplePrefixSuffixSuboptimal2Grid) {

    // create a manager to execute sBELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SMALL_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, SMALL_GRID_LENGTH-1);
    khs::sbela<grid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::idnode_t<grid_t>> closed;
    populateClosed<khs::idnode_t, grid_t> (closed, SMALL_GRID_LENGTH);

    // First, paths of length 2*(SMALL_GRID_LENGTH-1) must be discovered, and
    // this means that backward g-values should be propagated to all nodes in
    // those paths. Thus, use the last horizontal and vertical edge to get to
    // the goal as quasi-centroids of all optimal paths getting to the goal
    // through those edges and set the backward g-value of the end vertex of the
    // centroid
    update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, SMALL_GRID_LENGTH-1),
                                           0,
                                           closed);
    khs::centroid_t z0 = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-2, SMALL_GRID_LENGTH-1)),
                                          closed.find (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, SMALL_GRID_LENGTH-1)),
                                          2*(SMALL_GRID_LENGTH-1));
    khs::bucket_t<khs::centroid_t> centroids;
    std::vector<std::unordered_set<size_t>> pids;
    std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z0, centroids, pids);

    khs::centroid_t z1 = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, SMALL_GRID_LENGTH-2)),
                                          closed.find (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, SMALL_GRID_LENGTH-1)),
                                          2*(SMALL_GRID_LENGTH-1));
    prefixes = manager.get_prefixes (closed, z1, centroids, pids);

    // check that all prefixes computed for each quasi-centroid (i, j)->(i+1, j)
    // are valid and, in particular, that they contain no suffix ids.
    for (auto i = 0 ; i < SMALL_GRID_LENGTH - 1; i++) {
        for (auto j = 0 ; j < SMALL_GRID_LENGTH ; j++) {

            // create the centroid and compute its suffixes and prefixes
            update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, i+1, j),
                                                   2*SMALL_GRID_LENGTH - i - j - 1,
                                                   closed);
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SMALL_GRID_LENGTH, i+1, j)),
                                                 2*SMALL_GRID_LENGTH);
            std::vector<std::unordered_set<size_t>> pids;
            std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z);
            std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z, centroids, pids);

            // Other unit tests already verified that the prefixes and suffixes
            // are correct. We want now to verify that the unordered sets of the
            // prefixes contain no valid pid since it is impossible to have
            // loops. Note that nodes in closed might have pids already assigned
            // so that we give the value of the first suffix counter to compute
            // only those which are valid from closed.
            ASSERT_TRUE (correctPids (pids, prefixes, closed, manager.get_sc0 ()));
            for (const auto& ipid: pids) {
                ASSERT_EQ (ipid.size (), 0);
            }
        }
    }

    // check that all prefixes computed for each centroid (i, j)->(i-1, j) are
    // valid and, in particular, that they contain no suffix ids.
    for (auto i = 1 ; i < SMALL_GRID_LENGTH ; i++) {
        for (auto j = 0 ; j < SMALL_GRID_LENGTH ; j++) {

            // create the centroid and compute its suffixes and prefixes
            update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, i-1, j),
                                                   2*SMALL_GRID_LENGTH - i - j + 1,
                                                   closed);
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SMALL_GRID_LENGTH, i-1, j)),
                                                 2*SMALL_GRID_LENGTH);
            std::vector<std::unordered_set<size_t>> pids;
            std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z);
            std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z, centroids, pids);

            // Other unit tests already verified that the prefixes and suffixes
            // are correct. We want now to verify that the unordered sets of the
            // prefixes contain no valid pid since it is impossible to have
            // loops. Note that nodes in closed might have pids already assigned
            // so that we give the value of the first suffix counter to compute
            // only those which are valid from closed.
            ASSERT_TRUE (correctPids (pids, prefixes, closed, manager.get_sc0 ()));
            for (const auto& ipid: pids) {
                ASSERT_EQ (ipid.size (), 0);
            }
        }
    }

    // check that all prefixes computed for each quasi-centroid (i, j)->(i, j+1)
    // are valid and, in particular, that they contain no suffix ids.
    for (auto i = 0 ; i < SMALL_GRID_LENGTH ; i++) {
        for (auto j = 0 ; j < SMALL_GRID_LENGTH - 1 ; j++) {

            // create the centroid and compute its suffixes
            update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, i, j+1),
                                                   2*SMALL_GRID_LENGTH - i - j - 1,
                                                   closed);
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SMALL_GRID_LENGTH, i, j+1)),
                                                 2*SMALL_GRID_LENGTH);
            std::vector<std::unordered_set<size_t>> pids;
            std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z);
            std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z, centroids, pids);

            // and verify the unordered sets computed in the prefixes
            ASSERT_TRUE (correctPids (pids, prefixes, closed, manager.get_sc0 ()));
            for (const auto& ipid: pids) {
                ASSERT_EQ (ipid.size (), 0);
            }
        }
    }


    // check that all prefixes computed for each centroid (i, j)->(i, j-1) are
    // valid and, in particular, that they contain no suffix ids.
    for (auto i = 0 ; i < SMALL_GRID_LENGTH ; i++) {
        for (auto j = 1 ; j < SMALL_GRID_LENGTH ; j++) {

            // create the centroid and compute its suffixes
            update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, i, j-1),
                                                   2*SMALL_GRID_LENGTH - i - j + 1,
                                                   closed);
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SMALL_GRID_LENGTH, i, j-1)),
                                                 2*SMALL_GRID_LENGTH);
            std::vector<std::unordered_set<size_t>> pids;
            std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z);
            std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z, centroids, pids);

            // and verify the unordered sets computed in the prefixes
            ASSERT_TRUE (correctPids (pids, prefixes, closed, manager.get_sc0 ()));
            for (const auto& ipid: pids) {
                ASSERT_EQ (ipid.size (), 0);
            }
        }
    }
}

// Check the suffix ids computed by the prefix construction when considering a
// difference with the cost of the optimal paths equal to 2:
//
// 1. When using quasi-centroids no suffix ids are returned along with the
//    prefixes.
//
//    For a suffix to intersect the prefix of a quasi-centroid at least three
//    true centroids must be taken but that creates a difference with the cost
//    of the optimal solution equal to 6, larger than the value tested here, 4.
//
// 2. When using true centroids an arbitrary number of suffix ids are returned
//    along with the prefixes but in two special cases docummented below.
//
//    In general, there should be suffix ids returned by the prefix because with
//    a different of cost equal to 4 there must be at least one detour that
//    intersects with the prefix.
// -----------------------------------------------------------------------------
TEST_F (sBELAFixture, MultiplePrefixSuffixSuboptimal4Grid) {

    // create a manager to execute sBELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SMALL_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, SMALL_GRID_LENGTH-1);
    khs::sbela<grid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::idnode_t<grid_t>> closed;
    populateClosed<khs::idnode_t, grid_t> (closed, SMALL_GRID_LENGTH);

    // First, paths of length 2*(SMALL_GRID_LENGTH-1) must be discovered, and
    // this means that backward g-values should be propagated to all nodes in
    // those paths. Thus, use the last horizontal and vertical edge to get to
    // the goal as quasi-centroids of all optimal paths getting to the goal
    // through those edges and set the backward g-value of the end vertex of the
    // centroid
    update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, SMALL_GRID_LENGTH-1),
                                           0,
                                           closed);
    khs::centroid_t z0 = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-2, SMALL_GRID_LENGTH-1)),
                                          closed.find (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, SMALL_GRID_LENGTH-1)),
                                          2*(SMALL_GRID_LENGTH-1));
    khs::bucket_t<khs::centroid_t> centroids;
    std::vector<std::unordered_set<size_t>> pids;
    std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z0, centroids, pids);

    khs::centroid_t z1 = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, SMALL_GRID_LENGTH-2)),
                                          closed.find (grid_t (SMALL_GRID_LENGTH, SMALL_GRID_LENGTH-1, SMALL_GRID_LENGTH-1)),
                                          2*(SMALL_GRID_LENGTH-1));
    prefixes = manager.get_prefixes (closed, z1, centroids, pids);

    // In order to be able to generate suboptimal paths with a difference of
    // cost with the optimal solution equal to 4 it is necessary to propagate
    // more gb-values. For this, use all vertical and horizontal centroids and
    // compute its prefixes. As a result, every node (but the goal node) will
    // have two gb-values with the same parity.

    // First, compute the prefixes of all horizontal centroids
    for (auto i = 1 ; i < SMALL_GRID_LENGTH ; i++) {
        for (auto j = 0 ; j < SMALL_GRID_LENGTH - 1 ; j++) {

            // create the centroid and compute its suffixes
            update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, i-1, j),
                                                   2*SMALL_GRID_LENGTH - i - j + 1,
                                                   closed);
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SMALL_GRID_LENGTH, i-1, j)),
                                                 2*SMALL_GRID_LENGTH);
            std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z, centroids, pids);
        }
    }

    // Second, compute the prefixes of all vertical centroids
    for (auto i = 0 ; i < SMALL_GRID_LENGTH - 1; i++) {
        for (auto j = 1 ; j < SMALL_GRID_LENGTH ; j++) {

            // create the centroid and compute its suffixes
            update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, i, j-1),
                                                   2*SMALL_GRID_LENGTH - i - j + 1,
                                                   closed);
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SMALL_GRID_LENGTH, i, j-1)),
                                                 2*SMALL_GRID_LENGTH);
            std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z, centroids, pids);
        }
    }

    // Everything's almost in place! Still, many nodes have not a backward
    // g-value which allows the computation the computation of all suffixes with
    // a cost equal to 2*(1+SIMPLE_GRID_LENGTH), but many will be properly
    // computed in the first loop. Note that because it will launch the prefix
    // computation, more gb-values will be propagated and this is the reason why
    // loops 2--4 take longer than the first.

    // Quasi-centroids (i, j) -> (i+1, j). In all cases, the prefix computation
    // must return no suffix ids. For a suffix to intersect with the prefix, at
    // least three true centroids must be taken, and that creates a difference
    // with the cost of the optimal solution equal to 6.
    for (auto i = 0 ; i < SMALL_GRID_LENGTH - 1 ; i++) {
        for (auto j = 0 ; j < SMALL_GRID_LENGTH ; j++) {

            // create the centroid and compute its suffixes
            update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, i+1, j),
                                                   2*SMALL_GRID_LENGTH - i - j + 1,
                                                   closed);
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SMALL_GRID_LENGTH, i+1, j)),
                                                 2*(1+SMALL_GRID_LENGTH));
            std::vector<std::unordered_set<size_t>> pids;
            std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z);
            std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z, centroids, pids);

            // and verify the unordered sets computed in the prefixes
            ASSERT_TRUE (correctPids (pids, prefixes, closed, manager.get_sc0 ()));
            for (const auto& ipid: pids) {
                ASSERT_EQ (ipid.size (), 0);
            }
        }
    }

    // Quasi-centroids (i, j) -> (i, j+1). In all cases, the prefix computation
    // must return no suffix ids. For a suffix to intersect with the prefix, at
    // least three true centroids must be taken, and that creates a difference
    // with the cost of the optimal solution equal to 6.
    for (auto i = 0 ; i < SMALL_GRID_LENGTH ; i++) {
        for (auto j = 0 ; j < SMALL_GRID_LENGTH - 1 ; j++) {

            // create the centroid and compute its suffixes
            update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, i, j+1),
                                                   2*SMALL_GRID_LENGTH - i - j + 1,
                                                   closed);
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SMALL_GRID_LENGTH, i, j+1)),
                                                 2*(1+SMALL_GRID_LENGTH));
            std::vector<std::unordered_set<size_t>> pids;
            std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z);
            std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z, centroids, pids);

            // and verify the unordered sets computed in the prefixes
            ASSERT_TRUE (correctPids (pids, prefixes, closed, manager.get_sc0 ()));
            for (const auto& ipid: pids) {
                ASSERT_EQ (ipid.size (), 0);
            }
        }
    }

    // Centroids (i, j) -> (i-1, j) where
    //
    // 1. The bottom row must produce no pids because the prefixes of every
    //    centroid (i, 0) -> (i-1, 0) are empty as they must contain the end
    //    vertex of the centroid.
    //
    // 2. The right column must produce no pids as well. The idea is that for
    //    any centroid (i, j) -> (i-1, j) there is at least one suffix that goes
    //    (i-1, j-1) -> (i, j-1) -> (i+1, j-1) -> (i+1, j) and from there to the
    //    goal. Note that (i, j) is explicitly avoided because no suffix can
    //    contain the starting vertex of the centroid. Now, from
    //    (SMALL_GRID_LENGTH-1, j) it is impossible to create that tour and
    //    hence no pids must be generated.
    for (auto i = 1 ; i < SMALL_GRID_LENGTH ; i++) {
        for (auto j = 0 ; j < SMALL_GRID_LENGTH ; j++) {

            // create the centroid and compute its suffixes
            update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, i-1, j),
                                                   2*SMALL_GRID_LENGTH - i - j + 3,
                                                   closed);
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SMALL_GRID_LENGTH, i-1, j)),
                                                 2*(1+SMALL_GRID_LENGTH));
            std::vector<std::unordered_set<size_t>> pids;
            std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z);
            std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z, centroids, pids);

            // and verify the unordered sets computed in the prefixes
            ASSERT_TRUE (correctPids (pids, prefixes, closed, manager.get_sc0 ()));
            for (const auto& ipid: pids) {

                // in case we are at the bottom row, or the right column check
                // the number of pids is null
                if (j == 0 || i == SMALL_GRID_LENGTH-1) {
                    ASSERT_EQ (ipid.size (), 0);
                } else {

                    // otherwise, verify there is a strictly positive number of
                    // pids
                    ASSERT_GT (ipid.size (), 0);
                }
            }
        }
    }

    // Centroids (i, j) -> (i, j-1) where
    //
    // 1. The left vertical column must produce no pids because the prefixes of
    //    every centroid (0, j) -> (0, j-1) are empty as they must contain the
    //    end vertex of the centroid.
    //
    // 2. The top row must produce no pids as well. The idea is that for any
    //    centroid (i, j) -> (i, j-1) there is at least one suffix that goes
    //    (i-1, j-1) -> (i-1, j) -> (i-1, j+1) -> (i, j+1) and from there to the
    //    goal. Note that (i, j) is explicitly avoided because no suffix can
    //    contain the starting vertex of the centroid. Now, from (i,
    //    SMALL_GRID_LENGTH-1) it is impossible to create that tour and hence
    //    no pids must be generated.
    for (auto i = 0 ; i < SMALL_GRID_LENGTH; i++) {
        for (auto j = 1 ; j < SMALL_GRID_LENGTH; j++) {

            // create the centroid and compute its suffixes
            update_gbvalue<khs::idnode_t, grid_t> (grid_t (SMALL_GRID_LENGTH, i, j-1),
                                                   2*SMALL_GRID_LENGTH - i - j + 3,
                                                   closed);
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SMALL_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SMALL_GRID_LENGTH, i, j-1)),
                                                 2*(1+SMALL_GRID_LENGTH));
            std::vector<std::unordered_set<size_t>> pids;
            std::vector<std::vector<size_t>> suffixes = manager.get_suffixes (closed, z);
            std::vector<std::vector<size_t>> prefixes = manager.get_prefixes (closed, z, centroids, pids);

            // and verify the unordered sets computed in the prefixes
            ASSERT_TRUE (correctPids (pids, prefixes, closed, manager.get_sc0 ()));
            for (const auto& ipid: pids) {

                // in case we are at the top row or the left certical column
                // check the number of pids is null
                if (j == SMALL_GRID_LENGTH-1 || i == 0 ) {
                    ASSERT_EQ (ipid.size (), 0);
                } else {

                    // otherwise, verify there is a strictly positive number of
                    // pids
                    ASSERT_GT (ipid.size (), 0);
                }
            }
        }
    }
}

// Verify that the computation of optimal (thus simple) paths in a grid is
// computed correctly
// -----------------------------------------------------------------------------
TEST_F (sBELAFixture, GetOptimalSimplePathsGrid) {

    // create a manager to execute sBELA*. The precise number of solutions to
    // find is given because the doctor service will be used
    int k = binomial_coefficient (2*(VERY_SMALL_GRID_LENGTH-1), VERY_SMALL_GRID_LENGTH-1)/2;
    grid_t start = grid_t (VERY_SMALL_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (VERY_SMALL_GRID_LENGTH, VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1);
    khs::sbela<grid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::idnode_t<grid_t>> closed;
    populateClosed<khs::idnode_t, grid_t> (closed, VERY_SMALL_GRID_LENGTH);

    // sBELA* finds optimal paths using the last edges that get to the goal.

    // first, compute all optimal paths that get to the goal through the last
    // horizontal edge
    khs::centroid_t z0 = khs::centroid_t (closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-2, VERY_SMALL_GRID_LENGTH-1)),
                                          closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1)),
                                          2*(VERY_SMALL_GRID_LENGTH-1));
    khs::bucket_t<khs::centroid_t> centroids;
    khs::ksolution_t<grid_t, std::vector> ksolution0 = manager.get_paths (z0, closed, centroids);

    // check the number of centroids generated so far when computing the optimal
    // paths. This has to be equal to (N-1)^2 which are all the horizontal
    // centroids (i, j) -> (i-1, j) times 2 to account also for the vertical
    // centroids (i, j) -> (i, j-1) in the range [0, N-1). Also, (N-2) have to
    // be added to account for the horizontal centroids created in the top row
    ASSERT_EQ (centroids.size (),
               2*(VERY_SMALL_GRID_LENGTH-1)*(VERY_SMALL_GRID_LENGTH-1)+(VERY_SMALL_GRID_LENGTH-2));

    // check that the bucket of centroids contains now centroids with a cost
    // strictly equal to C0+2
    while (centroids.size () > 0 ) {
        khs::centroid_t z = centroids.pop_front ();
        ASSERT_EQ (z.get_cost (), 2*VERY_SMALL_GRID_LENGTH);
    }

    // finally, check also the solution is correct
    ASSERT_TRUE (ksolution0.doctor ());

    // second, compute all optimal paths that get to the goal through the last
    // vertical edge
    khs::centroid_t z1 = khs::centroid_t (closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-2)),
                                          closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1)),
                                          2*(VERY_SMALL_GRID_LENGTH-1));
    khs::ksolution_t<grid_t, std::vector> ksolution1 = manager.get_paths (z1, closed, centroids);

    // check the number of centroids generated so far when computing these extra
    // optimal paths. N-2 new centroids have to be created, those in the right
    // column of the grid
    ASSERT_EQ (centroids.size (), VERY_SMALL_GRID_LENGTH-2);

    // check that the bucket of centroids contains now centroids with a cost
    // strictly equal to C0+2
    while (centroids.size () > 0 ) {
        khs::centroid_t z = centroids.pop_front ();
        ASSERT_EQ (z.get_cost (), 2*VERY_SMALL_GRID_LENGTH);
    }

    // finally, check also the solution is correct
    ASSERT_TRUE (ksolution1.doctor ());
}

// Verify that the computation of suboptimal paths with a cost equal to C1=C0+2
// in a grid produces simple paths indeed
// -----------------------------------------------------------------------------
TEST_F (sBELAFixture, GetSuboptimal2SimplePathsGrid) {

    // create a manager to execute sBELA*. The number of paths to compute is
    // explicitly given to ensure that all optimal paths are computed
    int k = binomial_coefficient (2*(VERY_SMALL_GRID_LENGTH-1), VERY_SMALL_GRID_LENGTH-1)/2;
    grid_t start = grid_t (VERY_SMALL_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (VERY_SMALL_GRID_LENGTH, VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1);
    khs::sbela<grid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::idnode_t<grid_t>> closed;
    populateClosed<khs::idnode_t, grid_t> (closed, VERY_SMALL_GRID_LENGTH);

    // compute now *all* optimal paths to ensure that all nodes get their
    // corresponding gb-value

    // compute all optimal paths that get to the goal through the last
    // horizontal edge and set the backward g-value of the end vertex of the
    // centroid
    update_gbvalue<khs::idnode_t, grid_t> (grid_t (VERY_SMALL_GRID_LENGTH, VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1),
                                           0,
                                           closed);
    khs::centroid_t z0 = khs::centroid_t (closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-2, VERY_SMALL_GRID_LENGTH-1)),
                                          closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1)),
                                          2*(VERY_SMALL_GRID_LENGTH-1));
    khs::bucket_t<khs::centroid_t> centroids;
    khs::ksolution_t<grid_t, std::vector> ksolution0 = manager.get_paths (z0, closed, centroids);

    // and through the last vertical edge.
    khs::centroid_t z1 = khs::centroid_t (closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-2)),
                                          closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1)),
                                          2*(VERY_SMALL_GRID_LENGTH-1));
    khs::ksolution_t<grid_t, std::vector> ksolution1 = manager.get_paths (z1, closed, centroids);

    // compute suboptimal paths with a cost equal to C1=C0+2 = 8. These paths
    // can be computed using the centroids discovered when computing the optimal
    // paths
    int nbsolutions = 0;
    khs::centroid_t z = centroids.pop_front ();
    while (z.get_cost () == 8) {

        // compute all the suboptimal paths with length C0+2
        khs::ksolution_t<grid_t, std::vector> suboptimal = manager.get_paths (z, closed, centroids);

        // verify every suboptimal solution path is a simple path
        for (const auto& is : suboptimal.get_solutions ()) {
            ASSERT_TRUE (isSimplePath (is.get_solution ()));
            nbsolutions++;
        }

        // get the next centroid
        z = centroids.pop_front ();
    }

    // show information on the log file about the number of solutions found with cost C1=c0+2
    std::clog << " [sBELAFixture, GetSuboptimal2SimplePathsGrid] Number of solution paths with cost C1=C0+2 in a (4x4) grid" << std::endl;
    std::clog << " #solutions : " << nbsolutions << std::endl << std::endl;
}

// Verify that the computation of suboptimal paths with a cost equal to C2=C0+4
// in a grid produces simple paths indeed
// -----------------------------------------------------------------------------
TEST_F (sBELAFixture, GetSuboptimal4SimplePathsGrid) {

    // create a manager to execute sBELA*. The number of paths to compute is
    // explicitly given to ensure that all optimal paths are computed
    int k = binomial_coefficient (2*(VERY_SMALL_GRID_LENGTH-1), VERY_SMALL_GRID_LENGTH-1)/2;
    grid_t start = grid_t (VERY_SMALL_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (VERY_SMALL_GRID_LENGTH, VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1);
    khs::sbela<grid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::idnode_t<grid_t>> closed;
    populateClosed<khs::idnode_t, grid_t> (closed, VERY_SMALL_GRID_LENGTH);

    // compute now *all* optimal paths to ensure that all nodes get their
    // corresponding gb-value

    // compute all optimal paths that get to the goal through the last
    // horizontal edge and set the backward g-value of the end vertex of the
    // centroid
    update_gbvalue<khs::idnode_t, grid_t> (grid_t (VERY_SMALL_GRID_LENGTH, VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1),
                                           0,
                                           closed);
    khs::centroid_t z0 = khs::centroid_t (closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-2, VERY_SMALL_GRID_LENGTH-1)),
                                          closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1)),
                                          2*(VERY_SMALL_GRID_LENGTH-1));
    khs::bucket_t<khs::centroid_t> centroids;
    khs::ksolution_t<grid_t, std::vector> ksolution0 = manager.get_paths (z0, closed, centroids);

    // and through the last vertical edge.
    khs::centroid_t z1 = khs::centroid_t (closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-2)),
                                          closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1)),
                                          2*(VERY_SMALL_GRID_LENGTH-1));
    khs::ksolution_t<grid_t, std::vector> ksolution1 = manager.get_paths (z1, closed, centroids);

    // compute suboptimal paths with a cost equal to C2=C0+4 = 10. These paths
    // can be computed using the centroids discovered when computing the optimal
    // paths and also all suboptimal paths with cost C1=C0+2.
    int nbsolutions = 0;
    khs::centroid_t z = centroids.pop_front ();
    while (z.get_cost () <= 10) {

        // compute all the suboptimal paths with length C0+4
        khs::ksolution_t<grid_t, std::vector> suboptimal = manager.get_paths (z, closed, centroids);

        // verify every suboptimal solution path with cost C2 is a simple path
        if (z.get_cost () == 10) {

            for (const auto& is : suboptimal.get_solutions ()) {
                ASSERT_TRUE (isSimplePath (is.get_solution ()));
                nbsolutions++;
            }
        }

        // get the next centroid
        z = centroids.pop_front ();
    }

    // show information on the log file about the number of solutions found with cost C1=c0+2
    std::clog << " [sBELAFixture, GetSuboptimal4SimplePathsGrid] Number of solution paths with cost C2=C0+4 in a (4x4) grid" << std::endl;
    std::clog << " #solutions : " << nbsolutions << std::endl << std::endl;
}

// Verify that the computation of suboptimal paths with a cost equal to C3=C0+6
// in a grid produces simple paths indeed
// -----------------------------------------------------------------------------
TEST_F (sBELAFixture, GetSuboptimal6SimplePathsGrid) {

    // create a manager to execute sBELA*. The number of paths to compute is
    // explicitly given to ensure that all optimal paths are computed
    int k = binomial_coefficient (2*(VERY_SMALL_GRID_LENGTH-1), VERY_SMALL_GRID_LENGTH-1)/2;
    grid_t start = grid_t (VERY_SMALL_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (VERY_SMALL_GRID_LENGTH, VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1);
    khs::sbela<grid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::idnode_t<grid_t>> closed;
    populateClosed<khs::idnode_t, grid_t> (closed, VERY_SMALL_GRID_LENGTH);

    // compute now *all* optimal paths to ensure that all nodes get their
    // corresponding gb-value

    // compute all optimal paths that get to the goal through the last
    // horizontal edge and set the backward g-value of the end vertex of the
    // centroid
    update_gbvalue<khs::idnode_t, grid_t> (grid_t (VERY_SMALL_GRID_LENGTH, VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1),
                                           0,
                                           closed);
    khs::centroid_t z0 = khs::centroid_t (closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-2, VERY_SMALL_GRID_LENGTH-1)),
                                          closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1)),
                                          2*(VERY_SMALL_GRID_LENGTH-1));
    khs::bucket_t<khs::centroid_t> centroids;
    khs::ksolution_t<grid_t, std::vector> ksolution0 = manager.get_paths (z0, closed, centroids);

    // and through the last vertical edge.
    khs::centroid_t z1 = khs::centroid_t (closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-2)),
                                          closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1)),
                                          2*(VERY_SMALL_GRID_LENGTH-1));
    khs::ksolution_t<grid_t, std::vector> ksolution1 = manager.get_paths (z1, closed, centroids);

    // compute suboptimal paths with a cost equal to C3=C0+6 = 12. These paths
    // can be computed using the centroids discovered when computing the optimal
    // paths and also all suboptimal paths with cost C1 and C2.
    int nbsolutions = 0;
    khs::centroid_t z = centroids.pop_front ();
    while (z.get_cost () <= 12) {

        // compute all the suboptimal paths with length C0+6
        khs::ksolution_t<grid_t, std::vector> suboptimal = manager.get_paths (z, closed, centroids);

        // verify every suboptimal solution path with cost C3 is a simple path
        if (z.get_cost () == 12) {

            for (const auto& is : suboptimal.get_solutions ()) {
                ASSERT_TRUE (isSimplePath (is.get_solution ()));
                nbsolutions++;
            }
        }

        // get the next centroid
        z = centroids.pop_front ();
    }

    // show information on the log file about the number of solutions found with cost C1=c0+2
    std::clog << " [sBELAFixture, GetSuboptimal6SimplePathsGrid] Number of solution paths with cost C3=C0+6 in a (4x4) grid" << std::endl;
    std::clog << " #solutions : " << nbsolutions << std::endl << std::endl;
}

// Verify that the computation of suboptimal paths with a cost equal to C4=C0+8
// in a grid produces simple paths indeed. Note this is the step length of the
// longest simple path in a (4,4)-grid
// -----------------------------------------------------------------------------
TEST_F (sBELAFixture, GetSuboptimal8SimplePathsGrid) {

    // create a manager to execute sBELA*. The number of paths to compute is
    // explicitly given to ensure that all optimal paths are computed
    int k = binomial_coefficient (2*(VERY_SMALL_GRID_LENGTH-1), VERY_SMALL_GRID_LENGTH-1)/2;
    grid_t start = grid_t (VERY_SMALL_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (VERY_SMALL_GRID_LENGTH, VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1);
    khs::sbela<grid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::idnode_t<grid_t>> closed;
    populateClosed<khs::idnode_t, grid_t> (closed, VERY_SMALL_GRID_LENGTH);

    // compute now *all* optimal paths to ensure that all nodes get their
    // corresponding gb-value

    // compute all optimal paths that get to the goal through the last
    // horizontal edge and set the backward g-value of the end vertex of the
    // centroid
    update_gbvalue<khs::idnode_t, grid_t> (grid_t (VERY_SMALL_GRID_LENGTH, VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1),
                                           0,
                                           closed);
    khs::centroid_t z0 = khs::centroid_t (closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-2, VERY_SMALL_GRID_LENGTH-1)),
                                          closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1)),
                                          2*(VERY_SMALL_GRID_LENGTH-1));
    khs::bucket_t<khs::centroid_t> centroids;
    khs::ksolution_t<grid_t, std::vector> ksolution0 = manager.get_paths (z0, closed, centroids);

    // and through the last vertical edge.
    khs::centroid_t z1 = khs::centroid_t (closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-2)),
                                          closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1)),
                                          2*(VERY_SMALL_GRID_LENGTH-1));
    khs::ksolution_t<grid_t, std::vector> ksolution1 = manager.get_paths (z1, closed, centroids);

    // compute suboptimal paths with a cost equal to C4=C0+8 = 14. These paths
    // can be computed using the centroids discovered when computing the optimal
    // paths and also all suboptimal paths with cost C1, C2 and C3.
    int nbsolutions = 0;
    khs::centroid_t z = centroids.pop_front ();
    while (z.get_cost () <= 14) {

        // compute all the suboptimal paths with length C0+8
        khs::ksolution_t<grid_t, std::vector> suboptimal = manager.get_paths (z, closed, centroids);

        // verify every suboptimal solution path with cost C4 is a simple path
        if (z.get_cost () == 14) {

            for (const auto& is : suboptimal.get_solutions ()) {
                ASSERT_TRUE (isSimplePath (is.get_solution ()));
                nbsolutions++;
            }
        }

        // get the next centroid
        z = centroids.pop_front ();
    }

    // show information on the log file about the number of solutions found with cost C1=c0+2
    std::clog << " [sBELAFixture, GetSuboptimal8SimplePathsGrid] Number of solution paths with cost C4=C0+8 in a (4x4) grid: Longest Simple Path" << std::endl;
    std::clog << " #solutions : " << nbsolutions << std::endl << std::endl;
}

// Verify that the computation of paths in a (4,4)-grid eventually exhausts all
// centroids so that the longest path problem is found
// -----------------------------------------------------------------------------
TEST_F (sBELAFixture, GetLongestPathsGrid) {

    // create a manager to execute sBELA*. The number of paths is infinitely
    // large as we are looking for a longest path in the (4,4)-grid between two
    // specific vertices
    int k = std::numeric_limits<int>::max();
    grid_t start = grid_t (VERY_SMALL_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (VERY_SMALL_GRID_LENGTH, VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1);
    khs::sbela<grid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::idnode_t<grid_t>> closed;
    populateClosed<khs::idnode_t, grid_t> (closed, VERY_SMALL_GRID_LENGTH);

    // compute now *all* optimal paths to ensure that all nodes get their
    // corresponding gb-value

    // compute all optimal paths that get to the goal through the last
    // horizontal edge
    update_gbvalue<khs::idnode_t, grid_t> (grid_t (VERY_SMALL_GRID_LENGTH, VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1),
                                           0,
                                           closed);
    khs::centroid_t z0 = khs::centroid_t (closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-2, VERY_SMALL_GRID_LENGTH-1)),
                                          closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1)),
                                          2*(VERY_SMALL_GRID_LENGTH-1));
    khs::bucket_t<khs::centroid_t> centroids;
    khs::ksolution_t<grid_t, std::vector> ksolution0 = manager.get_paths (z0, closed, centroids);

    // and through the last vertical edge.
    khs::centroid_t z1 = khs::centroid_t (closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-2)),
                                          closed.find (grid_t (VERY_SMALL_GRID_LENGTH,
                                                               VERY_SMALL_GRID_LENGTH-1, VERY_SMALL_GRID_LENGTH-1)),
                                          2*(VERY_SMALL_GRID_LENGTH-1));
    khs::ksolution_t<grid_t, std::vector> ksolution1 = manager.get_paths (z1, closed, centroids);

    // compute all paths between the start and goal state until all centroids
    // are exhausted
    int maxlength = 0;
    while (centroids.size () > 0) {

        khs::centroid_t z = centroids.pop_front ();

        // expand this centroid and get all paths. While doing so, discover
        // other centroids
        khs::ksolution_t<grid_t, std::vector> paths = manager.get_paths (z, closed, centroids);

        // in case any paths have been found then update the length of the
        // longest path. Note that the cost of all paths found is equal to the
        // overall cost of the centroid.
        if (paths.size () > 0 and maxlength < z.get_cost ()) {
            maxlength = z.get_cost ();
        }
    }

    // show information on the log file about the number of solutions found with cost C1=c0+2
    std::clog << " [sBELAFixture, GetLongestPathGrid] Length of the Longest Path in a (4x4) grid" << std::endl;
    std::clog << " length : " << maxlength << std::endl << std::endl;
}

// Checks that sBELA0 correctly solves instances where start=goal in the
// n-pancake
// ----------------------------------------------------------------------------
TEST_F (sBELAFixture, NPancakeBruteForceSameSolution) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to run the sBELA* search algorithm to solve a random
        // instance a random number of times. h=0 is given for the original node
        // because no search is performed
        int k = 1 + rand () % MAX_VALUES;
        npancake_t start = randInstance (NB_DISCS);
        khs::sbela<npancake_t> manager {k, start, start};

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

// Checks that sBELA* correctly solves instances where start=goal in the
// n-pancake with consistent heuristics
// ----------------------------------------------------------------------------
TEST_F (sBELAFixture, NPancakeConsistentSameSolution) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to run the sBELA* search algorithm to solve a random
        // instance a random number of times. h=0 is given for the original node
        // because no search is performed
        int k = 1 + rand () % MAX_VALUES;
        npancake_t start = randInstance (NB_DISCS);
        npancake_t::init ("unit");
        khs::sbela<npancake_t> manager {k, start, start, false};

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

// Check that sBELA0 correctly finds one single simple solution between a random
// instance and the identity permutation in the 5-Pancake
// ----------------------------------------------------------------------------
TEST_F (sBELAFixture, NPancakeUnitBruteForceOne) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to find a single simple solution between a couple of
        // random instances of the 5-Pancake
        int k = 1;
        npancake_t start = randInstance (5);
        npancake_t goal = npancake_t{1, 2, 3, 4, 5};
        while (start == goal) {
            start = randInstance (5);
        }
        npancake_t::init ("unit");
        khs::sbela<npancake_t> manager {k, start, goal};

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

        // solve now the same task using the baseline simplistic BELA* enforcing
        // full exploration
        khs::bbela<npancake_t> manager0 {k, start, goal, false};
        auto ksolution0 = manager0.solve ();

        // and verify the cost of the solutions computed are the same
        ASSERT_TRUE (ksolution.doctor (ksolution0));
    }
}

// Check that sBELA0 correctly finds two single simple solutions between a
// random instance of the 5-Pancake and the identity permutation
// ----------------------------------------------------------------------------
TEST_F (sBELAFixture, NPancakeUnitBruteForceTwo) {

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
        khs::sbela<npancake_t> manager {k, start, goal};

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

        // solve now the same task using the baseline simplistic BELA* enforcing
        // full exploration
        khs::bbela<npancake_t> manager0 {k, start, goal, false};
        auto ksolution0 = manager0.solve ();

        // and verify the cost of the solutions computed are the same
        ASSERT_TRUE (ksolution.doctor (ksolution0));
    }
}

// Check that sBELA0 correctly finds an arbitrary number of simple solutions (10
// <= k <= 20) between a random instance of the 5-Pancake and the identtity
// permutation
// ----------------------------------------------------------------------------
TEST_F (sBELAFixture, NPancakeUnitBruteForceArbitrary) {

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
        khs::sbela<npancake_t> manager {k, start, goal};

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

        // solve now the same task using the baseline simplistic BELA* enforcing
        // full exploration
        khs::bbela<npancake_t> manager0 {k, start, goal, false};
        auto ksolution0 = manager0.solve ();

        // and verify the cost of the solutions computed are the same
        ASSERT_TRUE (ksolution.doctor (ksolution0));
    }
}

// Check that sBELA* correctly finds one single simple solution between a random
// instance of the 5-Pancake and the identity permutation using a consistent
// heuristic function
// ----------------------------------------------------------------------------
TEST_F (sBELAFixture, NPancakeUnitConsistentOne) {

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
        khs::sbela<npancake_t> manager {k, start, goal, false};

        // verify the variant and whether the heuristic function is consistent
        ASSERT_EQ (npancake_t::get_variant (), "unit");

        // and invoke the solver
        auto ksolution = manager.solve ();

        // verify the solution found contains one single solution
        ASSERT_EQ (ksolution.size (), k);
        for (auto j = 0 ; j < int (ksolution.size ()); j++) {
            ASSERT_TRUE (ksolution[j].get_simple ());
        }

        // and verify it is correct
        ASSERT_TRUE (ksolution.doctor ());

        // solve now the same task using the baseline simplistic BELA* enforcing
        // full exploration
        khs::bbela<npancake_t> manager0 {k, start, goal, false};
        auto ksolution0 = manager0.solve ();

        // and verify the cost of the solutions computed are the same
        ASSERT_TRUE (ksolution.doctor (ksolution0));
    }
}

// Check that sBELA* correctly finds two single simple solutions between two
// instances of the 5-Pancake randomly generated and the identity permutation
// using a consistent heuristic function
// ----------------------------------------------------------------------------
TEST_F (sBELAFixture, NPancakeUnitConsistentTwo) {

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
        khs::sbela<npancake_t> manager {k, start, goal, false};

        // verify the variant and whether the heuristic function is consistent
        ASSERT_EQ (npancake_t::get_variant (), "unit");

        // and invoke the solver
        auto ksolution = manager.solve ();

        // verify the solution found contains two solutions
        ASSERT_EQ (ksolution.size (), k);
        for (auto j = 0 ; j < int (ksolution.size ()); j++) {
            ASSERT_TRUE (ksolution[j].get_simple ());
        }

        // and verify they are correct
        ASSERT_TRUE (ksolution.doctor ());

        // solve now the same task using the baseline simplistic BELA* enforcing
        // full exploration
        khs::bbela<npancake_t> manager0 {k, start, goal, false};
        auto ksolution0 = manager0.solve ();

        // and verify the cost of the solutions computed are the same
        ASSERT_TRUE (ksolution.doctor (ksolution0));
    }
}

// Check that sBELA* correctly finds an arbitrary number of simple solutions (10
// <= k <= 20) between a random instance of the 5-Pancake and the identity
// permutation using a consistent heuristic function
// ----------------------------------------------------------------------------
TEST_F (sBELAFixture, NPancakeUnitConsistentArbitrary) {

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
        khs::sbela<npancake_t> manager {k, start, goal, false};

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

        // solve now the same task using the baseline simplistic BELA* enforcing
        // full exploration
        khs::bbela<npancake_t> manager0 {k, start, goal, false};
        auto ksolution0 = manager0.solve ();

        // and verify the cost of the solutions computed are the same
        ASSERT_TRUE (ksolution.doctor (ksolution0));
    }
}

// Check that sBELA0 correctly finds one single simple solution between a random
// instance of the 5-Pancake and the identity permutation in the heavy-cost
// variant
// ----------------------------------------------------------------------------
TEST_F (sBELAFixture, NPancakeHeavyCostBruteForceOne) {

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
        khs::sbela<npancake_t> manager {k, start, goal};

        // verify the variant and whether the heuristic function is consistent
        // even if no heuristic function is used
        ASSERT_EQ (npancake_t::get_variant (), "heavy-cost");

        // and invoke the solver
        auto ksolution = manager.solve ();

        // verify the solution found contains one single solution
        ASSERT_EQ (ksolution.size (), k);
        for (auto j = 0 ; j < int (ksolution.size ()); j++) {
            ASSERT_TRUE (ksolution[j].get_simple ());
        }

        // and verify it is correct
        ASSERT_TRUE (ksolution.doctor ());

        // solve now the same task using the baseline simplistic BELA* enforcing
        // full exploration
        khs::bbela<npancake_t> manager0 {k, start, goal, false};
        auto ksolution0 = manager0.solve ();

        // and verify the cost of the solutions computed are the same
        ASSERT_TRUE (ksolution.doctor (ksolution0));
    }
}

// Check that sBELA0 correctly finds two single simple solutions between a
// random instance of the 5-Pancake and the identity permutation in the
// heavy-cost variant
// ----------------------------------------------------------------------------
TEST_F (sBELAFixture, NPancakeHeavyCostBruteForceTwo) {

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
        khs::sbela<npancake_t> manager {k, start, goal};

        // verify the variant and whether the heuristic function is consistent
        // even if no heuristic function is used
        ASSERT_EQ (npancake_t::get_variant (), "heavy-cost");

        // and invoke the solver
        auto ksolution = manager.solve ();

        // verify the solution found contains two solutions
        ASSERT_EQ (ksolution.size (), k);
        for (auto j = 0 ; j < int (ksolution.size ()); j++) {
            ASSERT_TRUE (ksolution[j].get_simple ());
        }

        // and verify they are correct
        ASSERT_TRUE (ksolution.doctor ());

        // solve now the same task using the baseline simplistic BELA* enforcing
        // full exploration
        khs::bbela<npancake_t> manager0 {k, start, goal, false};
        auto ksolution0 = manager0.solve ();

        // and verify the cost of the solutions computed are the same
        ASSERT_TRUE (ksolution.doctor (ksolution0));
    }
}

// Check that sBELA0 correctly finds an arbitrary number of simple solutions (10
// <= k <= 20) between a random instance of the 5-Pancake and the identity
// permutation in the heavy-cost variant
// ----------------------------------------------------------------------------
TEST_F (sBELAFixture, NPancakeHeavyCostBruteForceArbitrary) {

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
        khs::sbela<npancake_t> manager {k, start, goal};

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

        // solve now the same task using the baseline simplistic BELA* enforcing
        // full exploration
        khs::bbela<npancake_t> manager0 {k, start, goal, false};
        auto ksolution0 = manager0.solve ();

        // and verify the cost of the solutions computed are the same
        ASSERT_TRUE (ksolution.doctor (ksolution0));
    }
}

// Check that sBELA* correctly finds one single simple solution between one
// instance of the 5-Pancake randomly generated and the identity permutation in
// the heavy-cost variant using a consistent heuristic function
// ----------------------------------------------------------------------------
TEST_F (sBELAFixture, NPancakeHeavyCostConsistentOne) {

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
        khs::sbela<npancake_t> manager {k, start, goal, false};

        // verify the variant and whether the heuristic function is consistent
        ASSERT_EQ (npancake_t::get_variant (), "heavy-cost");

        // and invoke the solver
        auto ksolution = manager.solve ();

        // verify the solution found contains one simple solution
        ASSERT_EQ (ksolution.size (), k);
        for (auto j = 0 ; j < int (ksolution.size ()); j++) {
            ASSERT_TRUE (ksolution[j].get_simple ());
        }

        // and verify it is correct
        ASSERT_TRUE (ksolution.doctor ());

        // solve now the same task using the baseline simplistic BELA* enforcing
        // full exploration
        khs::bbela<npancake_t> manager0 {k, start, goal, false};
        auto ksolution0 = manager0.solve ();

        // and verify the cost of the solutions computed are the same
        ASSERT_TRUE (ksolution.doctor (ksolution0));
    }
}

// Check that sBELA* correctly finds two single simple solutions between a
// random instance of the 5-Pancake and the identity permutation in the
// heavy-cost variant using a consistent heuristic function
// ----------------------------------------------------------------------------
TEST_F (sBELAFixture, NPancakeHeavyCostConsistentTwo) {

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
        khs::sbela<npancake_t> manager {k, start, goal, false};

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

        // solve now the same task using the baseline simplistic BELA* enforcing
        // full exploration
        khs::bbela<npancake_t> manager0 {k, start, goal, false};
        auto ksolution0 = manager0.solve ();

        // and verify the cost of the solutions computed are the same
        ASSERT_TRUE (ksolution.doctor (ksolution0));
    }
}

// Check that sBELA* correctly finds an arbitrary number of simple solutions (10
// <= k <= 20) between a random instance of the 5-Pancake and the identity
// permutation in the heavy-cost variant when using a consistent heuristic
// function
// ----------------------------------------------------------------------------
TEST_F (sBELAFixture, NPancakeHeavyCostConsistentArbitrary) {

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
        khs::sbela<npancake_t> manager {k, start, goal, false};

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

        // solve now the same task using the baseline simplistic BELA* enforcing
        // full exploration
        khs::bbela<npancake_t> manager0 {k, start, goal, false};
        auto ksolution0 = manager0.solve ();

        // and verify the cost of the solutions computed are the same
        ASSERT_TRUE (ksolution.doctor (ksolution0));
    }
}



// Local Variables:
// mode:cpp
// fill-column:80
// End:
