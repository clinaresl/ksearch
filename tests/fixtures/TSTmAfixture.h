// -*- coding: utf-8 -*-
// tstmAfixture.h
// -----------------------------------------------------------------------------
//
// Started on <jue 10-08-2023 17:00:13.506157634 (1691679613)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Fixture for testing the mA search algorithm
//

#ifndef _TSTMAFIXTURE_H_
#define _TSTMAFIXTURE_H_

#include<cstdlib>
#include<ctime>
#include<utility>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../TSThelpers.h"
#include "../../domains/n-pancake/npancake_t.h"
#include "../../src/algorithm/KHSmA.h"

// Class definition
//
// Defines a Google test fixture for testing the mA search algorithm
class MAFixture : public ::testing::Test {

protected:

    void SetUp () override {

        // just initialize the random seed to make sure that every iteration is
        // performed over different random data
        srand (time (nullptr));
    }

};

#endif // _TSTMAFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
