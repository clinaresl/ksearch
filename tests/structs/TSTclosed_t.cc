// -*- coding: utf-8 -*-
// TSTclosed_t.cc
// -----------------------------------------------------------------------------
//
// Started on <lun 07-08-2023 19:53:36.099937775 (1691430816)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

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

// // checks that instances of the sliding-tile puzzle can be inserted in closed
// // lists
// // ----------------------------------------------------------------------------
// TEST_F (ClosedFixture, NPuzzleInsertion) {

//     for (auto i = 0 ; i < NB_TESTS/10 ; i++) {

//         // create an empty closed list (of n-puzzles)
//         khs::closed_t<khs::node_t<npuzzle_t>> closed;

//         // create a random number of different nodes of npuzzles. Since the
//         // closed list does not store duplicates make sure that all nodes are
//         // unique
//         auto values = randNNodes<npuzzle_t> (1 + rand () % MAX_VALUES, NB_TILES);

//         // Insert them all into closed
//         int idx = 0;
//         for (auto value : values) {
//             auto ptr = closed.insert (value);

//             // check this item is inserted next to the last one
//             ASSERT_EQ (ptr, idx);

//             // and verify the size is correct
//             idx++;
//             ASSERT_EQ (closed.size (), idx);
//         }
//     }
// }

// // checks that lookups work correctly in closed lists of nodes of n-puzzles
// // ----------------------------------------------------------------------------
// TEST_F (ClosedFixture, NPuzzleLookup) {

//     for (auto i = 0 ; i < NB_TESTS/10 ; i++) {

//         // create an empty closed list (of n-puzzles)
//         khs::closed_t<khs::node_t<npuzzle_t>> closed;

//         // create a random number of diffferent nodes of npuzzles. Since the
//         // closed list does not store duplicates make sure that all nodes are
//         // unique
//         auto values = randNNodes<npuzzle_t> (2*(1 + rand () % MAX_VALUES), NB_TILES);

//         // Insert only half of the nodes in the closed list
//         auto idx = 0;
//         for (auto item : values) {
//             closed.insert (item);

//             // and verify the size is correct
//             idx++;
//             ASSERT_EQ (closed.size (), idx);

//             // in case that half of the nodes have been already processed, exit
//             if (idx >= values.size ()/2) {
//                 break;
//             }
//         }

//         // Now, ensure that all those nodes that have been inserted are actually
//         // found. Likewise, that those not being inserted are not found
//         idx = 0;
//         for (auto item : values) {

//             // lookup for this value in closed
//             auto lookup = closed.find (item);

//             // inserted values
//             if (idx < values.size ()/2) {

//                 // verify the information returned is correct
//                 ASSERT_TRUE (lookup != string::npos);
//                 ASSERT_EQ (closed[lookup], item);
//             } else {

//                 // unexisting values. Note that when an entry does not exist
//                 // there is no need to check the value of the iterator
//                 ASSERT_TRUE (lookup == string::npos);
//             }

//             // and increment the number of nodes processed so far
//             idx++;
//         }
//     }
// }

// // Check that nodes in the closed list are indeed inserted sequentially
// // ----------------------------------------------------------------------------
// TEST_F (ClosedFixture, NPuzzleSequential) {

//     for (auto i = 0 ; i < NB_TESTS/10 ; i++) {

//         // create an empty closed list (of n-puzzles)
//         khs::closed_t<khs::node_t<npuzzle_t>> closed;

//         // create a random number of diffferent nodes of npuzzles. Since the
//         // closed list does not store duplicates make sure that all nodes are
//         // unique
//         auto values = randNNodes<npuzzle_t> (1+rand () % MAX_VALUES, NB_TILES);

//         // Insert them all into closed and populate a vector of nodes of
//         // n-puzzles with the same information. Nodes in closed are inserted in
//         // the private vector in sequential order so that the same ordering is
//         // followed here
//         vector<khs::node_t<npuzzle_t>> nodes;
//         for (auto value : values) {
//             auto ptr = closed.insert (value);
//             nodes.push_back (value);

//             // ensure that nodes have been correctly inserted in the vector
//             // nodes
//             ASSERT_EQ (ptr, nodes.size ()-1);
//         }

//         // Now, traverse the vector of nodes to verify that they are inserted in
//         // the expected order
//         for (auto idx = 0 ; idx < nodes.size () ; idx++) {

//             // check that nodes in closed are stored sequentially
//             ASSERT_EQ (nodes[idx], closed[idx]);
//         }
//     }
// }

// // Check that nodes in closed consistently store pointers to themselves within
// // the closed list
// // ----------------------------------------------------------------------------
// TEST_F (ClosedFixture, NPuzzlePointers) {

//     for (auto i = 0 ; i < NB_TESTS/10 ; i++) {

//         // create an empty closed list (of n-puzzles)
//         khs::closed_t<khs::node_t<npuzzle_t>> closed;

//         // create a random number of diffferent nodes of npuzzles. Since the
//         // closed list does not store duplicates make sure that all nodes are
//         // unique
//         auto values = randNNodes<npuzzle_t> (1+rand () % MAX_VALUES, NB_TILES);

//         // Insert them all into closed
//         for (auto& value : values) {

//             // the following statement actually sets the pointer of a node. Note
//             // the usage of const_cast to ensure that the pointer can be set
//             closed.insert (const_cast<khs::node_t<npuzzle_t>&>(value));
//         }

//         // ensure that pointers of the random instances are correctly set. First
//         // accessing the closed list by reference
//         for (auto& value : values) {

//             // find this item in closed, which is known to exist!
//             auto ptr = closed.find (value);
//             ASSERT_TRUE (ptr != string::npos);

//             // now, ensure that both the item pointed to by the pointer and the
//             // result of looking it up are the same
//             ASSERT_EQ (value.get_pointer (), ptr);
//         }

//         // Secondly, accesing the closed list by index
//         for (auto idx = 0 ; idx < closed.size () ; idx++) {
//             ASSERT_EQ (idx, closed[idx].get_pointer ());
//         }
//     }
// }

// // Check that nodes in closed properly store a backpointer to their parents
// // ----------------------------------------------------------------------------
// TEST_F (ClosedFixture, NPuzzleBackPointers) {

//     for (auto i = 0 ; i < NB_TESTS/1000 ; i++) {

//         // create an empty closed list (of n-puzzles)
//         khs::closed_t<khs::node_t<npuzzle_t>> closed;

//         // create a random number of diffferent nodes of npuzzles. Since the
//         // closed list does not store duplicates make sure that all nodes are
//         // unique
//         auto values = randNNodes<npuzzle_t> (1+rand () % MAX_VALUES, NB_TILES);

//         // Insert them all into closed using a null backpointer
//         for (auto& value : values) {

//             // use a null backpointer because these nodes are the root of paths
//             // randomly generated
//             const_cast<khs::node_t<npuzzle_t>&>(value).set_backpointer (string::npos);
//             closed.insert (const_cast<khs::node_t<npuzzle_t>&>(value));
//         }

//         // initialize internal data for correctly and efficiently handling
//         // n-puzzles
//         npuzzle_t::initops ();
//         npuzzle_t::init ();

//         // extend the current closed list with paths randomly generated from
//         // each node previously inserted into the closed list. These nodes must
//         // also have their backpointer properly set
//         for (auto value : values) {

//             // generate a path from this node
//             generatePath<npuzzle_t>(value, closed, 1 + rand () % MAX_PATH_LENGTH);
//         }

//         // Check that nodes in closed either have a null backpointer or they
//         // have a backpointer to a node so that they are children of that node
//         auto nbparents = 0;
//         for (auto idx = 0 ; idx < closed.size () ; idx++) {

//             // get the idx-th node
//             auto node = closed[idx];

//             // if the backpointer of this node is null, then there is nothing to
//             // check but count the number of such nodes
//             if (node.get_backpointer () == string::npos) {
//                 nbparents++;
//             } else {

//                 // first of all, ensure that the pointer to the parent is within
//                 // bounds
//                 ASSERT_TRUE (node.get_backpointer () < closed.size ());

//                 // otherwise, ensure that this node is indeed a child of the
//                 // node pointed to by the backpointer
//                 auto parent = closed[node.get_backpointer ()];
//                 auto children = descendants<npuzzle_t>(parent);
//                 ASSERT_TRUE (find (children.begin (), children.end (), node) != children.end ());
//             }
//         }

//         // finally, ensure that the number of parents is precisely the number of
//         // nodes randomly generated
//         ASSERT_EQ (values.size (), nbparents);
//     }
// }



// Local Variables:
// mode:cpp
// fill-column:80
// End:
