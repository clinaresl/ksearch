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

        // expand the ancestor and get all its children
        std::vector<std::tuple<int, int, T>> successors;
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

    // show a solution path on the console
    template<typename T>
    void show_solution (std::vector<T>& path) {

        // show the solution path
        for (auto& node: path) {
            std::cout << node << " ";
        }
    }

    // show a bunch of solutions
    template<typename T>
    void show_solutions (std::vector<std::vector<T>> paths) {

        // show the solution paths after telling the user the number of paths found
        std::cout << " # paths: " << paths.size () << std::endl;
        for (auto& path: paths) {
            show_solution (path);
            std::cout << std::endl;
        }
    }
};


#endif // _TSTBELAFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
