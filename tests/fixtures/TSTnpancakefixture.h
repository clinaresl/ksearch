// -*- coding: utf-8 -*-
// TSTnpancakefixture.h
// -----------------------------------------------------------------------------
//
// Started on <lun 07-08-2023 20:42:14.769600532 (1691433734)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

//
// Fixture for testing the npancake_t class
//

#ifndef _TSTNPANCAKEFIXTURE_H_
#define _TSTNPANCAKEFIXTURE_H_

#include<algorithm>
#include<random>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../TSThelpers.h"
#include "../../domains/n-pancake/npancake_t.h"

// Class definition
//
// Defines a Google test fixture for testing the npancake_t class
class NPancakeFixture : public ::testing::Test {

protected:

    void SetUp () override {

        // just initialize the random seed to make sure that every iteration is
        // performed over different random data
        srand (time (nullptr));
    }

    // given two instances of the N-Pancake, it returns the number of discs of
    // the first one to flip to get the second one. In case the second instance
    // is not a descendant of the first one, 0 is returned
    const int getPrefix (const npancake_t& i1, const npancake_t& i2) {

        // first, find the location in the second instance of the first disc of
        // the first instance
        int length = 0;
        while (i2 [length] != i1 [0]) length++;

        // and now verify that all discs in the range [0, length] are flipped
        for (auto i = 0 ; i <= length ; i++) {
            if (i1 [i] != i2 [length - i]) return 0;
        }

        // at this point, the second instance is known to be a descendant of the
        // first one where the first length+1 discs have been flipped
        return length + 1;
    }
};

#endif // _TSTNPANCAKEFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
