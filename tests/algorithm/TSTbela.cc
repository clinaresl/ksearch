// -*- coding: utf-8 -*-
// TSTbela.cc
// -----------------------------------------------------------------------------
//
// Started on <vie 11-08-2023 21:11:13.019734052 (1691781073)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Unit tests of the BELA* search algorithm
//

#include "../fixtures/TSTbelafixture.h"

using namespace std;

// Checks that the BELA* search solver can be created. If the search algorithm
// is not invoked, it returns the default values
// ----------------------------------------------------------------------------
TEST_F (BELAFixture, ExplicitConstructorPancake) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // Create a manager to run the BELA* search algorithm to solve a random
        // instance a random number of times. h=0 is given for the original node
        // because no search is performed
        int k = 1 + rand () % MAX_VALUES;
        npancake_t start = randInstance (NB_DISCS);
        npancake_t goal = randInstance (NB_DISCS);
        khs::bela<npancake_t> manager {k, start, goal};

        // because the search algorithm is not invoked, it is expected ...
        ASSERT_EQ (manager.get_start (), start);
        ASSERT_EQ (manager.get_goal (), goal);
        ASSERT_EQ (manager.get_k (), k);
        ASSERT_EQ (manager.get_expansions (), 0);

        // Check that the heuristic value is accurately computed
        ASSERT_EQ (manager.get_h0 (), start.h (goal));

        // Note that the running CPU time can not be queried and the result is
        // unpredictable
    }
}

// Check that the closed list of simple grids, to be used in the following tests
// is correctly created
// ----------------------------------------------------------------------------
TEST_F (BELAFixture, ClosedListSimpleGrid) {

    // First, populate a closed list with the expansions of all nodes in the
    // state space of simple grid
    khs::closed_t<khs::labelednode_t<simplegrid_t>> closed;
    populateClosed<simplegrid_t> (closed, SIMPLE_GRID_LENGTH);

    // Now, verify the g*-value of each node in closed
    for (auto i = 0 ; i < closed.size () ; i++) {

        // get the state stored in the i-th index of the closed list
        simplegrid_t state = closed[i].get_state ();

        // check that the g*-value of the state is equal to its x-coordinate
        ASSERT_EQ (closed[i].get_g (), state.get_x ());

        // and verify that all information enclosed in the labeled back pointers
        // is correct

        // every node in the upper line should have only parent ...
        if (state.get_y () == 1) {
            ASSERT_EQ (closed[i].get_backpointers ().size (), 1);

            // verify this node is a descendant of that one and also that the
            // cost is correct
            simplegrid_t parent = (state.get_x () > 1)
                ? simplegrid_t {SIMPLE_GRID_LENGTH, state.get_x () - 1, 1}
                : simplegrid_t {SIMPLE_GRID_LENGTH, 0, 0};
            ASSERT_TRUE (verify_descendant<simplegrid_t> (state, parent, 1));
            ASSERT_TRUE (parent == closed[closed[i].get_backpointer (0).get_pointer()].get_state ());

        } else {

            // whereas nodes in the lower line have two parents for values of
            // x>1
            if (state.get_x () > 1) {
                ASSERT_EQ (closed[i].get_backpointers ().size (), 2);

                // the optimal path to these nodes goes through the parent in
                // the same line, and thus they are written in closed first
                simplegrid_t parent = simplegrid_t {SIMPLE_GRID_LENGTH,
                                                    state.get_x () - 1, 0};
                ASSERT_TRUE (verify_descendant<simplegrid_t> (state, parent, 1));
                ASSERT_TRUE (parent == closed[closed[i].get_backpointer (0).get_pointer()].get_state ());

                // and the second edge gets to them from the node in the upper
                // line
                parent = simplegrid_t {SIMPLE_GRID_LENGTH, state.get_x () - 1, 1};
                ASSERT_TRUE (verify_descendant<simplegrid_t> (state, parent, 2));
                ASSERT_TRUE (parent == closed[closed[i].get_backpointer (1).get_pointer()].get_state ());
            } else {

                // and (0,0) or (1, 0) should have only backpointer
                ASSERT_EQ (closed[i].get_backpointers ().size (), 1);

                // (0, 0) should have the null backpointer
                if (state.get_x () == 0) {

                    // which is the null labeled back pointer
                    khs::labeledbackpointer_t nullbp {string::npos, 0};
                    ASSERT_TRUE (closed[i].get_backpointer (0) == nullbp);
                } else {

                    // whereas (1,0) should point to (0, 0) which is known to be
                    // in location 0 of the closed list ---because it is
                    // explicitly written first in open, so that it gets first
                    // in closed
                    khs::labeledbackpointer_t unitbp {0, 1};
                    ASSERT_TRUE (closed[i].get_backpointer (0) == unitbp);
                }
            }
        }
    }
}

// Check that the closed list of grids, to be used in the following tests is
// correctly created
// ----------------------------------------------------------------------------
TEST_F (BELAFixture, ClosedListGrid) {

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::labelednode_t<grid_t>> closed;
    populateClosed<grid_t> (closed, SIMPLE_GRID_LENGTH);

    // Now, verify the g*-value of each node in closed
    for (auto i = 0 ; i < closed.size () ; i++) {

        // get the state stored in the i-th index of the closed list
        grid_t state = closed[i].get_state ();

        // check that the g*-value of the state is equal to the sum of its coordinates
        ASSERT_EQ (closed[i].get_g (), state.get_x () + state.get_y ());

        // and verify that all information enclosed in the labeled back pointers
        // is correct

        // every node in the interior of the grid should have four backpointers
        if (state.get_x () > 0 && state.get_x () < SIMPLE_GRID_LENGTH-1 &&
            state.get_y () > 0 && state.get_y () < SIMPLE_GRID_LENGTH-1 ) {
            ASSERT_TRUE (closed[i].get_backpointers ().size () == 4);

            // verify all backpointers, i.e., that they are indeed parents of
            // this node and also that the cost registered in closed is correct
            for (auto& bp: closed[i].get_backpointers ()) {
                grid_t parent = closed[bp.get_pointer ()].get_state ();
                ASSERT_TRUE (verify_descendant<grid_t> (state, parent, 1));
            }
        }

        // every node along the lower line, but the corners, should have three
        // backpointers
        if (state.get_y () == 0 && state.get_x () > 0 && state.get_x () < SIMPLE_GRID_LENGTH -1) {
            ASSERT_TRUE (closed[i].get_backpointers ().size () == 3);

            // verify all backpointers, i.e., that they are indeed parents of
            // this node and also that the cost registered in closed is correct
            for (auto& bp: closed[i].get_backpointers ()) {
                grid_t parent = closed[bp.get_pointer ()].get_state ();
                ASSERT_TRUE (verify_descendant<grid_t> (state, parent, 1));
            }
        }

        // every node along the upper line, but the corners and the node
        // immediately adjacent to the goal, should have three backpointers
        if (state.get_y () == SIMPLE_GRID_LENGTH-1) {

            // first, if this node is *not* immediately adjacent to the goal,
            // then it should have three backpointers
            if (state.get_x () > 0 && state.get_x () < SIMPLE_GRID_LENGTH -2) {
                ASSERT_TRUE (closed[i].get_backpointers ().size () == 3);
            } else {
                ASSERT_TRUE (closed[i].get_backpointers ().size () == 2);
            }

            // In any case, verify all backpointers, i.e., that they are indeed
            // parents of this node and also that the cost registered in closed
            // is correct
            for (auto& bp: closed[i].get_backpointers ()) {
                grid_t parent = closed[bp.get_pointer ()].get_state ();
                ASSERT_TRUE (verify_descendant<grid_t> (state, parent, 1));
            }
        }

        // every node along the left vertical line, but the corners, should have
        // three backpointers
        if (state.get_x () == 0 && state.get_y () > 0 && state.get_y () < SIMPLE_GRID_LENGTH -1) {
            ASSERT_TRUE (closed[i].get_backpointers ().size () == 3);

            // verify all backpointers, i.e., that they are indeed parents of
            // this node and also that the cost registered in closed is correct
            for (auto& bp: closed[i].get_backpointers ()) {
                grid_t parent = closed[bp.get_pointer ()].get_state ();
                ASSERT_TRUE (verify_descendant<grid_t> (state, parent, 1));
            }
        }

        // every node along the right vertical line, but the corners and the
        // node immediately adjacent to the goal, should have three backpointers
        if (state.get_x () == SIMPLE_GRID_LENGTH-1) {

            // if this node is *not* immediately adjacent to the goal, then it
            // should have three backpointers
            if (state.get_y () > 0 && state.get_y () < SIMPLE_GRID_LENGTH -2) {
                ASSERT_TRUE (closed[i].get_backpointers ().size () == 3);
            } else {
                ASSERT_TRUE (closed[i].get_backpointers ().size () == 2);
            }

            // In any case, verify all backpointers, i.e., that they are indeed
            // parents of this node and also that the cost registered in closed
            // is correct
            for (auto& bp: closed[i].get_backpointers ()) {
                grid_t parent = closed[bp.get_pointer ()].get_state ();
                ASSERT_TRUE (verify_descendant<grid_t> (state, parent, 1));
            }
        }

        // every corner should have two and only two backpointers
        if ( (state.get_x () == 0 && (state.get_y () == 0 || state.get_y () == SIMPLE_GRID_LENGTH-1)) ||
             (state.get_x () == SIMPLE_GRID_LENGTH-1 && (state.get_y () == 0 || state.get_y () == SIMPLE_GRID_LENGTH-1))) {

            // well, but the start state (0,0) which also contains a null labeled backpointer
            if (state.get_x () == 0 && state.get_y () == 0)
                ASSERT_TRUE (closed[i].get_backpointers ().size () == 3);
            else
                ASSERT_TRUE (closed[i].get_backpointers ().size () == 2);

            // verify all backpointers, i.e., that they are indeed parents of
            // this node and also that the cost registered in closed is correct
            for (auto& bp: closed[i].get_backpointers ()) {

                // skip the null labeled backpointer which, at this point, is
                // known to be stored only in the start state
                if (bp.get_pointer () == string::npos) {
                    continue;
                }

                grid_t parent = closed[bp.get_pointer ()].get_state ();
                ASSERT_TRUE (verify_descendant<grid_t> (state, parent, 1));
            }
        }
    }
}

// Check that centroids with null prefixes return only one path in the simple
// grid domain, the start state itself
// ----------------------------------------------------------------------------
TEST_F (BELAFixture, NullPrefixSimpleGrid) {

    khs::bucket_t<khs::centroid_t> centroids;

    // First, populate a closed list with the expansions of all nodes in the
    // state space of simple grid
    khs::closed_t<khs::labelednode_t<simplegrid_t>> closed;
    populateClosed<simplegrid_t> (closed, SIMPLE_GRID_LENGTH);

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    simplegrid_t start = simplegrid_t (SIMPLE_GRID_LENGTH, 0, 0);
    simplegrid_t goal = simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::bela<simplegrid_t> manager {k, start, goal};

    // even if it is not strictly speaking a centroid, take the edges (0, 0) ->
    // (1, 0) and (0, 0) -> (1, 1) and compute all its optimal prefixes. Note
    // that both edges have the same cost, 1
    khs::centroid_t z0 = khs::centroid_t (closed.find (start),
                                          closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, 1, 0)),
                                          1);
    vector<vector<simplegrid_t>> prefixes = manager.get_prefixes (closed, z0, centroids);

    // verify there is only one prefix, which consists of the start state itself
    ASSERT_TRUE (prefixes.size () == 1);
    ASSERT_TRUE (prefixes[0].size () == 1);
    ASSERT_TRUE (prefixes[0][0] == start);

    khs::centroid_t z1 = khs::centroid_t (closed.find (start),
                                          closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, 1, 1)),
                                          1);
    prefixes = manager.get_prefixes (closed, z1, centroids);

    // verify there is only one prefi, which consists of the start state itselfx
    ASSERT_TRUE (prefixes.size () == 1);
    ASSERT_TRUE (prefixes[0].size () == 1);
    ASSERT_TRUE (prefixes[0][0] == start);
}

// Check that centroids with null prefixes return only one path in the grid
// domain, the start state itself
// ----------------------------------------------------------------------------
TEST_F (BELAFixture, NullPrefixGrid) {

    khs::bucket_t<khs::centroid_t> centroids;

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::labelednode_t<grid_t>> closed;
    populateClosed<grid_t> (closed, SIMPLE_GRID_LENGTH);

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SIMPLE_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::bela<grid_t> manager {k, start, goal};

    // even if it is not strictly speaking a centroid, take the edges (0, 0) ->
    // (1, 0) and (0, 0) -> (0, 1) and compute all its optimal prefixes. Note
    // that both edges have the same cost, 1
    khs::centroid_t z0 = khs::centroid_t (closed.find (start),
                                          closed.find (grid_t (SIMPLE_GRID_LENGTH, 1, 0)),
                                          1);
    vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z0, centroids);

    // verify there is only one prefix, which consists of the start state itself
    ASSERT_TRUE (prefixes.size () == 1);
    ASSERT_TRUE (prefixes[0].size () == 1);
    ASSERT_TRUE (prefixes[0][0] == start);

    khs::centroid_t z1 = khs::centroid_t (closed.find (start),
                                          closed.find (grid_t (SIMPLE_GRID_LENGTH, 0, 1)),
                                          1);
    prefixes = manager.get_prefixes (closed, z1, centroids);

    // verify there is only one prefi, which consists of the start state itselfx
    ASSERT_TRUE (prefixes.size () == 1);
    ASSERT_TRUE (prefixes[0].size () == 1);
    ASSERT_TRUE (prefixes[0][0] == start);
}

// Verify that the number of non-null prefixes is correct in the simple grid domain
// ----------------------------------------------------------------------------
TEST_F (BELAFixture, NonNullPrefixSimpleGrid) {

    khs::bucket_t<khs::centroid_t> centroids;

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a simple grid
    khs::closed_t<khs::labelednode_t<simplegrid_t>> closed;
    populateClosed<simplegrid_t> (closed, SIMPLE_GRID_LENGTH);

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    simplegrid_t start = simplegrid_t (SIMPLE_GRID_LENGTH, 0, 0);
    simplegrid_t goal = simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::bela<simplegrid_t> manager {k, start, goal};

    // traverse all edges with non-null prefixes in the graph and verify all
    // solutions found

    // First, nodes along the lower line have only one prefix. This is true even
    // for the last edge getting to the goal
    for (auto i = 2 ; i < SIMPLE_GRID_LENGTH ; i++) {

        // note the edge (i, 0) -> (i+1, 0) is not a true centroid as it belongs
        // to the optimal path to get to (i+1,0). Nevertheless, get_prefixes
        // should work much the same
        khs::centroid_t z = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i, 0)),
                                             closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, 0)),
                                             1);
        vector<vector<simplegrid_t>> prefixes = manager.get_prefixes (closed, z, centroids);

        // verify there is only one prefix
        ASSERT_TRUE (prefixes.size () == 1);

        // verify the prefix is correct, i.e., it has the right number of nodes
        // and everyone is a descendant of the previous one
        ASSERT_TRUE (prefixes[0].size () == i+1);
        for (auto j = 0 ; j <= i ; j++) {
            ASSERT_TRUE (prefixes[0][j].get_x () == j);
            ASSERT_TRUE (prefixes[0][j].get_y () == 0);
        }
    }

    // Next, nodes along the upper line have only one prefix
    for (auto i = 2 ; i < SIMPLE_GRID_LENGTH ; i++) {

        // note the edge (i, 1) -> (i+1, 1) is not a true centroid as it belongs
        // to the optimal path to get to (i+1,1). Nevertheless, get_prefixes
        // should work much the same. Also, note that the last vertex (9, 1)
        // goes to the goal in the lower line
        int y = (i == SIMPLE_GRID_LENGTH-1) ? 0 : 1;
        khs::centroid_t z = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i, 1)),
                                             closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, y)),
                                             1);
        vector<vector<simplegrid_t>> prefixes = manager.get_prefixes (closed, z, centroids);

        // verify there is only one prefix
        ASSERT_TRUE (prefixes.size () == 1);

        // verify the prefix is correct, i.e., it has the right number of nodes
        // and everyone is a descendant of the previous one
        ASSERT_TRUE (prefixes[0].size () == i+1);
        for (auto j = 0 ; j <= i ; j++) {
            ASSERT_TRUE (prefixes[0][j].get_x () == j);

            // note that every path starts in (0,0) even if it then goes through
            // the upper line
            if (j==0) {
                ASSERT_TRUE (prefixes[0][j].get_y () == 0);
            } else {
                ASSERT_TRUE (prefixes[0][j].get_y () == 1);
            }
        }
    }

    // Next, consider the edges going from the upper line to the lower one.
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH ; i++) {

        // the edge (i, 1) -> (i+1, 0) is a true centroid indeed because its
        // cost is equal to 2, so that paths to (i+1, 0) through this edge are
        // known to be suboptimal.
        khs::centroid_t z {closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i, 1)),
                           closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, 0)),
                           2};
        vector<vector<simplegrid_t>> prefixes = manager.get_prefixes(closed, z, centroids);

        // verify there is only one prefix
        ASSERT_TRUE (prefixes.size () == 1);

        // verify the prefix is correct, i.e., it has the right number of nodes
        // and everyone is a descendant of the previous one
        ASSERT_TRUE (prefixes[0].size () == i+1);
        for (auto j = 0 ; j <= i ; j++) {
            ASSERT_TRUE (prefixes[0][j].get_x () == j);

            // note that every path starts in (0,0) even if it then goes through
            // the upper line
            if (j==0) {
                ASSERT_TRUE (prefixes[0][j].get_y () == 0);
            } else {
                ASSERT_TRUE (prefixes[0][j].get_y () == 1);
            }
        }
    }
}

// Verify that the number of non-null prefixes is correct in the grid domain
// ----------------------------------------------------------------------------
TEST_F (BELAFixture, NonNullPrefixGrid) {

    khs::bucket_t<khs::centroid_t> centroids;

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::labelednode_t<grid_t>> closed;
    populateClosed<grid_t> (closed, SIMPLE_GRID_LENGTH);

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SIMPLE_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::bela<grid_t> manager {k, start, goal};

    // Every node along the left line have only one optimal prefix to get to the
    // start state
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH ; i++) {
        khs::centroid_t z {closed.find (grid_t (SIMPLE_GRID_LENGTH, 0, i)),
                           closed.find (grid_t (SIMPLE_GRID_LENGTH, 0, i+1)),
                           1};
        vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z, centroids);

        // verify there is only one path
        ASSERT_TRUE (prefixes.size () == 1);

        // verify the prefix is correct, i.e., it has the right number of nodes
        // and everyone is a descendant of the previous one
        ASSERT_TRUE (prefixes[0].size () == i+1);
        for (auto j = 0 ; j <= i ; j++) {
            ASSERT_TRUE (prefixes[0][j].get_x () == 0);
            ASSERT_TRUE (prefixes[0][j].get_y () == j);
        }
    }

    // Every node along the lower line have only one optimal prefix to get to the
    // start state
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH ; i++) {
        khs::centroid_t z {closed.find (grid_t (SIMPLE_GRID_LENGTH, i, 0)),
                           closed.find (grid_t (SIMPLE_GRID_LENGTH, i+1, 0)),
                           1};
        vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z, centroids);

        // verify there is only one path
        ASSERT_TRUE (prefixes.size () == 1);

        // verify the prefix is correct, i.e., it has the right number of nodes
        // and everyone is a descendant of the previous one
        ASSERT_TRUE (prefixes[0].size () == i+1);
        for (auto j = 0 ; j <= i ; j++) {
            ASSERT_TRUE (prefixes[0][j].get_x () == j);
            ASSERT_TRUE (prefixes[0][j].get_y () == 0);
        }
    }

    // now, all nodes, but the ones considered previously have a significant
    // number of optimal paths
    vector<vector<grid_t>> prefixes;
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH ; i++) {
        for (auto j = 1 ; j < SIMPLE_GRID_LENGTH ; j++) {

            // (i, j) -> (i+1, j)
            if (i < SIMPLE_GRID_LENGTH-1) {
                khs::centroid_t east {closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                    closed.find (grid_t (SIMPLE_GRID_LENGTH, i+1, j)),
                    1};
                prefixes = manager.get_prefixes (closed, east, centroids);

                // verify the number of optimal paths is equal to the binomial
                // coefficient of (i+j) choose j
                ASSERT_EQ (prefixes.size (), binomial_coefficient (i+j, j));
            }

            // (i, j) -> (i, j+1)
            if (j < SIMPLE_GRID_LENGTH-1) {
                khs::centroid_t north {closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                    closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j+1)), 1};
                prefixes = manager.get_prefixes (closed, north, centroids);

                // verify the number of optimal paths is equal to the binomial
                // coefficient of (i+j) choose i
                ASSERT_EQ (prefixes.size (), binomial_coefficient (i+j, i));
            }

            // (i, j) -> (i-1, j)
            khs::centroid_t west {closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                closed.find (grid_t (SIMPLE_GRID_LENGTH, i-1, j)), 1};
            prefixes = manager.get_prefixes (closed, west, centroids);

            // verify the number of optimal paths is equal to the binomial
            // coefficient of (i+j) choose j
            ASSERT_EQ (prefixes.size (), binomial_coefficient (i+j, j));

            // (i, j) -> (i, j-1)
            khs::centroid_t south {closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j-1)), 1};
            prefixes = manager.get_prefixes (closed, south, centroids);

            // verify the number of optimal paths is equal to the binomial
            // coefficient of (i+j) choose i
            ASSERT_EQ (prefixes.size (), binomial_coefficient (i+j, i));
        }
    }
}

// Check that backward g-values are updated correctly in simple grids using one centroid
// ----------------------------------------------------------------------------
TEST_F (BELAFixture, UpdateBackwardgOneCentroidSimpleGrid) {

    khs::bucket_t<khs::centroid_t> centroids;

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    simplegrid_t start = simplegrid_t (SIMPLE_GRID_LENGTH, 0, 0);
    simplegrid_t goal = simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::bela<simplegrid_t> manager {k, start, goal};

    // Now, consider different edges (to be used as centroids) and verify that
    // the bucket of centroids is updated correctly

    // First, nodes along the lower line have only one prefix.
    for (auto i = 2 ; i < SIMPLE_GRID_LENGTH ; i++) {

        // First, populate a closed list with the expansions of all nodes in the
        // state space of a simple grid
        khs::closed_t<khs::labelednode_t<simplegrid_t>> closed;
        populateClosed<simplegrid_t> (closed, SIMPLE_GRID_LENGTH);

        // note the edge (i, 0) -> (i+1, 0) is not a true centroid as it belongs
        // to the optimal path to get to (i+1,0). Nevertheless, get_prefixes
        // should work much the same
        khs::centroid_t z = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i, 0)),
                                             closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, 0)),
                                             SIMPLE_GRID_LENGTH);
        vector<vector<simplegrid_t>> prefixes = manager.get_prefixes (closed, z, centroids);

        // verify the backward g-values are correct
        for (auto j = 0 ; j <= i ; j++) {

            // look for the node (j, 0) in the closed list
            auto ptr = closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, j, 0));

            // the node (j, 0) should have only backward g-value, equal to
            // SIMPLE_GRID_LENGTH - j
            ASSERT_TRUE (closed[ptr].get_gb().size () == 1);
            ASSERT_TRUE (closed[ptr].get_gb ()[0] == SIMPLE_GRID_LENGTH-j);
        }
    }

    // Second, nodes along the upper line have only one prefix.
    for (auto i = 2 ; i < SIMPLE_GRID_LENGTH - 1; i++) {

        // First, populate a closed list with the expansions of all nodes in the
        // state space of a simple grid
        khs::closed_t<khs::labelednode_t<simplegrid_t>> closed;
        populateClosed<simplegrid_t> (closed, SIMPLE_GRID_LENGTH);

        // note the edge (i, 1) -> (i+1, 1) is not a true centroid as it belongs
        // to the optimal path to get to (i+1,1). Nevertheless, get_prefixes
        // should work much the same.
        khs::centroid_t z = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i, 1)),
                                             closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, 1)),
                                             1+SIMPLE_GRID_LENGTH);
        vector<vector<simplegrid_t>> prefixes = manager.get_prefixes (closed, z, centroids);

        // verify the backward g-values are correct
        for (auto j = 1 ; j <= i ; j++) {

            // look for the node (j, 1) in the closed list
            auto ptr = closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, j, 1));

            // the node (j, 1) should have only backward g-value, equal to 1 +
            // SIMPLE_GRID_LENGTH - j
            ASSERT_TRUE (closed[ptr].get_gb().size () == 1);
            ASSERT_TRUE (closed[ptr].get_gb ()[0] == 1 + SIMPLE_GRID_LENGTH - j);
        }
    }

    // Thirdly, the starting vertex of diagonal edges have only one prefix.
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH; i++) {

        // First, populate a closed list with the expansions of all nodes in the
        // state space of a simple grid
        khs::closed_t<khs::labelednode_t<simplegrid_t>> closed;
        populateClosed<simplegrid_t> (closed, SIMPLE_GRID_LENGTH);

        // note the edge (i, 1) -> (i+1, 0) is a true centroid
        khs::centroid_t z = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i, 1)),
                                             closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, 0)),
                                             1+SIMPLE_GRID_LENGTH);
        vector<vector<simplegrid_t>> prefixes = manager.get_prefixes (closed, z, centroids);

        // verify the backward g-values are correct
        for (auto j = 1 ; j <= i ; j++) {

            // look for the node (j, 1) in the closed list
            auto ptr = closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, j, 1));

            // the node (j, 1) should have only backward g-value, equal to 1 +
            // SIMPLE_GRID_LENGTH - j
            ASSERT_TRUE (closed[ptr].get_gb().size () == 1);
            ASSERT_TRUE (closed[ptr].get_gb ()[0] == 1 + SIMPLE_GRID_LENGTH - j);
        }
    }
}

// Check that backward g-values are updated correctly in simple grids using one centroid
// ----------------------------------------------------------------------------
TEST_F (BELAFixture, UpdateBackwardgOneCentroidGrid) {

    khs::bucket_t<khs::centroid_t> centroids;

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SIMPLE_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::bela<grid_t> manager {k, start, goal};

    // Now, consider different edges (to be used as centroids) and verify that
    // the bucket of centroids is updated correctly

    // First, nodes along the left line have only one prefix.
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH-1 ; i++) {

        // First, populate a closed list with the expansions of all nodes in the
        // state space of a simple grid
        khs::closed_t<khs::labelednode_t<grid_t>> closed;
        populateClosed<grid_t> (closed, SIMPLE_GRID_LENGTH);

        // note the edge (0, i) -> (0, i+1) is not a true centroid as it belongs
        // to the optimal path to get to (0,i+1). Nevertheless, get_prefixes
        // should work much the same
        khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, 0, i)),
                                             closed.find (grid_t (SIMPLE_GRID_LENGTH, 0, i+1)),
                                             2*SIMPLE_GRID_LENGTH);
        vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z, centroids);

        // verify the backward g-values are correct
        for (auto j = 0 ; j <= i ; j++) {

            // look for this node in the closed list
            auto ptr = closed.find (grid_t{SIMPLE_GRID_LENGTH, 0, j});

            // this node should have only one backward g-value equal to
            // 2*SIMPLE_GRID_LENGTH - j
            ASSERT_TRUE (closed[ptr].get_gb ().size () == 1);
            ASSERT_TRUE (closed[ptr].get_gb ()[0] == 2*SIMPLE_GRID_LENGTH-j);
        }
    }

    // Also, nodes along the lower line have only one prefix.
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH-1 ; i++) {

        // First, populate a closed list with the expansions of all nodes in the
        // state space of a simple grid
        khs::closed_t<khs::labelednode_t<grid_t>> closed;
        populateClosed<grid_t> (closed, SIMPLE_GRID_LENGTH);

        // note the edge (i, 0) -> (i+1, 0) is not a true centroid as it belongs
        // to the optimal path to get to (i+1, 0). Nevertheless, get_prefixes
        // should work much the same
        khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, i, 0)),
                                             closed.find (grid_t (SIMPLE_GRID_LENGTH, i+1, 0)),
                                             2*(SIMPLE_GRID_LENGTH-1));
        vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z, centroids);

        // verify the backward g-values are correct
        for (auto j = 0 ; j <= i ; j++) {

            // look for this node in the closed list
            auto ptr = closed.find (grid_t{SIMPLE_GRID_LENGTH, j, 0});

            // this node should have only one backward g-value equal to
            // 2*SIMPLE_GRID_LENGTH - j - 2
            ASSERT_TRUE (closed[ptr].get_gb ().size () == 1);
            ASSERT_TRUE (closed[ptr].get_gb ()[0] == 2*SIMPLE_GRID_LENGTH-j - 2);
        }
    }

    // There are lots of optimal paths in the interior of the grid. However, all
    // nodes should be given only one backward g-value when using only centroid

    // (i, j) -> (i+1, j)
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH - 1 ; i++ ) {
       for (auto j = 1 ; j < SIMPLE_GRID_LENGTH ; j++ ) {

           // First, populate a closed list with the expansions of all nodes in the
           // state space of a simple grid
           khs::closed_t<khs::labelednode_t<grid_t>> closed;
           populateClosed<grid_t> (closed, SIMPLE_GRID_LENGTH);

           // note the edge (i, j) -> (i+1, j) is not a true centroid as it belongs
           // to the optimal path to get to (i+1, j). Nevertheless, get_prefixes
           // should work much the same
           khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                                                closed.find (grid_t (SIMPLE_GRID_LENGTH, i+1, j)),
                                                2*(SIMPLE_GRID_LENGTH - 1));
           vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z, centroids);

           // verify the backward g-values are correct
           for (auto k = 0 ; k <= i ; k++) {
               for (auto l = 0 ; l <= j ; l++) {

                   if (k == i+1 && l==j) continue;

                   // look for this node in the closed list
                   auto ptr = closed.find (grid_t{SIMPLE_GRID_LENGTH, k, l});

                   // this node should have only one backward g-value equal to
                   // 2*SIMPLE_GRID_LENGTH - k - l - 2
                   ASSERT_TRUE (closed[ptr].get_gb ().size () == 1);
                   ASSERT_TRUE (closed[ptr].get_gb ()[0] == 2*SIMPLE_GRID_LENGTH-k-l-2);
               }
           }
       }
    }

    // (i, j) -> (i, j+1)
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH ; i++ ) {
       for (auto j = 1 ; j < SIMPLE_GRID_LENGTH - 1 ; j++ ) {

           // First, populate a closed list with the expansions of all nodes in the
           // state space of a simple grid
           khs::closed_t<khs::labelednode_t<grid_t>> closed;
           populateClosed<grid_t> (closed, SIMPLE_GRID_LENGTH);

           // note the edge (i, j) -> (i, j+1) is not a true centroid as it
           // belongs to the optimal path to get to (i, j+1). Nevertheless,
           // get_prefixes should work much the same
           khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                                                closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j+1)),
                                                2*(SIMPLE_GRID_LENGTH - 1));
           vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z, centroids);

           // verify the backward g-values are correct
           for (auto k = 0 ; k <= i ; k++) {
               for (auto l = 0 ; l <= j ; l++) {

                   // look for this node in the closed list
                   auto ptr = closed.find (grid_t{SIMPLE_GRID_LENGTH, k, l});

                   // this node should have only one backward g-value equal to
                   // 2*SIMPLE_GRID_LENGTH - k - l - 2
                   ASSERT_TRUE (closed[ptr].get_gb ().size () == 1);
                   ASSERT_TRUE (closed[ptr].get_gb ()[0] == 2*SIMPLE_GRID_LENGTH-k-l-2);
               }
           }
       }
    }

    // (i, j) -> (i-1, j)
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH ; i++ ) {
       for (auto j = 1 ; j < SIMPLE_GRID_LENGTH ; j++ ) {

           // First, populate a closed list with the expansions of all nodes in the
           // state space of a simple grid
           khs::closed_t<khs::labelednode_t<grid_t>> closed;
           populateClosed<grid_t> (closed, SIMPLE_GRID_LENGTH);

           // note the edge (i, j) -> (i-1, j) is a true centroid
           khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                                                closed.find (grid_t (SIMPLE_GRID_LENGTH, i-1, j)),
                                                2*SIMPLE_GRID_LENGTH);
           vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z, centroids);

           // verify the backward g-values are correct
           for (auto k = 0 ; k <= i ; k++) {
               for (auto l = 0 ; l <= j ; l++) {

                   // look for this node in the closed list
                   auto ptr = closed.find (grid_t{SIMPLE_GRID_LENGTH, k, l});

                   // it should have only one backward g-value equal to
                   // 2*SIMPLE_GRID_LENGTH - k - l
                   ASSERT_TRUE (closed[ptr].get_gb ().size () == 1);
                   ASSERT_TRUE (closed[ptr].get_gb ()[0] == 2*SIMPLE_GRID_LENGTH-k-l);
               }
           }
       }
    }

    // (i, j) -> (i, j-1)
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH ; i++ ) {
       for (auto j = 1 ; j < SIMPLE_GRID_LENGTH ; j++ ) {

           // First, populate a closed list with the expansions of all nodes in the
           // state space of a simple grid
           khs::closed_t<khs::labelednode_t<grid_t>> closed;
           populateClosed<grid_t> (closed, SIMPLE_GRID_LENGTH);

           // note the edge (i, j) -> (i, j-1) is a true centroid
           khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                                                closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j-1)),
                                                2*SIMPLE_GRID_LENGTH);
           vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z, centroids);

           // verify the backward g-values are correct
           for (auto k = 0 ; k <= i ; k++) {
               for (auto l = 0 ; l <= j ; l++) {

                   // look for thi node in the closed list
                   auto ptr = closed.find (grid_t{SIMPLE_GRID_LENGTH, k, l});

                   // it should have only one backward g-value equal to
                   // 2*SIMPLE_GRID_LENGTH - k - l
                   ASSERT_TRUE (closed[ptr].get_gb ().size () == 1);
                   ASSERT_TRUE (closed[ptr].get_gb ()[0] == 2*SIMPLE_GRID_LENGTH-k-l);
               }
           }
       }
    }
}

// Check that backward g-values are updated correctly in simple grids using two
// centroids
// ----------------------------------------------------------------------------
TEST_F (BELAFixture, UpdateBackwardgTwoCentroidsSimpleGrid) {

    khs::bucket_t<khs::centroid_t> centroids;

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    simplegrid_t start = simplegrid_t (SIMPLE_GRID_LENGTH, 0, 0);
    simplegrid_t goal = simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::bela<simplegrid_t> manager {k, start, goal};

    // Now, consider different combinations of edges (to be used as centroids)
    // and verify that the bucket of centroids is updated correctly. It is impossible to
    // verify all cases and just a selection is made next

    // This test considers the two incident edges of every node in the lower
    // line, but (0, 0) and (1, 0) which have only incident edge
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH ; i++) {

        // First, populate a closed list with the expansions of all nodes in the
        // state space of a simple grid
        khs::closed_t<khs::labelednode_t<simplegrid_t>> closed;
        populateClosed<simplegrid_t> (closed, SIMPLE_GRID_LENGTH);

        // first, create a centroid with the edge (i, 0) -> (i+1, 0) and
        // propagate the backward g-values
        khs::centroid_t z0 = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i, 0)),
                                              closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, 0)),
                                              SIMPLE_GRID_LENGTH);
        vector<vector<simplegrid_t>> prefixes = manager.get_prefixes (closed, z0, centroids);

        // and also consider the edge (i, 1) -> (i+1, 0) and propagate the
        // backward g-values
        khs::centroid_t z1 = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i, 1)),
                                              closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, 0)),
                                              SIMPLE_GRID_LENGTH+1);
        prefixes = manager.get_prefixes (closed, z1, centroids);

        // verify the backward g-values are correct
        for (auto j = 0 ; j <= i ; j++) {

            // look for the node (j, 0) in the closed list
            auto ptr = closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, j, 0));

            // thi node should have only backward g-value, equal to
            // SIMPLE_GRID_LENGTH - j unless it is (0, 0)
            if (j>0) {
                ASSERT_TRUE (closed[ptr].get_gb().size () == 1);
                ASSERT_TRUE (closed[ptr].get_gb ()[0] == SIMPLE_GRID_LENGTH-j);

                // look now for the node (j, 1) in the closed list
                ptr = closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, j, 1));

                // thi node should have only backward g-value, equal to
                // SIMPLE_GRID_LENGTH - j + 1
                ASSERT_TRUE (closed[ptr].get_gb().size () == 1);
                ASSERT_TRUE (closed[ptr].get_gb ()[0] == SIMPLE_GRID_LENGTH-j+1);
            } else {

                // In this case it should have two different backward g-values:
                // SIMPLE_GRID_LENGTH and SIMPLE_GRID_LENGTH+1
                ASSERT_TRUE (closed[ptr].get_gb().size () == 2);
            }

        }
    }

    // The second case considers simultaneously all the outgoing edges of the
    // nodes in the upper line
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH ; i++) {

        // First, populate a closed list with the expansions of all nodes in the
        // state space of a simple grid
        khs::closed_t<khs::labelednode_t<simplegrid_t>> closed;
        populateClosed<simplegrid_t> (closed, SIMPLE_GRID_LENGTH);

        // first, create a centroid with the edge (i, 1) -> (i+1, 1) and
        // propagate the backward g-values
        khs::centroid_t z0 = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i, 1)),
                                              closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, 1)),
                                              SIMPLE_GRID_LENGTH+1);
        vector<vector<simplegrid_t>> prefixes = manager.get_prefixes (closed, z0, centroids);

        // and also consider the edge (i, 1) -> (i+1, 0) and propagate the
        // backward g-values
        khs::centroid_t z1 = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i, 1)),
                                              closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, 0)),
                                              SIMPLE_GRID_LENGTH+1);
        prefixes = manager.get_prefixes (closed, z1, centroids);

        // verify the backward g-values are correct
        for (auto j = 1 ; j <= i ; j++) {

            // look for the node (j, 1) in the closed list
            auto ptr = closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, j, 1));

            // this node should have only backward g-value, equal to
            // SIMPLE_GRID_LENGTH - j + 1
            ASSERT_TRUE (closed[ptr].get_gb().size () == 1);
            ASSERT_TRUE (closed[ptr].get_gb ()[0] == SIMPLE_GRID_LENGTH-j+1);

            // look now for the node (j, 0) in the closed list
            ptr = closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, j, 0));

            // thi node should have no backward g-values
            ASSERT_TRUE (closed[ptr].get_gb().size () == 0);
        }

        // finally, check that the start state has only one g-value equal to
        // SIMPLE_GRID_LENGTH+1
        ASSERT_TRUE (closed[0].get_gb().size () == 1);
        ASSERT_TRUE (closed[0].get_gb ()[0] == SIMPLE_GRID_LENGTH+1);
    }
}

// Check that backward g-values are updated correctly in grids using two
// centroids
// ----------------------------------------------------------------------------
TEST_F (BELAFixture, UpdateBackwardgTwoCentroidsGrid) {

    khs::bucket_t<khs::centroid_t> centroids;

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SIMPLE_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::bela<grid_t> manager {k, start, goal};

    // Now, consider different combinations of edges (to be used as centroids)
    // and verify that the bucket of centroids is updated correctly. It is
    // impossible to verify all cases and just a selection is made next

    // First, consider the two incident edges (i-1, j)->(i,j) and (i,j-1)->(i,j)
    // in the interior of the grid. As a result, all nodes with coordinates
    // strictly less than i and j should have one signle backward g-value
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH-1 ; i++) {
        for (auto j = 1 ; j < SIMPLE_GRID_LENGTH-1 ; j++) {

            // First, populate a closed list with the expansions of all nodes in the
            // state space of a simple grid
            khs::closed_t<khs::labelednode_t<grid_t>> closed;
            populateClosed<grid_t> (closed, SIMPLE_GRID_LENGTH);

            // note the edge (i-1, j) -> (i, j) is not a true centroid as it
            // belongs to the optimal path to get to (i,j). Nevertheless,
            // get_prefixes should work much the same
            khs::centroid_t z0 = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, i-1, j)),
                                                 closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                                                 2*(SIMPLE_GRID_LENGTH-1));
            vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z0, centroids);

            // note the edge (i, j-1) -> (i, j) is not a true centroid as it
            // belongs to the optimal path to get to (i,j). Nevertheless,
            // get_prefixes should work much the same
            khs::centroid_t z1 = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j-1)),
                                                 closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                                                 2*(SIMPLE_GRID_LENGTH-1));

            prefixes = manager.get_prefixes (closed, z1, centroids);

            // verify that all nodes have correct backward g-values
            for (auto k = 0 ; k < SIMPLE_GRID_LENGTH ; k++) {
                for (auto l = 0 ; l < SIMPLE_GRID_LENGTH ; l++) {

                    // look for the node (k, l) in the closed list
                    auto ptr = closed.find (grid_t (SIMPLE_GRID_LENGTH, k, l));

                    // if this node is within the interior of the subgrid
                    // defined by (i, j) (excluding the vertex itself (i,j)),
                    // then it should contain only one single g-value, even if
                    // two centroids were used
                    if ( ((k <= i && l < j)) || (k < i && l <= j) ) {

                        // this node should have only backward g-value, equal to
                        // 2*SIMPLE_GRID_LENGTH - k - l - 2
                        ASSERT_TRUE (closed[ptr].get_gb().size () == 1);
                        ASSERT_TRUE (closed[ptr].get_gb ()[0] == 2*SIMPLE_GRID_LENGTH-k-l-2);
                    } else {

                        // otherwise, this node should contain no g-value at all
                        ASSERT_TRUE (closed[ptr].get_gb ().size () == 0);
                    }
                }
            }
        }
    }

    // Secondly, consider the following edges in the interior of the grid
    // (i,j-1)->(i,j) and (i,j)->(i-1,j). This configuration will force some
    // nodes to have zero g-values, others to have only one g-value and the rest
    // to have two different g-values
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH ; i++) {
        for (auto j = 1 ; j < SIMPLE_GRID_LENGTH ; j++) {

            // First, populate a closed list with the expansions of all nodes in the
            // state space of a simple grid
            khs::closed_t<khs::labelednode_t<grid_t>> closed;
            populateClosed<grid_t> (closed, SIMPLE_GRID_LENGTH);

            // note the edge (i, j-1) -> (i, j) is not a true centroid as it
            // belongs to the optimal path to get to (i,j). Nevertheless,
            // get_prefixes should work much the same
            khs::centroid_t z0 = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j-1)),
                                                 closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                                                 2*(SIMPLE_GRID_LENGTH-1));
            vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z0, centroids);

            // note the edge (i, j) -> (i-1, j) is a true centroid
            khs::centroid_t z1 = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SIMPLE_GRID_LENGTH, i-1, j)),
                                                 2*SIMPLE_GRID_LENGTH);

            prefixes = manager.get_prefixes (closed, z1, centroids);

            // verify that all nodes have correct backward g-values
            for (auto k = 0 ; k < SIMPLE_GRID_LENGTH ; k++) {
                for (auto l = 0 ; l < SIMPLE_GRID_LENGTH ; l++) {

                    // look for the node (k, l) in the closed list
                    auto ptr = closed.find (grid_t (SIMPLE_GRID_LENGTH, k, l));

                    // nodes outside the region delimited by (i,j)
                    // should have no g-values
                    if (k > i || l > j) {
                        ASSERT_TRUE (closed[ptr].get_gb ().size () == 0);
                    } else if (k <= i) {

                        if (l==j) {
                            // Nodes located at the straight line j (up to x=i)
                            // should have only one g-value, the one
                            // corresponding to the centroid (i,j)->(i-1,j)
                            ASSERT_TRUE (closed[ptr].get_gb ().size () == 1);
                            ASSERT_TRUE (closed[ptr].get_gb ()[0] == 2*SIMPLE_GRID_LENGTH-k-l);
                        } else {

                            // Nodes located in this are must have two backward
                            // g-values, each set by every centroid
                            ASSERT_TRUE (closed[ptr].get_gb ().size () == 2);
                            ASSERT_TRUE (closed[ptr].get_gb ()[0] == 2*SIMPLE_GRID_LENGTH-k-l-2);
                            ASSERT_TRUE (closed[ptr].get_gb ()[1] == 2*SIMPLE_GRID_LENGTH-k-l);
                        }
                    }
                }
            }
        }
    }
}

// Check that no new centroids are created in a simple grid when a prefix
// contains no bridge
// ------------------------------------------------------------------------
TEST_F (BELAFixture, NullCentroidSimpleGrid) {

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    simplegrid_t start = simplegrid_t (SIMPLE_GRID_LENGTH, 0, 0);
    simplegrid_t goal = simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::bela<simplegrid_t> manager {k, start, goal};

    // Nodes on the upper line have no bridges, and thus, any prefix whose start
    // state lies there should generate no new additional centroid
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH ; i++) {

        // First, populate a closed list with the expansions of all nodes in the
        // state space of a simple grid
        khs::closed_t<khs::labelednode_t<simplegrid_t>> closed;
        populateClosed<simplegrid_t> (closed, SIMPLE_GRID_LENGTH);

        // Start with an empty collection of centroids
        khs::bucket_t<khs::centroid_t> centroids;

        // first, create a centroid with the edge (i, 1) -> (i+1, 0) and
        // propagate the backward g-values
        khs::centroid_t z = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i, 1)),
                                             closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, 0)),
                                             SIMPLE_GRID_LENGTH+1);
        vector<vector<simplegrid_t>> prefixes = manager.get_prefixes (closed, z, centroids);

        // verify that no new centroids were created
        ASSERT_EQ (centroids.size (), 0);
    }
}

// Check that new centroids are created in the simple grid
// ------------------------------------------------------------------------
TEST_F (BELAFixture, NonNullCenntroidSimpleGrid) {

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    simplegrid_t start = simplegrid_t (SIMPLE_GRID_LENGTH, 0, 0);
    simplegrid_t goal = simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::bela<simplegrid_t> manager {k, start, goal};

    // Every node on the lower line has one bridge. Thus, when using the
    // centroid (i,0)-->(i+1,0), the enumeration of prefixes should discover up
    // to (i-1) new centroids
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH ; i++) {

        // First, populate a closed list with the expansions of all nodes in the
        // state space of a simple grid
        khs::closed_t<khs::labelednode_t<simplegrid_t>> closed;
        populateClosed<simplegrid_t> (closed, SIMPLE_GRID_LENGTH);

        // Start with an empty collection of centroids
        khs::bucket_t<khs::centroid_t> centroids;

        // first, create a centroid with the edge (i, 0) -> (i+1, 0) and
        // propagate the backward g-values
        khs::centroid_t z = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i, 0)),
                                             closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, 0)),
                                             SIMPLE_GRID_LENGTH);
        vector<vector<simplegrid_t>> prefixes = manager.get_prefixes (closed, z, centroids);

        // verify the correct number of centroids was created
        ASSERT_EQ (centroids.size (), i-1);

        // verify also that the centroids are correct
        while (centroids.size ()) {

            khs::centroid_t new_z = centroids.pop_front();

            // Certainly, I'm not checking that there are no duplicate centroids
            // because I already verified the number is correct. All is done
            // next is just simply to check that every centroid is correct

            // First, verify the cost is correct!
            ASSERT_TRUE (new_z.get_cost () == SIMPLE_GRID_LENGTH+1);

            // secondly, check the start and end vertices of the centroid are
            // also correct, i.e., that they are of the form (i, 1) -> (i+1, 0)
            khs::labelednode_t<simplegrid_t> start = closed[new_z.get_start ()];
            khs::labelednode_t<simplegrid_t> end = closed[new_z.get_end ()];
            ASSERT_TRUE (start.get_state ().get_y () == 1);
            ASSERT_TRUE (end.get_state ().get_y () == 0);
            ASSERT_EQ (end.get_state ().get_x () - start.get_state ().get_x (), 1);
        }
    }
}

// Check that new centroids are created in the grid domain
// ------------------------------------------------------------------------
TEST_F (BELAFixture, NonNullCentroidGrid) {

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SIMPLE_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH - 1, SIMPLE_GRID_LENGTH-1);
    khs::bela<grid_t> manager {k, start, goal};

    // Now, consider different combinations of edges (to be used as centroids)
    // and verify that the bucket of centroids is updated correctly. It is
    // impossible to verify all cases and just a selection is made next

    // First, consider the edges in the left line (0, i)->(0, i+1). In this
    // case, 2*(i+1) new centroids should be created: (1, i)->(0, i) and (0,
    // i+1)->(0,i) for each i, 0 included

    for (auto i = 0 ; i < SIMPLE_GRID_LENGTH-1 ; i++) {

        // First, populate a closed list with the expansions of all nodes in the
        // state space of a grid
        khs::closed_t<khs::labelednode_t<grid_t>> closed;
        populateClosed<grid_t> (closed, SIMPLE_GRID_LENGTH);

        // start with an empty collection of centroids
        khs::bucket_t<khs::centroid_t> centroids;

        // create the centroid and compute its prefixes
        khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, 0, i)),
                                              closed.find (grid_t (SIMPLE_GRID_LENGTH, 0, i+1)),
                                              2*(SIMPLE_GRID_LENGTH-1));
        vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z, centroids);

        // verify the correct number of centroids was created
        ASSERT_EQ (centroids.size (), 2*(i+1));

        // and verify also that they are all correct
        while (centroids.size ()) {

            khs::centroid_t new_z = centroids.pop_front ();

            // In this case, a centroid is correct if and only if it vertically
            // gets to any node in the lower line, or it is a horizontal edge
            // coming from x=1
            khs::labelednode_t<grid_t> start = closed[new_z.get_start ()];
            khs::labelednode_t<grid_t> end = closed[new_z.get_end ()];
            ASSERT_TRUE ( ((start.get_state ().get_x () == 0 && end.get_state ().get_x () == 0) &&
                           (start.get_state ().get_y () - end.get_state ().get_y () == 1)) ||
                          ((start.get_state ().get_y () == end.get_state ().get_y ()) &&
                           (start.get_state ().get_x () - end.get_state ().get_x () == 1)) );
        }
    }

    // Secondly, consider the case of edges which are *not* centroids, e.g., an
    // edge (i,j)->(i,j+1). This sort of edges when treated as centroids must
    // create 2*(i+1)(j+1) new centroids, two for each node in the interior of
    // the subgraph delimited by (0,0) and (i,j)
    for (auto i = 0 ; i < SIMPLE_GRID_LENGTH ; i++) {
        for (auto j = 0  ; j < SIMPLE_GRID_LENGTH-1 ; j++) {

            // First, populate a closed list with the expansions of all nodes in the
            // state space of a grid
            khs::closed_t<khs::labelednode_t<grid_t>> closed;
            populateClosed<grid_t> (closed, SIMPLE_GRID_LENGTH);

            // start with an empty collection of centroids
            khs::bucket_t<khs::centroid_t> centroids;

            // create the centroid and compute its prefixes
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j+1)),
                                                 2*(SIMPLE_GRID_LENGTH-1));
            vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z, centroids);

            // verify the correct number of centroids was created. Account for
            // the boundary effects. If i=SIMPLE_GRID_LENGTH-1, then there is no
            // incoming edge (i+1,j)->(i,j) and therefore it is necessary to
            // substract (j+1) values. Moreover, if i=SIMPLE_GRID_LENGTH-1 and
            // j=SIMPLE_GRID_LENGTH-2, so that we are sitting at the node
            // immediately adjacent to the goal, then it is necessary to
            // substract also another unit as the goal must create no new
            // centroid
            if (i == SIMPLE_GRID_LENGTH-1) {
                if (j == SIMPLE_GRID_LENGTH-2) {
                    ASSERT_EQ (centroids.size (), 2*(i+1)*(j+1)-j-2);
                } else {
                    ASSERT_EQ (centroids.size (), 2*(i+1)*(j+1)-j-1);
                }
            } else {
                ASSERT_EQ (centroids.size (), 2*(i+1)*(j+1));
            }
        }
    }

    // The last case considers the usage of a true centroid (i,j)->(i-1,j). In
    // this case 2*i*(j+1) new centroids must be created
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH-1 ; i++) {
        for (auto j = 0 ; j < SIMPLE_GRID_LENGTH ; j++) {

            // First, populate a closed list with the expansions of all nodes in the
            // state space of a grid
            khs::closed_t<khs::labelednode_t<grid_t>> closed;
            populateClosed<grid_t> (closed, SIMPLE_GRID_LENGTH);

            // start with an empty collection of centroids
            khs::bucket_t<khs::centroid_t> centroids;

            // create the centroid and compute its prefixes
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SIMPLE_GRID_LENGTH, i-1, j)),
                                                 2*(SIMPLE_GRID_LENGTH-1));
            vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z, centroids);

            // verify the correct number of centroids was created. Account for
            // the boundary effects. If j=SIMPLE_GRID_LENGTH-1, then there is no
            // incoming edge (i,j+1)->(i,j) and therefore it is necessary to
            // substract (i+1) values. Moreover, if j=SIMPLE_GRID_LENGTH-1 and
            // i=SIMPLE_GRID_LENGTH-2, so that we are sitting at the node
            // immediately adjacent to goal, then it is necessary to substract
            // also another unit as the goal must create no new centroid
            if (j == SIMPLE_GRID_LENGTH-1) {
                if (i == SIMPLE_GRID_LENGTH-2) {
                    ASSERT_EQ (centroids.size (), 2*(i+1)*(j+1)-i-2);
                } else {
                    ASSERT_EQ (centroids.size (), 2*(i+1)*(j+1)-i-1);
                }
            } else {
                ASSERT_EQ (centroids.size (), 2*(i+1)*(j+1));
            }
        }
    }
}

// Check that centroids that contain no suffix correctly return only the goal
// state in the simple grid
// -----------------------------------------------------------------------------
TEST_F (BELAFixture, NullSuffixSimpleGrid) {

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    simplegrid_t start = simplegrid_t (SIMPLE_GRID_LENGTH, 0, 0);
    simplegrid_t goal = simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::bela<simplegrid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a simple grid
    khs::closed_t<khs::labelednode_t<simplegrid_t>> closed;
    populateClosed<simplegrid_t> (closed, SIMPLE_GRID_LENGTH);

    // use the edge (SIMPLE_GRID_LENGTH-1, 0)->(SIMPLE_GRID_LENGTH) as a
    // centroid of all optimal paths getting to the goal through that edge
    khs::centroid_t z = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, 0)),
                                         closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0)),
                                         SIMPLE_GRID_LENGTH);
    khs::bucket_t<khs::centroid_t> centroids;
    vector<vector<simplegrid_t>> prefixes = manager.get_prefixes (closed, z, centroids);

    // next, compute the suffixes of the last edge (SIMPLE_GRID_LENGTH-1,
    // 0)->(SIMPLE_GRID_LENGTH)
    vector<vector<simplegrid_t>> suffixes = manager.get_suffixes (closed, z);

    // As a result, the backward g-value of the goal should be updated to 0, and
    // it should contain only one
    auto ptr = closed.find (goal);
    ASSERT_EQ (closed[ptr].get_gb ().size (), 1);
    ASSERT_EQ (closed[ptr].get_gb ()[0], 0);

    // verify also that the suffix is null, i.e., it consists of only one path
    // that contains the goal state
    ASSERT_EQ (suffixes.size (), 1);
    ASSERT_EQ (suffixes[0].size (), 1);
    ASSERT_EQ (suffixes[0][0], goal);
}

// Check that centroids that contain no suffix correctly return only the goal
// state in the grid domain
// -----------------------------------------------------------------------------
TEST_F (BELAFixture, NullSuffixGrid) {

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SIMPLE_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1);
    khs::bela<grid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::labelednode_t<grid_t>> closed;
    populateClosed<grid_t> (closed, SIMPLE_GRID_LENGTH);

    // use the last horizontal edge leading to the goal as a centroid of all
    // optimal paths getting to the goal through that edge
    khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-2, SIMPLE_GRID_LENGTH-1)),
                                         closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1)),
                                         2*(SIMPLE_GRID_LENGTH-1));
    khs::bucket_t<khs::centroid_t> centroids;
    vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z, centroids);

    // next, compute the suffixes of the last horizontal edge getting to the
    // goal
    vector<vector<grid_t>> suffixes = manager.get_suffixes (closed, z);

    // As a result, the backward g-value of the goal should be updated to 0, and
    // it should contain only one
    auto ptr = closed.find (goal);
    ASSERT_EQ (closed[ptr].get_gb ().size (), 1);
    ASSERT_EQ (closed[ptr].get_gb ()[0], 0);

    // verify also that the suffix is null, i.e., it consists of only one path
    // that contains the goal state
    ASSERT_EQ (suffixes.size (), 1);
    ASSERT_EQ (suffixes[0].size (), 1);
    ASSERT_EQ (suffixes[0][0], goal);
}

// Check that centroids that contain a single non-null suffix correctly compute
// it correctly in a simple grid
// -----------------------------------------------------------------------------
TEST_F (BELAFixture, NonNullSuffixSimpleGrid) {

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    simplegrid_t start = simplegrid_t (SIMPLE_GRID_LENGTH, 0, 0);
    simplegrid_t goal = simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::bela<simplegrid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a simple grid
    khs::closed_t<khs::labelednode_t<simplegrid_t>> closed;
    populateClosed<simplegrid_t> (closed, SIMPLE_GRID_LENGTH);

    // use the edge (SIMPLE_GRID_LENGTH-1, 0)->(SIMPLE_GRID_LENGTH) as a
    // centroid of all optimal paths getting to the goal through that edge
    khs::centroid_t z0 = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, 0)),
                                          closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0)),
                                          SIMPLE_GRID_LENGTH);
    khs::bucket_t<khs::centroid_t> centroids;
    vector<vector<simplegrid_t>> prefixes = manager.get_prefixes (closed, z0, centroids);

    // next, compute the suffixes of the last horizontal edge to get to the
    // goal. This step is necessary to update the backward g-value of the goal.
    vector<vector<simplegrid_t>> suffixes = manager.get_suffixes (closed, z0);

    // next, consider all horizontal edges in the lower line as centroids (even
    // if they are not centroids indeed) and verify that all suffixes are
    // correctly computed
    for (auto i=0 ; i < SIMPLE_GRID_LENGTH ; i++) {

        // consider the edge (i, 0)->(i+1, 0) and compute its suffixes
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
        // with x-values in the range [i+1, SIMPLE_GRID_LENGTH] and y=0
        ASSERT_EQ (suffixes.size (), 1);
        for (auto j = 0 ; j < suffixes[0].size () ;j++) {
            ASSERT_EQ (suffixes[0][j].get_x (), j+i+1);
            ASSERT_EQ (suffixes[0][j].get_y (), 0);
        }
    }

    // Also, all the diagonal edges (i, 1)->(i+1, 0) should create a single
    // suffix
    for (auto i=1 ; i < SIMPLE_GRID_LENGTH-1 ; i++) {

        // consider the edge (i, 0)->(i+1, 0) and compute its suffixes
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
        // with x-values in the range [i+1, SIMPLE_GRID_LENGTH] and y=0
        ASSERT_EQ (suffixes.size (), 1);
        for (auto j = 0 ; j < suffixes[0].size () ;j++) {
            ASSERT_EQ (suffixes[0][j].get_x (), j+i+1);
            ASSERT_EQ (suffixes[0][j].get_y (), 0);
        }
    }
}

// Check that centroids that contain a single non-null suffix correctly compute
// it correctly in the grid domain
// -----------------------------------------------------------------------------
TEST_F (BELAFixture, NonNullSuffixGrid) {

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SIMPLE_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1);
    khs::bela<grid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a simple grid
    khs::closed_t<khs::labelednode_t<grid_t>> closed;
    populateClosed<grid_t> (closed, SIMPLE_GRID_LENGTH);

    // use the last horizontal edge getting to the goal as a centroid of all
    // optimal paths getting to the goal through that edge
    khs::centroid_t z0 = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-2, SIMPLE_GRID_LENGTH-1)),
                                          closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1)),
                                          2*(SIMPLE_GRID_LENGTH-1));
    khs::bucket_t<khs::centroid_t> centroids;
    vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z0, centroids);

    // next, compute the suffixes of the last horizontal edge to get to the
    // goal. This step is necessary to update the backward g-value of the goal.
    vector<vector<grid_t>> suffixes = manager.get_suffixes (closed, z0);

    // next, consider all horizontal edges in the upper line as centroids (even
    // if they are not centroids indeed) and verify that all suffixes are
    // correctly computed
    for (auto i=0 ; i < SIMPLE_GRID_LENGTH-2 ; i++) {

        // consider the edge (i, SIMPLE_GRID_LENGTH-1)->(i+1,
        // SIMPLE_GRID_LENGTH-1) and compute its suffixes
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
        // y=SIMPLE_GRID_LENGTH-1
        ASSERT_EQ (suffixes.size (), 1);
        for (auto j = 0 ; j < suffixes[0].size () ;j++) {
            ASSERT_EQ (suffixes[0][j].get_x (), j+i+1);
            ASSERT_EQ (suffixes[0][j].get_y (), SIMPLE_GRID_LENGTH-1);
        }
    }
}

// Check that centroids that contain various non-null suffixes are correctly
// computed in a simple grid
// -----------------------------------------------------------------------------
TEST_F (BELAFixture, MultipleNonNullSuffixSimpleGrid) {

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    simplegrid_t start = simplegrid_t (SIMPLE_GRID_LENGTH, 0, 0);
    simplegrid_t goal = simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::bela<simplegrid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a simple grid
    khs::closed_t<khs::labelednode_t<simplegrid_t>> closed;
    populateClosed<simplegrid_t> (closed, SIMPLE_GRID_LENGTH);

    // First, paths of length SIMPLE_GRID_LENGTH must be discovered, and this
    // means that backward g-values should be propagated to all nodes in those
    // paths. Thus,use the edge (SIMPLE_GRID_LENGTH-1, 0)->(SIMPLE_GRID_LENGTH,
    // 0) as a centroid of all optimal paths getting to the goal through that
    // edge.
    khs::centroid_t z0 = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, 0)),
                                          closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0)),
                                          SIMPLE_GRID_LENGTH);
    khs::bucket_t<khs::centroid_t> centroids;
    vector<vector<simplegrid_t>> prefixes = manager.get_prefixes (closed, z0, centroids);
    vector<vector<simplegrid_t>> suffixes = manager.get_suffixes (closed, z0);

    // Secondly, to propagate the backward g-values to nodes in the upper line
    // it is necessary know to process a true centroid
    // (SIMPLE_GRID_LENGTH-1,1)-(SIMPLE_GRID_LENGTH,0)
    khs::centroid_t z1 = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, 1)),
                                          closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0)),
                                          SIMPLE_GRID_LENGTH+1);
    prefixes = manager.get_prefixes (closed, z1, centroids);
    suffixes = manager.get_suffixes (closed, z1);

    // At this point, any of the edges (i,1)->(i+1,1) for i in (0,
    // SIMPLE_GRID_LENGTH-1) should lead to several suffixes
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH-1 ; i++) {

        khs::centroid_t z = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i, 1)),
                                             closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, i+1, 1)),
                                             1+SIMPLE_GRID_LENGTH);
        khs::bucket_t<khs::centroid_t> centroids;
        vector<vector<simplegrid_t>> prefixes = manager.get_prefixes (closed, z, centroids);
        vector<vector<simplegrid_t>> suffixes = manager.get_suffixes (closed, z);

        // Next, verify the number of suffixes is correct
        ASSERT_EQ (suffixes.size (), SIMPLE_GRID_LENGTH-i-1);

        // and also that every suffix contains the right nodes
        for (auto j = 0 ; j < suffixes.size () ; j++) {
            for (auto k = 0 ; k < suffixes[j].size () ; k++) {
                ASSERT_EQ (suffixes[j][k].get_x (), k+i+1);

                // the verification on the y-coordinate is much trickier because
                // it depends on the order the suffixes are generated. I
                // intentionally disregard that and thus I'm only verifying the
                // rather trivial condition that y is either 0 or 1
                ASSERT_TRUE (suffixes[j][k].get_y () == 0 || suffixes[j][k].get_y () == 1 );
            }
        }
    }
}

// Check that centroids that contain various non-null suffixes are correctly
// computed in the grid domain
// -----------------------------------------------------------------------------
TEST_F (BELAFixture, MultipleNonNullSuffixGrid) {

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SIMPLE_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1);
    khs::bela<grid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::labelednode_t<grid_t>> closed;
    populateClosed<grid_t> (closed, SIMPLE_GRID_LENGTH);

    // First, paths of length 2*(SIMPLE_GRID_LENGTH-1) must be discovered, and
    // this means that backward g-values should be propagated to all nodes in
    // those paths. Thus, use the last horizontal and vertical edge to get to
    // the goal as centroids of all optimal paths getting to the goal through
    // those edges
    khs::centroid_t z0 = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-2, SIMPLE_GRID_LENGTH-1)),
                                          closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1)),
                                          2*(SIMPLE_GRID_LENGTH-1));
    khs::bucket_t<khs::centroid_t> centroids;
    vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z0, centroids);

    // Compute the suffixes of this centroid to ensure that the backward g-value
    // of the goal state gets updated int CLOSED
    vector<vector<grid_t>> suffixes = manager.get_suffixes (closed, z0);

    khs::centroid_t z1 = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-2)),
                                          closed.find (grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1)),
                                          2*(SIMPLE_GRID_LENGTH-1));
    prefixes = manager.get_prefixes (closed, z1, centroids);

    // Compute the suffixes of this centroid to ensure that the backward g-value
    // of the goal state gets updated int CLOSED ---though this is redundand
    // because it was already done above for the other centroid
    suffixes = manager.get_suffixes (closed, z1);

    // Next, verify the number of suffixes of any edge (i, j)->(i+1, j), i in
    // [1, SIMPLE_GRID_LENGTH-3], j in [1, SIMPLE_GRID_LENGTH-2] is correct
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH-2 ; i++) {
        for (auto j = 1 ; j < SIMPLE_GRID_LENGTH-1 ; j++) {

            // create the centroid and compute all its suffixes
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SIMPLE_GRID_LENGTH, i+1, j)),
                                                 2*(SIMPLE_GRID_LENGTH-1));
            vector<vector<grid_t>> suffixes = manager.get_suffixes (closed, z);

            // Next, verify the number of suffixes is correct, i.e., it is equal
            // to the binomial coefficient (2*SIMPLE_GRID_LENGTH - (i + j + 3))
            // choose (SIMPLE_GRID_LENGTH - i - 2)
            ASSERT_EQ (suffixes.size (), binomial_coefficient (2*SIMPLE_GRID_LENGTH - (i + j + 3),
                                                               SIMPLE_GRID_LENGTH - i - 2));
        }
    }

    // Well, repeat again but this time considering centroids (i, j)->(i, j+1)
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH-1 ; i++) {
        for (auto j = 1 ; j < SIMPLE_GRID_LENGTH-2 ; j++) {

            // create the centroid and compute all its suffixes
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j+1)),
                                                 2*(SIMPLE_GRID_LENGTH-1));
            vector<vector<grid_t>> suffixes = manager.get_suffixes (closed, z);

            // Next, verify the number of suffixes is correct, i.e., it is equal
            // to the binomial coefficient (2*SIMPLE_GRID_LENGTH - (i + j + 3))
            // choose (SIMPLE_GRID_LENGTH - i - 2)
            ASSERT_EQ (suffixes.size (), binomial_coefficient (2*SIMPLE_GRID_LENGTH - (i + j + 3),
                                                               SIMPLE_GRID_LENGTH - j - 2));
        }
    }

    // The last case involves the consideration of true centroids that lead to
    // suboptimal paths. In this case, consider the edges (i, j)->(i-1, j) for i
    // in [1, SIMPLE_GRID_LENGTH-2] and j in [1, SIMPLE_GRID_LENGTH-2]
    for (auto i = 1 ; i < SIMPLE_GRID_LENGTH-1 ; i++) {
        for (auto j = 1 ; j < SIMPLE_GRID_LENGTH-2 ; j++) {

            // create the centroid and compute all its suffixes
            khs::centroid_t z = khs::centroid_t (closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                                                 closed.find (grid_t (SIMPLE_GRID_LENGTH, i-1, j)),
                                                 2*SIMPLE_GRID_LENGTH);
            vector<vector<grid_t>> suffixes = manager.get_suffixes (closed, z);

            // Next, verify the number of suffixes is correct, i.e., it is equal
            // to the binomial coefficient 2*SIMPLE_GRID_LENGTH - i - j - 3
            // choose SIMPLE_GRID_LENGTH - 2 -i
            ASSERT_EQ (suffixes.size (), binomial_coefficient (2*SIMPLE_GRID_LENGTH - (i + j + 1),
                                                               SIMPLE_GRID_LENGTH - i));
        }
    }
}

// Check that centroids can be effectively used for returning optimal and
// suboptimal paths in the simple grid
// -----------------------------------------------------------------------------
TEST_F (BELAFixture, GetPathsSimpleGrid) {

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    simplegrid_t start = simplegrid_t (SIMPLE_GRID_LENGTH, 0, 0);
    simplegrid_t goal = simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::bela<simplegrid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a simple grid
    khs::closed_t<khs::labelednode_t<simplegrid_t>> closed;
    populateClosed<simplegrid_t> (closed, SIMPLE_GRID_LENGTH);

    // First, paths of length SIMPLE_GRID_LENGTH must be discovered, and this
    // means that backward g-values should be propagated to all nodes in those
    // paths. Thus,use the edge (SIMPLE_GRID_LENGTH-1, 0)->(SIMPLE_GRID_LENGTH,
    // 0) as a centroid of all optimal paths getting to the goal through that
    // edge.
    khs::centroid_t z0 = khs::centroid_t (closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, 0)),
                                          closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0)),
                                          SIMPLE_GRID_LENGTH);

    // compute all paths reprresented by this centroid
    khs::bucket_t<khs::centroid_t> centroids;
    khs::ksolution_t solutions = manager.get_paths (z0, closed, centroids);

    // and verify there is only one
    ASSERT_EQ (solutions.size (), 1);

    // verify also that every solution is correct
    for (auto i = 0 ; i < solutions.size () ; i++) {
        auto solution = solutions[i];
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

        // verify also that every solution is correct
        for (auto i = 0 ; i < solutions.size () ; i++) {
            auto solution = solutions[i];
            ASSERT_TRUE (solution.doctor ());
        }
    }
}

// Check that centroids can be effectively used for returning optimal paths in
// the grid domain
// -----------------------------------------------------------------------------
TEST_F (BELAFixture, GetOptimalPathsGrid) {

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SIMPLE_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1);
    khs::bela<grid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::labelednode_t<grid_t>> closed;
    populateClosed<grid_t> (closed, SIMPLE_GRID_LENGTH);

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

    // verify also that every solution is correct
    for (auto i = 0 ; i < solutions.size () ; i++) {
        auto solution = solutions[i];
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

    // verify also that every solution is correct
    for (auto i = 0 ; i < solutions.size () ; i++) {
        auto solution = solutions[i];
        ASSERT_TRUE (solution.doctor ());
    }
}

// Check that centroids can be effectively used for returning suboptimal paths
// in the grid domain
// -----------------------------------------------------------------------------
TEST_F (BELAFixture, GetSuboptimalPathsGrid) {

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SIMPLE_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH-1, SIMPLE_GRID_LENGTH-1);
    khs::bela<grid_t> manager {k, start, goal};

    // First, populate a closed list with the expansions of all nodes in the
    // state space of a grid
    khs::closed_t<khs::labelednode_t<grid_t>> closed;
    populateClosed<grid_t> (closed, SIMPLE_GRID_LENGTH);

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

    // Repeat the experiment but this time using the last vertical edge to get
    // to the goal
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

        // compute the number of prefixes and suffixes so that the expected
        // number of paths is their product
        int x0 = closed[z.get_start ()].get_state ().get_x ();
        int y0 = closed[z.get_start ()].get_state ().get_y ();
        int x1 = closed[z.get_end ()].get_state ().get_x ();
        int y1 = closed[z.get_end ()].get_state ().get_y ();
        auto nbprefixes = binomial_coefficient (x0+y0, x0);
        auto nbsuffixes = binomial_coefficient (SIMPLE_GRID_LENGTH-1-x1+SIMPLE_GRID_LENGTH-1-y1, SIMPLE_GRID_LENGTH-1-x1);
        ASSERT_EQ (solutions.size (), nbprefixes*nbsuffixes);
    }
}

// Checks that solvable instances where start=goal can be correctly solved
// ----------------------------------------------------------------------------
TEST_F (BELAFixture, SolvableSameNPancakeSolution) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a manager to run the BELA* search algorithm to solve a random
        // instance a random number of times. h=0 is given for the original node
        // because no search is performed
        int k = 1 + rand () % MAX_VALUES;
        npancake_t start = randInstance (NB_DISCS);
        khs::bela<npancake_t> manager {k, start, start};

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

// Check that BELA* correctly finds one single solution between two instances of
// the 5-Pancake
TEST_F (BELAFixture, SolvableNPancakeOne) {

        for (auto i = 0 ; i < NB_TESTS ; i++) {

            // create a manager to find a single solution between a couple of
            // random instances of the 5-Pancake
            int k = 1;
            npancake_t start = randInstance (5);
            npancake_t goal = randInstance (5);
            khs::bela<npancake_t> manager {k, start, goal};

            // initialize the static information of the n-pancake
            npancake_t::init ("unit");

            // and invoke the solver
            auto ksolution = manager.solve ();

            // verify the solution found contains one single solution
            ASSERT_EQ (ksolution.size (), k);

            // and verify it is correct
            ASSERT_TRUE (ksolution.doctor ());
        }
}

// Check that BELA* correctly finds two single solutions between two instances
// of the 5-Pancake
TEST_F (BELAFixture, SolvableNPancakeTwo) {

        for (auto i = 0 ; i < NB_TESTS ; i++) {

            // create a manager to find two solutions between a couple of random
            // instances of the 5-Pancake which are guaranteed to be different
            int k = 2;
            npancake_t start = randInstance (5);
            npancake_t goal = randInstance (5);
            while (start == goal) {
                goal = randInstance (5);
            }
            khs::bela<npancake_t> manager {k, start, goal};

            // initialize the static information of the n-pancake
            npancake_t::init ("unit");

            // and invoke the solver
            auto ksolution = manager.solve ();

            // verify the solution found contains two solutions
            ASSERT_EQ (ksolution.size (), k);

            // and verify they are correct
            ASSERT_TRUE (ksolution.doctor ());
        }
}

// Check that BELA* correctly finds an arbitrary number of solutions (10 <= k <=
// 20) between two instances of the 5-Pancake
TEST_F (BELAFixture, SolvableNPancakeArbitrary) {

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
            khs::bela<npancake_t> manager {k, start, goal};

            // initialize the static information of the n-pancake
            npancake_t::init ("unit");

            // and invoke the solver
            auto ksolution = manager.solve ();

            // verify the solution found contains k solutions
            ASSERT_EQ (ksolution.size (), k);

            // and verify they are correct
            ASSERT_TRUE (ksolution.doctor ());
        }
}

// Check that BELA* correctly finds one single solution between two instances of
// the 5-Pancake in the heavy-cost variant
TEST_F (BELAFixture, SolvableHeavyCostNPancakeOne) {

        for (auto i = 0 ; i < NB_TESTS ; i++) {

            // create a manager to find a single solution between a couple of
            // random instances of the 5-Pancake
            int k = 1;
            npancake_t start = randInstance (5);
            npancake_t goal = randInstance (5);
            khs::bela<npancake_t> manager {k, start, goal};

            // initialize the static information of the n-pancake
            npancake_t::init ("heavy-cost");

            // and invoke the solver
            auto ksolution = manager.solve ();

            // verify the solution found contains one single solution
            ASSERT_EQ (ksolution.size (), k);

            // and verify it is correct
            ASSERT_TRUE (ksolution.doctor ());
        }
}

// Check that BELA* correctly finds two single solutions between two instances
// of the 5-Pancake in the heavy-cost variant
TEST_F (BELAFixture, SolvableHeavyCostNPancakeTwo) {

        for (auto i = 0 ; i < NB_TESTS ; i++) {

            // create a manager to find two solutions between a couple of random
            // instances of the 5-Pancake which are guaranteed to be different
            int k = 2;
            npancake_t start = randInstance (3);
            npancake_t goal = randInstance (3);
            while (start == goal) {
                goal = randInstance (3);
            }
            khs::bela<npancake_t> manager {k, start, goal};

            // initialize the static information of the n-pancake
            npancake_t::init ("heavy-cost");

            // and invoke the solver
            auto ksolution = manager.solve ();

            // verify the solution found contains two solutions
            ASSERT_EQ (ksolution.size (), k);

            // and verify they are correct
            ASSERT_TRUE (ksolution.doctor ());
        }
}

// Check that BELA* correctly finds an arbitrary number of solutions (10 <= k <=
// 20) between two instances of the 5-Pancake in the heavy-cost variant
TEST_F (BELAFixture, SolvableHeavyCostNPancakeArbitrary) {

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
            khs::bela<npancake_t> manager {k, start, goal};

            // initialize the static information of the n-pancake
            npancake_t::init ("heavy-cost");

            // and invoke the solver
            auto ksolution = manager.solve ();

            // verify the solution found contains k solutions
            ASSERT_EQ (ksolution.size (), k);

            // and verify they are correct
            ASSERT_TRUE (ksolution.doctor ());
        }
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
