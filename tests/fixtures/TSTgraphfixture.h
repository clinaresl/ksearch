// -*- coding: utf-8 -*-
// TSTgraphfixture.h
// -----------------------------------------------------------------------------
//
// Started on <lun 07-08-2023 20:42:14.769600532 (1691433734)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Fixture for testing the graph_t class
//

#ifndef _TSTGRAPHFIXTURE_H_
#define _TSTGRAPHFIXTURE_H_

#include <algorithm>
#include <random>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../TSThelpers.h"
#include "../../domains/roadmap/graph_t.h"

// Class definition
//
// Defines a Google test fixture for testing the graph_t class
class GraphFixture : public ::testing::Test {

protected:

    void SetUp () override {

        // just initialize the random seed to make sure that every iteration is
        // performed over different random data
        srand (time (nullptr));
    }
};

#endif // _TSTGRAPHFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
