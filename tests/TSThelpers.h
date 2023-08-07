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
template <typename T>
std::vector<khs::backpointer_t<T>> randVectorBackpointer (int n) {

    // create a vector of backpointers
    std::vector<khs::backpointer_t<T>> backpointers;

    // create a random number generator
    std::random_device rd;
    std::mt19937 gen (rd ());
    std::uniform_int_distribution<> dis (0, MAX_VALUE);

    // create n backpointers
    for (auto i = 0 ; i < n ; i++) {
        auto bp = khs::backpointer_t<T> {size_t (dis (gen)), size_t (dis (gen))};
        backpointers.push_back (bp);
    }

    return backpointers;
}

#endif // _TSTHELPERS_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
