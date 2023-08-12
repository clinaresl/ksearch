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

// Check that the closed list to be used in the following tests is correctly
// created
// ----------------------------------------------------------------------------
TEST_F (BELAFixture, ClosedListSimpleGrid) {

    // First, populate a closed list with the expansions of all nodes in the
    // state space of simple grid
    khs::closed_t<khs::labelednode_t<simplegrid_t>> closed;
    populateClosed (closed, SIMPLE_GRID_LENGTH);

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
            ASSERT_TRUE (verify_descendant (state, parent, 1));
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
                ASSERT_TRUE (verify_descendant (state, parent, 1));
                ASSERT_TRUE (parent == closed[closed[i].get_backpointer (0).get_pointer()].get_state ());

                // and the second edge gets to them from the node in the upper
                // line
                parent = simplegrid_t {SIMPLE_GRID_LENGTH, state.get_x () - 1, 1};
                ASSERT_TRUE (verify_descendant (state, parent, 2));
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

// Check that centroids with null prefixes return no path
// ----------------------------------------------------------------------------
TEST_F (BELAFixture, NullPrefixSimpleGrid) {

    // First, populate a closed list with the expansions of all nodes in the
    // state space of simple grid
    khs::closed_t<khs::labelednode_t<simplegrid_t>> closed;
    populateClosed (closed, SIMPLE_GRID_LENGTH);
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
