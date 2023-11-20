// -*- coding: utf-8 -*-
// TSTbucket_t.cc
// -----------------------------------------------------------------------------
//
// Started on <lun 07-08-2023 18:40:16.050953386 (1691426416)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

//
// Unit tests for the bucket_t class
//

#include "../fixtures/TSTbucketfixture.h"

using namespace std;

// Checks the creation of empty buckets is initialized correctly
// ----------------------------------------------------------------------------
TEST_F (BucketFixture, EmptyStringBucket) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a bucket to store strings
        khs::bucket_t<string> bucket;

        // now, verify the bucket is properly initialized
        ASSERT_EQ (bucket.get_mini (), 1);
        ASSERT_EQ (bucket.get_maxi (), 1);
        ASSERT_EQ (bucket.get_nbbuckets (), 1);
        ASSERT_EQ (bucket.size (), 0);
    }
}

// Checks the number of buckets grows as the indices become larger
// ----------------------------------------------------------------------------
TEST_F (BucketFixture, SizeStringBucket) {

    // resizing a bucket is a very costly operation and thus, this is done only
    // once

    // create a bucket to store strings
    khs::bucket_t<string> bucket;

    // now insert items at the positions which follow the serie of powers of
    // 2 until a very large number
    int idx = 1;
    while (idx <= MAX_NB_BUCKETS ) {

        // insert a string with idx chars
        bucket.insert (randString (idx), idx);

        // in this test, items are inserted in increasing order, thus, verify
        // that the maximum index progresses accordingly, whereas the minimum
        // index is always the same
        ASSERT_EQ (bucket.get_mini (), 1);
        ASSERT_EQ (bucket.get_maxi (), idx);

        // verify now that the number of buckets has doubled
        ASSERT_GE (bucket.get_nbbuckets (), 1+idx);
        idx *= 2;
    }
}

// Checks that buckets can be effectively resized
// ----------------------------------------------------------------------------
TEST_F (BucketFixture, ResizeStringBucket) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a bucket to store strings
        khs::bucket_t<string> bucket;

        // randomly determine the number of buckets which is necessarily larger
        // than 0. Note the maximum length is roughly 1,000, the reason being
        // that creating buckets which are larger is a very costly operation
        auto length = 1 + random () % MAX_NB_BUCKETS/1'000;

        // and compute the minimum power of two which is greater or equal than
        // the random number of buckets
        auto pow2 = 1;
        while (pow2 < length) {
            pow2 *= 2;
        };

        // resize the bucket and verify the value returned is correct
        ASSERT_EQ (bucket.set_nbbuckets (length), pow2);

        // also verify the operation was correctly performed
        ASSERT_EQ (bucket.get_nbbuckets (), pow2);

        // in spite of the bucket being resized, no insertions were done, and
        // thus ...
        ASSERT_EQ (bucket.get_mini (), 1);
        ASSERT_EQ (bucket.get_maxi (), 1);
        ASSERT_EQ (bucket.size (), 0);
    }
}

// Checks that strings are properly sorted in a strings bucket in increasing
// order of their length
// ----------------------------------------------------------------------------
TEST_F (BucketFixture, InsertStringBucket) {

    // function used for computing the minimum and maximum index
    auto comp = [] (const string& s1, const string& s2) {
        return s1.size () < s2.size ();
    };

    for (auto i = 0 ; i < NB_TESTS/10 ; i++) {

        // create a bucket to store strings
        khs::bucket_t<string> bucket;

        // populate the bucket and get the values inserted
        vector<string> values = populate (bucket);

        // get the minimum and maximum length which should be proven to be equal
        // to the minimum and maximum indexes in the bucket
        auto mini = min_element (values.begin (), values.end (), comp);
        auto maxi = max_element (values.begin (), values.end (), comp);
        ASSERT_EQ (values[mini-values.begin ()].size (), bucket.get_mini ());
        ASSERT_EQ (values[maxi-values.begin ()].size (), bucket.get_maxi ());

        // extract all items from the bucket and verify their length is given in
        // increasing order
        int idx = 0;
        int current = 0;
        while (bucket.size ()) {

            // extract the first element from the bucket
            string item = bucket.pop_front ();
            ASSERT_GE (item.size (), current);

            // and verify the size has decreased accordingly
            idx++;
            ASSERT_EQ (bucket.size (), values.size () - idx);

            // and update the length of the last string retrieved from the
            // bucket
            current = item.size ();
        }
    }
}

// Checks that strings are properly removed from a bucket of strings
// ----------------------------------------------------------------------------
TEST_F (BucketFixture, RemoveStringBucket) {

    // function used for computing the minimum and maximum index
    auto comp = [] (const string& s1, const string& s2) {
        return s1.size () < s2.size ();
    };

    for (auto i = 0 ; i < NB_TESTS/10 ; i++) {

        // create a bucket to store strings
        khs::bucket_t<string> bucket;

        // populate the bucket and get the values inserted
        vector<string> values = populate (bucket);

        // create a histogram with the observations of the lengths of all values
        vector<int> histogram (MAX_NB_CHARS);
        for (auto v : values ) {
            histogram[v.size ()]++;
        }

        // verify the histogram corresponds with the data stored in the bucket
        for (auto j = 0 ; j < MAX_NB_CHARS ; j++) {
            ASSERT_EQ (bucket.size (j), histogram[j]);
        }

        // next, randomly remove some items from the bucket. For this, randomly
        // choose half the values generated and remove items with their length
        // from the bucket
        const auto length = values.size ();
        for (auto j = 0 ; j < length ; j++) {
            auto loc = random () % values.size ();
            auto idx = values[loc].size ();

            // remove an item from the bucket with a length equal to the loc-th
            // item in values, and verify that the extracted element has
            // precisely that size
            auto item = bucket.remove (idx);
            ASSERT_EQ (item.size (), idx);

            // verify the histogram corresponds with the data stored in the bucket
            histogram[idx]--;
            for (auto j = 0 ; j < MAX_NB_CHARS ; j++) {
                ASSERT_EQ (bucket.size (j), histogram[j]);
            }

            // make sure to remove this item from the vector of values to avoid
            // selecting it again
            values.erase (values.begin () + loc);

            // verify now that the total number of items has been properly
            // updated
            ASSERT_EQ (bucket.size (), values.size ());

            // get the minimum and maximum length which should be proven to be
            // equal to the minimum and maximum indexes in the bucket unless the
            // size of the bucket is zero in which case the bucket cannonically
            // make min and max being equal to 1 ---which is not necessary the
            // value computed by min and max element
            auto mini = min_element (values.begin (), values.end (), comp);
            auto maxi = max_element (values.begin (), values.end (), comp);
            ASSERT_TRUE (bucket.size () == 0 ||
                         (values[mini-values.begin ()].size () == bucket.get_mini ()));
            ASSERT_TRUE (bucket.size () == 0 ||
                         (values[maxi-values.begin ()].size () == bucket.get_maxi ()));
        }

        // before running into the next try, verify the total number of items is
        // zero. This is redundant, I know! Pure paranoia ...!!
        ASSERT_EQ (bucket.size (), 0);
    }
}

// Checks that mixed insertions and removals from a bucket of strings work as
// expected
// ----------------------------------------------------------------------------
TEST_F (BucketFixture, InsertRemoveStringBucket) {

    // function used for computing the minimum and maximum index
    auto comp = [] (const string& s1, const string& s2) {
        return s1.size () < s2.size ();
    };

    for (auto i = 0 ; i < NB_TESTS/10 ; i++) {

        // create a bucket to store strings
        khs::bucket_t<string> bucket;

        // populate the bucket and get the values inserted
        vector<string> values = populate (bucket);

        // create a histogram with the observations of the lengths of all values
        vector<int> histogram (MAX_NB_CHARS);
        for (auto v : values ) {
            histogram[v.size ()]++;
        }

        // verify the histogram corresponds with the data stored in the bucket
        for (auto j = 0 ; j < MAX_NB_CHARS ; j++) {
            ASSERT_EQ (bucket.size (j), histogram[j]);
        }

        // now, create a vector of strings with additional data to insert into
        // the bucket with only a few items
        vector<string> data;
        for (auto j = 0 ; j < MAX_VALUES/10 ; j++) {
            data.push_back (randString (MIN_NB_CHARS, MAX_NB_CHARS));
        }

        // and now, until the additional data is exhausted or the bucket is
        // exhausted
        while (data.size () && bucket.size ()) {

            // through a dice to determine whether to insert an additional item
            // or to remove an existing item from the bucket
            int op = rand () % 2;
            if (!op) {

                // removing from the bucket a random element
                auto loc = random () % values.size ();
                auto idx = values[loc].size ();

                // and remove it. Verify next that the item removed has the
                // expected size
                auto item = bucket.remove (idx);
                ASSERT_EQ (item.size (), idx);

                // make sure to remove this item from the vector of values to avoid
                // selecting it again
                values.erase (values.begin () + loc);

                // verify now that the total number of items has been properly
                // updated
                ASSERT_EQ (bucket.size (), values.size ());
            } else {

                // insert an item from the vector of additional data
                auto loc = random () % data.size ();
                bucket.insert (data[loc], data[loc].size ());

                // ensure that values contains the same data stored in the bucket
                values.push_back (data[loc]);

                // make sure to remove this item from the vector of additional
                // data to avoid selecting it again
                data.erase (data.begin () + loc);

                // verify now that the total number of items has been properly
                // updated. One is substracted because the additional item was
                // also moved to values
                ASSERT_EQ (bucket.size (), values.size ());
            }

            // get the minimum and maximum length which should be proven to be
            // equal to the minimum and maximum indexes in the bucket unless the
            // size of the bucket is zero in which case the bucket cannonically
            // make min and max being equal to 1 ---which is not necessary the
            // value computed by min and max element
            auto mini = min_element (values.begin (), values.end (), comp);
            auto maxi = max_element (values.begin (), values.end (), comp);
            ASSERT_TRUE (bucket.size () == 0 ||
                         (values[mini-values.begin ()].size () == bucket.get_mini ()));
            ASSERT_TRUE (bucket.size () == 0 ||
                         (values[maxi-values.begin ()].size () == bucket.get_maxi ()));
        }

        // extract all items from the bucket and verify their length is given in
        // increasing order
        int idx = 0;
        int current = 0;
        while (bucket.size ()) {

            // extract the first element from the bucket
            string item = bucket.pop_front ();
            ASSERT_GE (item.size (), current);

            // and verify the size has decreased accordingly
            idx++;
            ASSERT_EQ (bucket.size (), values.size () - idx);

            // and update the length of the last string retrieved from the
            // bucket
            current = item.size ();
        }
    }
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
