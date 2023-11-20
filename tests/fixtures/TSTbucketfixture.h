// -*- coding: utf-8 -*-
// TSTbucketfixture.h
// -----------------------------------------------------------------------------
//
// Started on <lun 07-08-2023 18:34:06.859232505 (1691426046)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

//
// Fixture for testing the bucket class
//

#ifndef _TSTBUCKETFIXTURE_H_
#define _TSTBUCKETFIXTURE_H_

#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "../TSTdefs.h"
#include "../TSThelpers.h"
#include "../../src/structs/KHSbucket_t.h"

// Class definition
//
// Defines a Google test fixture for testing buckets
class BucketFixture : public ::testing::Test {

    protected:

        void SetUp () override {

            // just initialize the random seed to make sure that every iteration
            // is performed over different random data
            srand (time (nullptr));
        }

        // create a vector of strings and populate a bucket with all of them in
        // increasing order of their size. In doing so, it checks that all
        // operations are performed correctly
        std::vector<std::string> populate (khs::bucket_t<std::string>& bucket) {

            // randomly determine the number of buckets which is necessarily larger
            // than 0. Note the maximum length is roughly 1,000, the reason being
            // that creating buckets which are larger is a very costly operation
            auto length = 1 + random () % MAX_NB_BUCKETS/1'000;

            // make sure that the maximum length of the strings to generate next
            // fits into the number of buckets. If not, insert would resize the
            // underlying vector if necessary but this introduces a severe penalty
            // in performace
            while (length < MAX_NB_CHARS) {
                length = 1 + random () % MAX_NB_BUCKETS/1'000;
            }

            // create as many strings as the length randomly determined
            std::vector<std::string> values (length);

            // and create all these values
            for (auto j = 0 ; j < length ; j++) {
                values[j] = randString (MIN_NB_CHARS, MAX_NB_CHARS);
            }

            // insert all strings in the bucket sorting them in increasing order
            // of their length. A good practice might be to set the number of
            // buckets before inserting data but this is unnecessary. Not doing
            // it can only lead to a performance penalty but as for the sake of
            // completeness in the tests, it is best not to do and to see
            // everything working out as expected
            for (auto v : values) {
                bucket.insert (v, v.size ());
            }

            // and return the vector of values inserted in the bucket
            return values;
        }
};

#endif // _TSTBUCKETFIXTURE_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
