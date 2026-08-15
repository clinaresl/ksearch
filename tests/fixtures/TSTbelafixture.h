// -*- coding: utf-8 -*-
// TSTbelafixture.h
// -----------------------------------------------------------------------------
//
// Started on <vie 11-08-2023 21:10:13.271272100 (1691781013)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

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
#include "../../domains/n-pancake/npancake_t.h"

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

    // verify the instance given first is a descendant of the second one. Also,
    // verify the cost given third is correct
    template<typename T>
    bool verify_descendant (T& descendant,
                            T& ancestor,
                            int cost) {

        // Now, expand this node and verify if the given descendant is found
        // among them with the application of an operator that has exactly the
        // given cost. Admittedly, the following approach forces all descendants
        // to be verified, even if the descendant of interest was found early,
        // but my current implementation of the children does not allow stopping
        // the generation of successor.
        bool found = false;
        ancestor.children (
            0,
            ancestor,
            [&] (int g, int h, T&& successor) {

                // if the given descendant is found, then verify the cost is correct
                // and return
                if (successor == descendant) {

                    // verify the cost is correct
                    found = (cost == g);
                }                
            });

        // and return whether it was found or not
        return found;
    }

    // update the backward g-value of a node in closed
    template <template <typename> class T, typename D>
    void update_gbvalue (const T<D>& node, const int gb, khs::closed_t<T<D>>& closed) {

        // First, look for this node in closed
        size_t ptr = closed.find (node);

        // and add the given backward g-value unless it is already stored
        if (not closed[ptr].find_gb (gb)) {
            closed[ptr] += gb;
        }
    }
};


#endif // _TSTBELAFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
