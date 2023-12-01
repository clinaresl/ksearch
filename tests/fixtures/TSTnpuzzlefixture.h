// -*- coding: utf-8 -*-
// TSTnpuzzlefixture.h
// -----------------------------------------------------------------------------
//
// Started on <dom 27-02-2022 23:46:41.253352711 (1646002001)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Test fixture for testing the sliding tile npuzzle
//

#ifndef _TSTNPUZZLEFIXTURE_H_
#define _TSTNPUZZLEFIXTURE_H_

#include<algorithm>
#include<random>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../../domains/n-puzzle/npuzzle_t.h"

// Class definition
//
// Defines a Google test fixture for testing the npuzzle
class NPuzzleFixture : public ::testing::Test {

protected:

    void SetUp () override {

        // just initialize the random seed to make sure that every iteration is
        // performed over different random data
        srand (time (nullptr));
    }

    // create a random instance of the N-puzzle with N being a perfect square
    // number
    const npuzzle_t randInstance (const int n) {

        // first, create the identity permutation
        std::vector<int> goal;
        for (auto i = 0 ; i < n ; goal.push_back (i++));

        // and just shuffle the goal. Certainly, there is no guarantee this is a
        // solvable instance but that doesn't matter!
        std::random_device rd;
        std::mt19937 generator (rd());
        std::shuffle (goal.begin (), goal.end (), generator);

        // and return the random instance
        return npuzzle_t {goal};
    }
};

#endif // _TSTNPUZZLEFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
