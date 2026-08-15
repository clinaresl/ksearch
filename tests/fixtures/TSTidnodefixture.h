// -*- coding: utf-8 -*-
// TSTidnodefixture.h
// -----------------------------------------------------------------------------
//
// Started on <lun 02-06-2025 13:41:14.877091538 (1748864474)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Fixture node for testing the class idnode_t
//

#ifndef _TSTIDNODEFIXTURE_H_
#define _TSTIDNODEFIXTURE_H_

#include<cstdlib>
#include<ctime>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../TSThelpers.h"
#include "../../src/structs/KHSidnode_t.h"

// Class definition
//
// Defines a Google test fixture for testing labeled nodes
class IdNodeFixture : public ::testing::Test {

    protected:

        void SetUp () override {

            // just initialize the random seed to make sure that every iteration
            // is performed over different random data
            srand (time (nullptr));
        }
};

#endif // _TSTIDNODEFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
