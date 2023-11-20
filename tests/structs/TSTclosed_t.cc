// -*- coding: utf-8 -*-
// TSTclosed_t.cc
// -----------------------------------------------------------------------------
//
// Started on <lun 07-08-2023 19:53:36.099937775 (1691430816)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

//
// Unit tests for testing the closed_t class
//

#include "../fixtures/TSTclosedfixture.h"

using namespace std;

// checks that closed lists are properly created
// ----------------------------------------------------------------------------
TEST_F (ClosedFixture, Empty) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create an empty closed list (of ints) and verify its size is zero
        khs::closed_t<int> closed;
        ASSERT_EQ (closed.size (), 0);
    }
}

// checks that instances of the N-Pancake can be inserted in closed lists
// ----------------------------------------------------------------------------
TEST_F (ClosedFixture, BackNPancakeInsertion) {

    for (auto i = 0 ; i < NB_TESTS/10 ; i++) {

        // create an empty closed list (of n-pancakes)
        khs::closed_t<khs::backnode_t<npancake_t>> closed;

        // create a random number of different nodes of npancakes. Since the
        // closed list does not store duplicates make sure that all nodes are
        // unique
        auto values = randNBackNodes (1 + rand () % MAX_VALUES, NB_DISCS);

        // Insert them all into closed
        int idx = 0;
        for (auto value : values) {
            auto ptr = closed.insert (value);

            // check this item is inserted next to the last one
            ASSERT_EQ (ptr, idx);

            // and verify the size is correct
            idx++;
            ASSERT_EQ (closed.size (), idx);
        }
    }
}

// checks that lookups work correctly in closed lists of nodes of n-pancakes
// ----------------------------------------------------------------------------
TEST_F (ClosedFixture, BackNPancakeLookup) {

    for (auto i = 0 ; i < NB_TESTS/10 ; i++) {

        // create an empty closed list (of n-pancakes)
        khs::closed_t<khs::node_t<npancake_t>> closed;

        // create a random number of diffferent nodes of npancakes. Since the
        // closed list does not store duplicates make sure that all nodes are
        // unique
        auto values = randNBackNodes (2*(1 + rand () % MAX_VALUES), NB_DISCS);

        // Insert only half of the nodes in the closed list
        auto idx = 0;
        for (auto item : values) {
            closed.insert (item);

            // and verify the size is correct
            idx++;
            ASSERT_EQ (closed.size (), idx);

            // in case that half of the nodes have been already processed, exit
            if (idx >= values.size ()/2) {
                break;
            }
        }

        // Now, ensure that all those nodes that have been inserted are actually
        // found. Likewise, that those not being inserted are not found
        idx = 0;
        for (auto item : values) {

            // lookup for this value in closed
            auto lookup = closed.find (item);

            // inserted values
            if (idx < values.size ()/2) {

                // verify the information returned is correct
                ASSERT_TRUE (lookup != string::npos);
                ASSERT_EQ (closed[lookup], item);
            } else {

                // unexisting values. Note that when an entry does not exist
                // there is no need to check the value of the iterator
                ASSERT_TRUE (lookup == string::npos);
            }

            // and increment the number of nodes processed so far
            idx++;
        }
    }
}

// Check that nodes in closed can be looked up and their information correctly
// retrieved
// ----------------------------------------------------------------------------
TEST_F (ClosedFixture, BackNPancakeInformation) {

    for (auto i = 0 ; i < NB_TESTS/10 ; i++) {

        // create an empty closed list (of n-pancakes)
        khs::closed_t<khs::node_t<npancake_t>> closed;

        // create a random number of diffferent nodes of npancakes. Since the
        // closed list does not store duplicates make sure that all nodes are
        // unique
        auto values = randNBackNodes (1 + rand () % MAX_VALUES, NB_DISCS);

        // generate random g- and h-values and insert them in each backnode
        vector<int> gvalues, hvalues;
        for (auto i = 0 ; i < values.size () ; i++) {
            gvalues.push_back (rand () % MAX_VALUES);
            hvalues.push_back (rand () % MAX_VALUES);
            values[i].set_g (gvalues.back ());
            values[i].set_h (hvalues.back ());
        }

        // Insert all nodes in the closed list
        auto idx = 0;
        for (auto item : values) {
            closed.insert (item);

            // and verify the size is correct
            idx++;
            ASSERT_EQ (closed.size (), idx);
        }

        // Now, look for every node in the closed list and verify that its g-
        // and h-values are correct
        for (auto idx = 0 ; idx < values.size () ; idx++) {

            // lookup for this backnode in closed
            auto ptr = closed.find (values[idx]);

            // and verify their g- and h-values are correct
            ASSERT_EQ (closed[ptr].get_g (), gvalues[idx]);
            ASSERT_EQ (closed[ptr].get_h (), hvalues[idx]);
        }
    }
}

// Check that nodes in the closed list are indeed inserted sequentially
// ----------------------------------------------------------------------------
TEST_F (ClosedFixture, BackNPancakeSequential) {

    for (auto i = 0 ; i < NB_TESTS/10 ; i++) {

        // create an empty closed list (of n-pancakes)
        khs::closed_t<khs::node_t<npancake_t>> closed;

        // create a random number of diffferent nodes of npancakes. Since the
        // closed list does not store duplicates make sure that all nodes are
        // unique
        auto values = randNBackNodes (1+rand () % MAX_VALUES, NB_DISCS);

        // Insert them all into closed and also in a vector of backnodes of
        // n-pancakes. Nodes in closed are inserted in the private vector in
        // sequential order so that the same ordering should be followed by the
        // vector.
        vector<khs::node_t<npancake_t>> nodes;
        for (auto value : values) {
            auto ptr = closed.insert (value);
            nodes.push_back (value);

            // ensure that nodes have been correctly inserted in the vector
            // nodes
            ASSERT_EQ (ptr, nodes.size ()-1);
        }

        // Now, traverse the vector of nodes to verify that they are inserted in
        // the expected order
        for (auto idx = 0 ; idx < nodes.size () ; idx++) {

            // check that nodes in closed are stored sequentially
            ASSERT_EQ (nodes[idx], closed[idx]);
        }
    }
}

// Check that nodes in closed consistently store pointers to themselves within
// the closed list
// ----------------------------------------------------------------------------
TEST_F (ClosedFixture, BackNPancakePointers) {

    for (auto i = 0 ; i < NB_TESTS/10 ; i++) {

        // create an empty closed list (of n-pancakes)
        khs::closed_t<khs::node_t<npancake_t>> closed;

        // create a random number of diffferent nodes of npancakes. Since the
        // closed list does not store duplicates make sure that all nodes are
        // unique
        auto values = randNBackNodes (1+rand () % MAX_VALUES, NB_DISCS);

        // Insert them all into closed
        for (auto& value : values) {

            // the following statement actually sets the pointer of a node. Note
            // the usage of const_cast to ensure that the pointer can be set
            closed.insert (const_cast<khs::backnode_t<npancake_t>&>(value));
        }

        // ensure that pointers of the random instances are correctly set. First
        // accessing the closed list by reference
        for (auto& value : values) {

            // find this item in closed, which is known to exist!
            auto ptr = closed.find (value);
            ASSERT_TRUE (ptr != string::npos);

            // now, ensure that both the item pointed to by the pointer and the
            // result of looking it up are the same
            ASSERT_EQ (value.get_pointer (), ptr);
        }

        // Secondly, accesing the closed list by index
        for (auto idx = 0 ; idx < closed.size () ; idx++) {
            ASSERT_EQ (idx, closed[idx].get_pointer ());
        }
    }
}

// Check that nodes in closed properly store a backpointer to their parents
// ----------------------------------------------------------------------------
TEST_F (ClosedFixture, BackNPancakeBackPointers) {

    for (auto i = 0 ; i < NB_TESTS/10 ; i++) {

        // create an empty closed list (of n-pancakes)
        khs::closed_t<khs::backnode_t<npancake_t>> closed;

        // simulate a path just by randomly selecting backnodes each one with a
        // different instance of a N-Pancake
        auto path = randNBackNodes (MAX_PATH_LENGTH, NB_DISCS);

        // Insert them all into closed
        for (auto i = 0 ; i < path.size () ; i++) {

            // The first backnode is inserted with a null backpointer, whereas
            // the rest refer to the previous item (this is true because nodes
            // in closed are given pointers as indices to a private list
            // starting from zero)
            size_t backpointer = (i == 0) ? string::npos : i-1;

            // set the backpointer of the i-th node and inserted into the closed
            // list
            khs::backpointer_t bp{backpointer, 0};
            path[i] += bp;
            closed.insert (path[i]);
        }

        // Try now to reconstruct the path from the last node in the simulated
        // path just by traversing the closed list. Because there is no
        // backpointer to the last item in the path, it has to be manually
        // added. Use a deque because it is necessary to push by the front
        std::deque<khs::backnode_t<npancake_t>> reconstructed;
        reconstructed.push_front (path.back ());

        // find in closed the last node in the path
        auto goal = closed[closed.find (path.back ())];

        // Note that every node in closed should have only backpointer
        auto backpointers = goal.get_backpointers ();
        ASSERT_EQ (backpointers.size (), 1);
        auto bp = backpointers[0];

        // and traverse the closed list reconstructing the original path
        while (bp.get_pointer() != string::npos) {
            reconstructed.push_front (closed[bp.get_pointer ()]);

            // and find its immediate ancestor. In passing verify this node only
            // has one backpointer
            backpointers = closed[bp.get_pointer ()].get_backpointers ();
            ASSERT_EQ (backpointers.size (), 1);
            bp = backpointers[0];
        }

        // now, verify that both the path and the reconstructed path are
        // strictly equal
        ASSERT_EQ (path.size (), reconstructed.size ());
        for (auto i = 0 ; i < path.size () ; i++) {
            ASSERT_EQ (path[i], reconstructed[i]);
        }
    }
}



// Local Variables:
// mode:cpp
// fill-column:80
// End:
