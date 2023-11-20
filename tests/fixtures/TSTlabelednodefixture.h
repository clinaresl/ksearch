// -*- coding: utf-8 -*-
// TSTlabelednodefixture.h
// -----------------------------------------------------------------------------
//
// Started on <vie 11-08-2023 19:28:07.498167128 (1691774887)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

//
// Fixture used for testing the class labelednode_t
//

#ifndef _TSTLABELEDNODEFIXTURE_H_
#define _TSTLABELEDNODEFIXTURE_H_

#include<cstdlib>
#include<ctime>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../TSThelpers.h"
#include "../../src/structs/KHSlabelednode_t.h"

// Class definition
//
// Defines a Google test fixture for testing labeled nodes
class LabeledNodeFixture : public ::testing::Test {

    protected:

        void SetUp () override {

            // just initialize the random seed to make sure that every iteration
            // is performed over different random data
            srand (time (nullptr));
        }
};

#endif // _TSTLABELEDNODEFIXTURE_H_


// Local Variables:
// mode:cpp
// fill-column:80
// End:
