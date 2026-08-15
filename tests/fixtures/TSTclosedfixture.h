// -*- coding: utf-8 -*-
// TSTclosedfixture.h
// -----------------------------------------------------------------------------
//
// Started on <lun 07-08-2023 19:51:04.521583125 (1691430664)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

//
// Fixture for testing the closed_t class
//

#ifndef _TSTCLOSEDFIXTURE_H_
#define _TSTCLOSEDFIXTURE_H_

#include<algorithm>
#include<cstdlib>
#include<ctime>
#include<deque>
#include<random>
#include<vector>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../TSThelpers.h"
#include "../../src/structs/KHSclosed_t.h"
#include "../../src/structs/KHSbacknode_t.h"
#include "../../src/structs/KHSidnode_t.h"
#include "../../src/algorithm/KHSmA.h"
#include "../../domains/n-pancake/npancake_t.h"

// Class definition
//
// Defines a Google test fixture for testing closed lists
class ClosedFixture : public ::testing::Test {

protected:

    void SetUp () override {

        // just initialize the random seed to make sure that every iteration
        // is performed over different random data
        srand (time (nullptr));
    }

    // Generate a vector with n random different instances of backnodes of the
    // N-pancake with up to length discs each
    std::vector<khs::backnode_t<npancake_t>> randNBackNodes (const int n, const int length) {

        std::set<khs::backnode_t<npancake_t>> prev;
        std::vector<khs::backnode_t<npancake_t>> instances;

        // now, create as many random instances as requested of the given length
        for (auto i = 0 ; i < n ; i++){

            // create a backnode with a random instance of a pancake
            auto iperm = randInstance (length);

            // and ensure it is unique
            while (prev.find (iperm) != prev.end ()) {
                iperm = randInstance (length);
            }

            // and add it to the vector of instances to return
            khs::backnode_t ipancake {iperm};
            instances.push_back (ipancake);
        }

        return instances;
    }

    // Generate a vector with n random different instances of idnodes of the
    // N-pancake with up to length discs each
    std::vector<khs::idnode_t<npancake_t>> randNIdNodes (const int n, const int length) {

        std::set<khs::idnode_t<npancake_t>> prev;
        std::vector<khs::idnode_t<npancake_t>> instances;

        // now, create as many random instances as requested of the given length
        for (auto i = 0 ; i < n ; i++){

            // create an idnode with a random instance of a pancake
            auto iperm = randInstance (length);

            // and ensure it is unique
            while (prev.find (iperm) != prev.end ()) {
                iperm = randInstance (length);
            }

            // and add it to the vector of instances to return
            khs::idnode_t ipancake {iperm};
            instances.push_back (ipancake);
        }

        return instances;
    }

    // generate n random vectors with labeled backpointers, which are not
    // necessarily different
    std::vector<std::vector<khs::labeledbackpointer_t>> randLabeledBackPointers (const int n) {

        std::vector<std::vector<khs::labeledbackpointer_t>> result;

        // initialize the random generator
        std::random_device rd;
        std::mt19937 gen (rd ());
        std::uniform_int_distribution<> dist(1, MAX_VALUES);

        // create the n random vectors
        for (auto i = 0 ; i < n ; i++) {

            // now, create a vector with m random items each
            int m = dist (gen);
            std::vector<khs::labeledbackpointer_t> lbp;
            for (auto j = 0 ; j < m ; j++) {

                lbp.push_back (khs::labeledbackpointer_t {size_t (dist (gen)), dist (gen)});
            }

            // and add it to the result
            result.push_back (lbp);
        }

        // and return it
        return result;
    }

    // generate n random vectors with integeres, which are not necessarily different
    std::vector<std::vector<int>> randVectorIntegers (const int n) {

        std::vector<std::vector<int>> result;

        // initialize the random generator
        std::random_device rd;
        std::mt19937 gen (rd ());
        std::uniform_int_distribution<> dist(1, MAX_VALUES);

        // create the n random vectors
        for (auto i = 0 ; i < n ; i++) {
            result.push_back (randVectorInt (1 + dist (gen), MAX_VALUES));
        }

        return result;
    }

    // generate n random vectors with size_t, which are not necessarily different
    std::vector<std::vector<size_t>> randVectorLongs (const int n) {

        std::vector<std::vector<size_t>> result;

        // initialize the random generator
        std::random_device rd;
        std::mt19937 gen (rd ());
        std::uniform_int_distribution<> dist(1, MAX_VALUES);

        // create the n random vectors
        for (auto i = 0 ; i < n ; i++) {

            // now, create a vector with m random items each
            int m = dist (gen);
            std::vector<size_t> longs;
            for (auto j = 0 ; j < m ; j++) {
                longs.push_back (size_t (dist (gen)));
            }

            // and add it to the result
            result.push_back (longs);
        }

        return result;
    }

};

#endif // _TSTCLOSEDFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
