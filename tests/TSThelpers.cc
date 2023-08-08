// -*- coding: utf-8 -*-
// TSThelpers.cc
// -----------------------------------------------------------------------------
//
// Started on <lun 07-08-2023 16:09:23.879483705 (1691417363)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Helper functions for all tests
//

#include "TSThelpers.h"

using namespace std;

// Generate a random string with length characters in the sequence ASCII(32) -
// ASCII(126) which do not appear in the string exclude
string randString (int length, string exclude) {

  // just randomly sample a character from the set until the string gets the
  // requested length
  char randchr;
  string randstr = "";
  for (int i = 0 ; i < length ; i++) {
    do {
      randchr = (char) (rand () % (126 - 32 + 1) + 32);
    } while (exclude.find (randchr) != string::npos);
    randstr += randchr;
  }

  return randstr;
}

// Generate a random string with a number of characters randomly selected in the
// range [l0, l1) in the sequence ASCII(32) - ASCII(126) which do not appear in
// the string exclude
string randString (int l0, int l1, string exclude)
{
    // randomly determine the length of the string
    int length = l0 + random () % (l1 - l0);

    // and return a string randomly generated with this length
    return randString (length, exclude);
}

// return a vector with n numbers randomly generated in the interval [0, m). If
// remove_duplicates takes the value true, then no values are duplicated
vector<int> randVectorInt (int n, int m, bool remove_duplicates) {

    // create an empty vector
    set<int> lookup;
    vector<int> result;

    // generate n random numbers
    for (auto i = 0 ; i < n ; i++) {

        // generate a new random number avoiding duplicates if requested
        int item = rand () % m;
        while (remove_duplicates &&
               lookup.find (item) != lookup.end ()) {
            item = rand () % m;
        }

        // and insert it into the vector, and remember it for future insertions
        result.push_back (item);
        lookup.insert (item);
    }

    // and return the vector
    return result;
}

// return a vector with n strings randomly generated each with m chars. If
// remove_duplicates takes the value true, then no values are duplicated
vector<string> randVectorString (int n, int m, bool remove_duplicates) {

    // create an empty vector
    set<string> lookup;
    vector<string> result;

    // generate n random strings
    for (auto i = 0 ; i < n ; i++) {

        // generate a new random string avoiding duplicates if requested
        string item = randString (m);
        while (remove_duplicates &&
               lookup.find (item) != lookup.end ()) {
            item = randString (n);
        }

        // and insert it into the vector
        result.push_back (item);
    }

    // and return the vector
    return result;
}

// create a random instance of the N-Pancake with the given length
const npancake_t randInstance (int n) {

    // first, create the identity permutation of the given length
    std::vector<int> goal;
    for (auto i = 0 ; i < n ; goal.push_back (i++));

    // and just shuffle the goal
    std::random_device rd;
    std::mt19937 generator (rd());
    std::shuffle (goal.begin (), goal.end (), generator);

    // and return the random instance
    npancake_t result {goal};
    return result;
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
