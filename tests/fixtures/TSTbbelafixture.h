// -*- coding: utf-8 -*-
// TSTbbelafixture.h
// -----------------------------------------------------------------------------
//
// Started on <mar 01-07-2025 19:41:03.258963721 (1751391663)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Fixture used for testing the baseline simplistic BELA*
//

#ifndef _TSTBBELAFIXTURE_H_
#define _TSTBBELAFIXTURE_H_

#include<algorithm>
#include<ctime>
#include<unordered_set>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../TSThelpers.h"
#include "../../domains/n-pancake/npancake_t.h"
#include "../../src/algorithm/KHSbbela.h"

// Class definition
//
// Defines a Google test fixture for testing the baseline simplistic sBELA*
// search algorithm
class bBELAFixture : public ::testing::Test {

protected:

    void SetUp () override {

        // just initialize the random seed to make sure that every iteration is
        // performed over different random data
        srand (time (nullptr));
    }

    // update the backward g-value of a node in closed
    template <template <typename> class T, typename D>
    void update_gbvalue (const T<D>& node, const int gb, khs::closed_t<T<D>>& closed) {

        // First, look for this node in closed
        size_t ptr = closed.find (node);

        // and add the given backward g-value unless it is already stored
        if (not closed[ptr].find_gb (gb)) {
            closed[ptr] += gb;
        }
    }
};

#endif // _TSTBBELAFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
