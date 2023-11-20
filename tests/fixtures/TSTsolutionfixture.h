// -*- coding: utf-8 -*-
// TSTsolutionfixture.h
// -----------------------------------------------------------------------------
//
// Started on <mié 09-08-2023 11:48:44.146700171 (1691574524)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

//
// Fixture for testing the solution_t class
//

#ifndef _TSTSOLUTIONFIXTURE_H_
#define _TSTSOLUTIONFIXTURE_H_

#include<algorithm>
#include<cstdlib>
#include<ctime>
#include<vector>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../TSThelpers.h"
#include "../../src/structs/KHSsolution_t.h"
#include "../../domains/n-pancake/npancake_t.h"

// Class definition
//
// Defines a Google test fixture for testing the solution_t class
class SolutionFixture : public ::testing::Test {

protected:

    void SetUp () override {

        // just initialize the random seed to make sure that every iteration
        // is performed over different random data
        srand (time (nullptr));
    }
};


#endif // _TSTSOLUTIONFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
