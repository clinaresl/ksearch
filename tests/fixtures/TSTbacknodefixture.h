// -*- coding: utf-8 -*-
// TSTbacknodefixture.h
// -----------------------------------------------------------------------------
//
// Started on <lun 07-08-2023 17:14:38.798084455 (1691421278)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

//
// Test fixture for testing backnodes
//

#ifndef _TSTBACKNODEFIXTURE_H_
#define _TSTBACKNODEFIXTURE_H_

#include<cstdlib>
#include<ctime>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../TSThelpers.h"
#include "../../src/structs/KHSbacknode_t.h"

// Class definition
//
// Defines a Google test fixture for testing backnodes
class BackNodeFixture : public ::testing::Test {

    protected:

        void SetUp () override {

            // just initialize the random seed to make sure that every iteration
            // is performed over different random data
            srand (time (nullptr));
        }
};

#endif // _TSTBACKNODEFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
