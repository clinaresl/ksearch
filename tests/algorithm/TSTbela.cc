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
    vector<vector<simplegrid_t>> prefixes = manager.get_prefixes (closed, z0);

    // verify there is only one prefix, which consists of the start state itself
    ASSERT_TRUE (prefixes.size () == 1);
    ASSERT_TRUE (prefixes[0].size () == 1);
    ASSERT_TRUE (prefixes[0][0] == start);

    khs::centroid_t z1 = khs::centroid_t (closed.find (start),
                                          closed.find (simplegrid_t (SIMPLE_GRID_LENGTH, 1, 1)),
                                          1);
    prefixes = manager.get_prefixes (closed, z1);

    // verify there is only one prefi, which consists of the start state itselfx
    ASSERT_TRUE (prefixes.size () == 1);
    ASSERT_TRUE (prefixes[0].size () == 1);
    ASSERT_TRUE (prefixes[0][0] == start);
}

// Check that centroids with null prefixes return only one path in the grid
// domain, the start state itself
// ----------------------------------------------------------------------------
TEST_F (BELAFixture, NullPrefixGrid) {

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
    vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z0);

    // verify there is only one prefix, which consists of the start state itself
    ASSERT_TRUE (prefixes.size () == 1);
    ASSERT_TRUE (prefixes[0].size () == 1);
    ASSERT_TRUE (prefixes[0][0] == start);

    khs::centroid_t z1 = khs::centroid_t (closed.find (start),
                                          closed.find (grid_t (SIMPLE_GRID_LENGTH, 0, 1)),
                                          1);
    prefixes = manager.get_prefixes (closed, z1);

    // verify there is only one prefi, which consists of the start state itselfx
    ASSERT_TRUE (prefixes.size () == 1);
    ASSERT_TRUE (prefixes[0].size () == 1);
    ASSERT_TRUE (prefixes[0][0] == start);
}

// Verify that the number of non-null prefixes is correct in the simple grid domain
// ----------------------------------------------------------------------------
TEST_F (BELAFixture, NonNullPrefixSimpleGrid) {

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
        vector<vector<simplegrid_t>> prefixes = manager.get_prefixes (closed, z);

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
        vector<vector<simplegrid_t>> prefixes = manager.get_prefixes (closed, z);

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
        vector<vector<simplegrid_t>> prefixes = manager.get_prefixes(closed, z);

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
        vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z);

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
        vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z);

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
                prefixes = manager.get_prefixes (closed, east);

                // verify the number of optimal paths is equal to the binomial
                // coefficient of (i+j) choose j
                ASSERT_EQ (prefixes.size (), binomial_coefficient (i+j, j));
            }

            // (i, j) -> (i, j+1)
            if (j < SIMPLE_GRID_LENGTH-1) {
                khs::centroid_t north {closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                    closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j+1)), 1};
                prefixes = manager.get_prefixes (closed, north);

                // verify the number of optimal paths is equal to the binomial
                // coefficient of (i+j) choose i
                ASSERT_EQ (prefixes.size (), binomial_coefficient (i+j, i));
            }

            // (i, j) -> (i-1, j)
            khs::centroid_t west {closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                closed.find (grid_t (SIMPLE_GRID_LENGTH, i-1, j)), 1};
            prefixes = manager.get_prefixes (closed, west);

            // verify the number of optimal paths is equal to the binomial
            // coefficient of (i+j) choose j
            ASSERT_EQ (prefixes.size (), binomial_coefficient (i+j, j));

            // (i, j) -> (i, j-1)
            khs::centroid_t south {closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j)),
                closed.find (grid_t (SIMPLE_GRID_LENGTH, i, j-1)), 1};
            prefixes = manager.get_prefixes (closed, south);

            // verify the number of optimal paths is equal to the binomial
            // coefficient of (i+j) choose i
            ASSERT_EQ (prefixes.size (), binomial_coefficient (i+j, i));
        }
    }
}

// Check that backward g-values are updated correctly in simple grids using one centroid
// ----------------------------------------------------------------------------
TEST_F (BELAFixture, UpdateBarckwardgOneCentroidSimpleGrid) {

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    simplegrid_t start = simplegrid_t (SIMPLE_GRID_LENGTH, 0, 0);
    simplegrid_t goal = simplegrid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::bela<simplegrid_t> manager {k, start, goal};

    // Now, consider different edges (to be used as centroids) and verify that
    // the closed list is updated correctly

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
        vector<vector<simplegrid_t>> prefixes = manager.get_prefixes (closed, z);

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
        vector<vector<simplegrid_t>> prefixes = manager.get_prefixes (closed, z);

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
        vector<vector<simplegrid_t>> prefixes = manager.get_prefixes (closed, z);

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

    // create a manager to execute BELA*
    int k = rand () % MAX_VALUES;
    grid_t start = grid_t (SIMPLE_GRID_LENGTH, 0, 0);
    grid_t goal = grid_t (SIMPLE_GRID_LENGTH, SIMPLE_GRID_LENGTH, 0);
    khs::bela<grid_t> manager {k, start, goal};

    // Now, consider different edges (to be used as centroids) and verify that
    // the closed list is updated correctly

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
        vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z);

        // verify the backward g-values are correct
        for (auto j = 0 ; j <= i ; j++) {

            // look for the node (0, j) in the closed list
            auto ptr = closed.find (grid_t{SIMPLE_GRID_LENGTH, 0, j});

            // the node (0, j) should have only one backward g-value equal to
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
        vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z);

        // verify the backward g-values are correct
        for (auto j = 0 ; j <= i ; j++) {

            // look for the node (0, j) in the closed list
            auto ptr = closed.find (grid_t{SIMPLE_GRID_LENGTH, j, 0});

            // the node (0, j) should have only one backward g-value equal to
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
           vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z);

           // verify the backward g-values are correct
           for (auto k = 0 ; k <= i ; k++) {
               for (auto l = 0 ; l <= j ; l++) {

                   // look for the node (0, j) in the closed list
                   auto ptr = closed.find (grid_t{SIMPLE_GRID_LENGTH, k, l});

                   // the node (0, j) should have only one backward g-value equal to
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
           vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z);

           // verify the backward g-values are correct
           for (auto k = 0 ; k <= i ; k++) {
               for (auto l = 0 ; l <= j ; l++) {

                   // look for the node (0, j) in the closed list
                   auto ptr = closed.find (grid_t{SIMPLE_GRID_LENGTH, k, l});

                   // the node (0, j) should have only one backward g-value equal to
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
           vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z);

           // verify the backward g-values are correct
           for (auto k = 0 ; k <= i ; k++) {
               for (auto l = 0 ; l <= j-1 ; l++) {

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
           vector<vector<grid_t>> prefixes = manager.get_prefixes (closed, z);

           // verify the backward g-values are correct
           for (auto k = 0 ; k <= i-1 ; k++) {
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



// Local Variables:
// mode:cpp
// fill-column:80
// End:
