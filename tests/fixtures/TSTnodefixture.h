// -*- coding: utf-8 -*-
// TSTnodefixture.h
// -----------------------------------------------------------------------------
//
// Started on <mar 06-12-2022 14:41:23.389235885 (1670334083)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

//
// Test fixture for testing nodes
//

#ifndef _TSTNODEFIXTURE_H_
#define _TSTNODEFIXTURE_H_

#include<cstdlib>
#include<ctime>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../TSThelpers.h"
#include "../../src/structs/KHSnode_t.h"

// Class definition
//
// Defines a Google test fixture for testing nodes
class NodeFixture : public ::testing::Test {

    protected:

        void SetUp () override {

            // just initialize the random seed to make sure that every iteration
            // is performed over different random data
            srand (time (nullptr));
        }
};

#endif // _TSTNODEFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
