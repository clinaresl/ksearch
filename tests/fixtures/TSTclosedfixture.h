// -*- coding: utf-8 -*-
// TSTclosedfixture.h
// -----------------------------------------------------------------------------
//
// Started on <lun 07-08-2023 19:51:04.521583125 (1691430664)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Fixture for testing the closed_t class
//

#ifndef _TSTCLOSEDFIXTURE_H_
#define _TSTCLOSEDFIXTURE_H_

#include<algorithm>
#include<cstdlib>
#include<ctime>
#include<random>
#include<vector>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../TSThelpers.h"
#include "../../src/structs/KHSclosed_t.h"
#include "../../src/structs/KHSbacknode_t.h"

// Class definition
//
// Defines a Google test fixture for testing closed lists
class ClosedFixture : public ::testing::Test {

protected:

    void SetUp () override {

        // just initialize the random seed to make sure that every iteration
        // is performed over different random data
        srand (time (nullptr));
    }
};

#endif // _TSTCLOSEDFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
