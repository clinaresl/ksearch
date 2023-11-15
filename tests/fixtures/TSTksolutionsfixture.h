// -*- coding: utf-8 -*-
// TSTksolutionsfixture.h
// -----------------------------------------------------------------------------
//
// Started on <mié 09-08-2023 14:42:05.692773389 (1691584925)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Fixture for testing the ksolutions_t class
//

#ifndef _TSTKSOLUTIONSFIXTURE_H_
#define _TSTKSOLUTIONSFIXTURE_H_

#include<algorithm>
#include<cstdlib>
#include<ctime>
#include<vector>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../TSThelpers.h"
#include "../../src/structs/KHSksolutions_t.h"
#include "../../domains/n-pancake/npancake_t.h"

// Class definition
//
// Defines a Google test fixture for testing the ksolutions_t class
class KSolutionsFixture : public ::testing::Test {

protected:

    void SetUp () override {

        // just initialize the random seed to make sure that every iteration
        // is performed over different random data
        srand (time (nullptr));
    }

    // Generate a solution of the k-shortest path problem for the specified
    // value of k. The length of each solution is randomly chosen and the
    // solution generated is not necessarily correct.
    khs::ksolution_t<npancake_t, std::vector> randKSolution (const int k) {

        // randomly create a start and goal states
        npancake_t start = randInstance (NB_DISCS);
        npancake_t goal = randInstance (NB_DISCS);

        // create a container for storing k solutions of the k-shortest path
        // problem
        khs::ksolution_t<npancake_t, std::vector> ksolution{k, start, goal};

        // initialize the unit cost model
        npancake_t::init ("unit");

        // populate the solution of the k-shortest path problem with k paths
        while (ksolution.size () < k) {

            // and randomly generate a path from it with a random length
            std::pair<std::vector<npancake_t>, int> randWalk = randPath (start, 10 + rand () % (MAX_PATH_LENGTH - 10));

            // explicitly create other data for populating this solution
            int k = rand () % MAX_VALUE;
            std::vector<npancake_t> path = randWalk.first;
            int nbcentroids = rand () % MAX_VALUE;
            int h0 = rand () % MAX_VALUE;
            int cost = randWalk.second;
            size_t expansions = 1 + rand () % MAX_VALUE;
            double cpu_time = static_cast<double>(rand () % MAX_VALUE);
            std::string solver = randString (50);

            // Create a solution which stores all this information
            khs::solution_t<npancake_t, std::vector> solution (k, path, start, goal, nbcentroids,
                                                               h0, cost, expansions, cpu_time, solver);

            // and add it to the set of solutions of the k-shortest path problem
            ksolution += solution;
        }

        // and return all the solutions of the simulated k-shortest path problem
        return ksolution;
    }

};


#endif // _TSTKSOLUTIONSFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
