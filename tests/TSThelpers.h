// -*- coding: utf-8 -*-
// TSThelpers.h
// -----------------------------------------------------------------------------
//
// Started on <lun 07-08-2023 16:06:48.172249590 (1691417208)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Helper functions for all tests
//
#ifndef _TSTHELPERS_H_
#define _TSTHELPERS_H_

#include<algorithm>
#include<random>
#include<set>
#include<string>
#include<vector>

#include "TSTdefs.h"
#include "../src/structs/KHSnode_t.h"
#include "../src/structs/KHSbacknode_t.h"
#include "../src/structs/KHSlabelednode_t.h"
#include "../src/structs/KHSclosed_t.h"
#include "../src/structs/KHSbucket_t.h"
#include "../domains/n-pancake/npancake_t.h"

// Return the binomial coefficient of n choose k
size_t binomial_coefficient (int n, int k);

// Generate a random string with length characters in the sequence ASCII(32) -
// ASCII(126) which do not appear in the string exclude
std::string randString (int length, std::string exclude="");

// Generate a random string with a number of characters randomly selected in the
// range [l0, l1) in the sequence ASCII(32) - ASCII(126) which do not appear in
// the string exclude
std::string randString (int l0, int l1, std::string exclude="");

// return a vector with n numbers randomly generated in the interval [0, m). If
// remove_duplicates takes the value true, then no values are duplicated
std::vector<int> randVectorInt (int n, int m, bool remove_duplicates=false);

// return a vector with n strings randomly generated each with m chars. If
// remove_duplicates takes the value true, then no values are duplicated
std::vector<std::string> randVectorString (int n, int m, bool remove_duplicates=false);

// return a vector with n backpointers, each one randomly created with two
// values in the range [0, MAX_VALUE)
std::vector<khs::backpointer_t> randVectorBackpointer (int n);

// return a vector with n labeledbackpointers, each one randomly created with
// two values in the range [0, MAX_VALUE)
std::vector<khs::labeledbackpointer_t> randVectorLabeledBackpointer (int n);

// create a random instance of the N-Pancake with the given length
const npancake_t randInstance (int length);

// Generate a correct path from the given start state with the specified number
// of transitions. It also returns the cost of the solution found under the cost
// model used in the initialization of the init table outside this function
const std::pair<std::vector<npancake_t>, int> randPath (const npancake_t& start, const int length);

// Populate a closed list with the expansions of the full state space of the
// domain given in T with the given length (which is a property of the domain, N)
template <typename T>
void populateClosed (khs::closed_t<khs::labelednode_t<T>>& closed, int length) {


    // expand nodes in best-first order. For this, use an open list to store all
    // nodes generated
    khs::bucket_t<khs::labelednode_t<T>> open;

    // and populate it with the start state with f=g=0 and a null back pointer
    khs::labelednode_t<T> start {T {length, 0, 0}};
    start += khs::labeledbackpointer_t {std::string::npos, 0};
    open.insert (start, 0);

    // expand all nodes until the open list becomes empty
    while (open.size ()) {

        // get the next node to expand
        auto node = open.pop_front ();
        auto state = node.get_state ();

        // in case this node is the goal, just continue because this node has no
        // children
        if (state.is_goal ()) {
            continue;
        }

        // Check whether this node has been expanded before or not
        auto ptr = closed.find (node);

        // In case it has never been expanded
        if (ptr == std::string::npos) {

            // Then add it to CLOSED for the first time. Note that the new node
            // in CLOSED contains only one labeled backpointer, the one stored
            // in OPEN
            ptr = closed.insert (node);
        } else {

            // Otherwise, if the node already exists in CLOSED, then simply add
            // its labeled backpointer, with information about its parent and
            // the cost of the operator that generated
            closed[ptr] += node.get_backpointer (0);

            // and skip the expansion of this node!
            continue;
        }

        // expand the current node ---disregarding both the h-value of this node
        // and the goal
        std::vector<std::tuple<int, int, T>> successors;
        state.children (0, node.get_state (), successors);

        // and insert them into the open list
        for (auto& successor : successors) {

            // create a backnode with this successor. Note that the h value is
            // dismissed
            khs::labelednode_t<T> onode{std::get<2>(successor), 0, node.get_g () + std::get<0>(successor)};

            // set the backpointer to the location of its parent in CLOSED
            // at the last index
            auto bps = closed[ptr].get_backpointers ();
            onode += khs::labeledbackpointer_t{ptr, std::get<0>(successor)};

            // and add it to OPEN using f=g
            open.insert (onode, onode.get_g ());
        }
    }
}

#endif // _TSTHELPERS_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
