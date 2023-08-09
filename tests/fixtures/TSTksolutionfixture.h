// -*- coding: utf-8 -*-
// TSTksolutionfixture.h
// -----------------------------------------------------------------------------
//
// Started on <mié 09-08-2023 14:06:30.837278054 (1691582790)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Fixture for testing the ksolution_t class
//

#ifndef _TSTKSOLUTIONFIXTURE_H_
#define _TSTKSOLUTIONFIXTURE_H_

#include<algorithm>
#include<cstdlib>
#include<ctime>
#include<vector>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../TSThelpers.h"
#include "../../src/structs/KHSksolution_t.h"
#include "../../domains/n-pancake/npancake_t.h"

// Class definition
//
// Defines a Google test fixture for testing the ksolution_t class
class KSolutionFixture : public ::testing::Test {

protected:

    void SetUp () override {

        // just initialize the random seed to make sure that every iteration
        // is performed over different random data
        srand (time (nullptr));
    }
};


#endif // _TSTKSOLUTIONFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
