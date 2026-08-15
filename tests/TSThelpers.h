// -*- coding: utf-8 -*-
// TSThelpers.h
// -----------------------------------------------------------------------------
//
// Started on <lun 07-08-2023 16:06:48.172249590 (1691417208)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

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

// definition of a struct used to generate a list of numbers of the desired
// length. succ () returns the next integer index, starting from 1
struct index_t {
    int _index;
    index_t () : _index {0}{}
    int operator () () {return ++_index;}
};

template<typename T>
void print_vec (std::vector<T> vec) {
    for (auto item : vec) {
        std::cout << item << " ";
    }
}

// Return the binomial coefficient of n choose k
size_t binomial_coefficient (int n, int k);

// Generate a random string with length characters in the sequence ASCII(32) -
// ASCII(126) which do not appear in the string exclude
std::string randString (int length, std::string exclude="");

// Generate a random string with a number of characters randomly selected in the
// range [l0, l1) in the sequence ASCII(32) - ASCII(126) which do not appear in
// the string exclude
std::string randString (int l0, int l1, std::string exclude="");

// return a vector with a list of numbers from 0 up to n-1
std::vector<int> succListInt (int n);

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

// determine whether the given path is simple, i.e., it contains no duplicates
template<typename D>
bool isSimplePath (const std::vector<D>& path) {

    // The procedure used here has quadratic complexity which is much worse than
    // the procedure used in the implementation of the libksearch library. The
    // reason why a different procedure is implemented here is to ensure that
    // bugs are spotted!
    for (auto i = 0 ; i < path.size () ; i++) {
        for (auto j = i+1; j < path.size () ; j++) {

            // in case there are two items which are equal then return false
            // immediately
            if (path[i] == path[j]) {
                return false;
            }
        }
    }

    // At this point, the path is known to be simple
    return true;
}

// Populate a closed list with the expansions of the full state space of the
// domain given in T with the given length (which is a property of the domain, N)
template <template<typename> class T, typename D>
void populateClosed (khs::closed_t<T<D>>& closed, int length) {

    // expand nodes in best-first order. For this, use an open list to store all
    // nodes generated
    khs::bucket_t<T<D>> open;

    // and populate it with the start state with f=g=0 and a null back pointer
    T<D> start {D {length, 0, 0}};
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

            // but insert it into closed first, in case it is not stored yet.
            // The reason is that when computing suffixes, the goal has to be
            // found in closed.
            auto ptr = closed.find (node);
            if (ptr == std::string::npos) {
                ptr = closed.insert (node);
            } else {

                // if the node did not exist in CLOSED, it is copied into it.
                // This means, the goal node is copied with its backpointer. In
                // case it already exists, all that is required is just to
                // update its backpointers
                closed[ptr] += node.get_backpointer (0);
            }

            // and proceed with the next node in OPEN
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
        state.children (
            0,
            node.get_state (),
            [&] (int g, int h, D&& successor) {

                // create a backnode with this successor. Note that the h value is
                // dismissed
                T<D> onode{std::move (successor), 0, node.get_g () + g};

                // set the backpointer to the location of its parent in CLOSED
                // at the last index
                onode += khs::labeledbackpointer_t{ptr, g};

                // and add it to OPEN using f=g
                open.insert (onode, onode.get_g ());
            });
    }
}

#endif // _TSTHELPERS_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
