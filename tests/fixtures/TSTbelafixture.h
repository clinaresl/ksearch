// -*- coding: utf-8 -*-
// TSTbelafixture.h
// -----------------------------------------------------------------------------
//
// Started on <vie 11-08-2023 21:10:13.271272100 (1691781013)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Fixture used for testing the BELA* search algorithm
//

#ifndef _TSTBELAFIXTURE_H_
#define _TSTBELAFIXTURE_H_

#include<cstdlib>
#include<ctime>
#include<utility>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../TSThelpers.h"
#include "../../src/structs/KHSclosed_t.h"
#include "../../domains/n-pancake/npancake_t.h"
#include "../../src/algorithm/KHSbela.h"

// Class definition
//
// Defines a Google test fixture for testing the BELA* search algorithm
class BELAFixture : public ::testing::Test {

protected:

    void SetUp () override {

        // just initialize the random seed to make sure that every iteration is
        // performed over different random data
        srand (time (nullptr));
    }

    // verify the given instance of simplegrid is a descendant of the second
    // instance. Also, verify the cost given third is correct also
    bool verify_descendant (simplegrid_t& descendant,
                            simplegrid_t& ancestor,
                            int cost) {

        // expand the ancestor and get all its children
        std::vector<std::tuple<int, int, simplegrid_t>> successors;
        ancestor.children (0, ancestor, successors);

        // and now look among all descendants to see if the given one appears
        // among them
        for (auto& successor: successors) {

            // if the given descendant is found, then verify the cost is correct
            // and return
            if (std::get<2> (successor) == descendant) {

                // verify the cost is correct
                return cost == std::get<0> (successor);
            }
        }

        // at this point, the successor has not been found, ...
        return false;
    }
};


#endif // _TSTBELAFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
