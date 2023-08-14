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

        // every node along the upper line, but the corners, should have three
        // backpointers
        if (state.get_y () == SIMPLE_GRID_LENGTH-1 && state.get_x () > 0 && state.get_x () < SIMPLE_GRID_LENGTH -1) {
            ASSERT_TRUE (closed[i].get_backpointers ().size () == 3);

            // verify all backpointers, i.e., that they are indeed parents of
            // this node and also that the cost registered in closed is correct
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

        // every node along the right vertical line, but the corners, should have
        // three backpointers
        if (state.get_x () == SIMPLE_GRID_LENGTH-1 && state.get_y () > 0 && state.get_y () < SIMPLE_GRID_LENGTH -1) {
            ASSERT_TRUE (closed[i].get_backpointers ().size () == 3);

            // verify all backpointers, i.e., that they are indeed parents of
            // this node and also that the cost registered in closed is correct
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
TEST_F (BELAFixture, UpdateBarckwardgOneCentroidSimpleGrid) {

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
TEST_F (BELAFixture, UpdateBarckwardgOneCentroidGrid) {

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
TEST_F (BELAFixture, UpdateBarckwardgTwoCentroidsSimpleGrid) {

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
TEST_F (BELAFixture, UpdateBarckwardgTwoCentroidsGrid) {

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
                            if (closed[ptr].get_gb ().size () != 1) {
                                cout << closed[ptr] << endl;
                            }
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
    grid_t goal = grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
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
            // substract (j+1) values
            if (i == SIMPLE_GRID_LENGTH-1) {
                ASSERT_EQ (centroids.size (), 2*(i+1)*(j+1)-j-1);
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

            // while (centroids.size ()) {
            //     auto new_z = centroids.pop_front ();
            //     manager.show_centroid (cout, new_z, closed);
            // }

            // verify the correct number of centroids was created. Account for
            // the boundary effects. If j=SIMPLE_GRID_LENGTH-1, then there is no
            // incoming edge (i,j+1)->(i,j) and therefore it is necessary to
            // substract (i+1) values
            if (j == SIMPLE_GRID_LENGTH-1) {
                ASSERT_EQ (centroids.size (), 2*(i+1)*(j+1)-i-1);
            } else {
                ASSERT_EQ (centroids.size (), 2*(i+1)*(j+1));
            }
        }
    }
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
