// -*- coding: utf-8 -*-
// TSTsolutionfixture.h
// -----------------------------------------------------------------------------
//
// Started on <mié 09-08-2023 11:48:44.146700171 (1691574524)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

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

    // Generate a correct path from the given start state with the specified
    // number of transitions. It also returns the cost of the solution found
    // under the cost model used in the initialization of the init table outside
    // this function
    const std::pair<std::vector<npancake_t>, int> randPath (const npancake_t& start, const int length) {

        int cost = 0;
        std::vector<npancake_t> path;

        // add the start state to the path
        path.push_back (start);

        // starting from the given start state create a descendant and add it to
        // the path
        while (path.size () < length) {

            // compute all children of the last state in the path
            std::vector<std::tuple<int, int, npancake_t>> successors;
            path.back ().children (0, start, successors);

            // and randomly select one
            int idx = rand () % successors.size ();

            // and add it to the path and update the cost
            path.push_back (std::get<2> (successors[idx]));
            cost += std::get<0> (successors[idx]);
        }

        // and finally return a pair with the path and the cost
        return std::make_pair (path, cost);
    }
};


#endif // _TSTSOLUTIONFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
