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

// Return the binomial coefficient of n choose k
size_t binomial_coefficient (int n, int k) {

    // create a bidimensional matrix nxn
    vector<vector<size_t>> bc (n+1, vector<size_t> (n+1));

    // now, fill in the base cases
    for (auto i = 0 ; i < n+1 ; bc[i++][0] = 1);
    for (auto i = 0 ; i < n+1 ; bc[i][i] = 1, i++);

    // fill in the table
    for (auto i = 2 ; i < n+1 ; i++) {
        for (auto j = 1 ; j < i ; j++) {
            bc[i][j] = bc[i-1][j-1] + bc[i-1][j];
        }
    }

    // and return the binomial coefficient
    return bc[n][k];
}

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

// return a vector with n backpointers, each one randomly created with two
// values in the range [0, MAX_VALUE)
vector<khs::backpointer_t> randVectorBackpointer (int n) {

    // create a vector of backpointers
    vector<khs::backpointer_t> backpointers;

    // create a random number generator
    random_device rd;
    mt19937 gen (rd ());
    uniform_int_distribution<> dis (0, MAX_VALUE);

    // create n backpointers
    for (auto i = 0 ; i < n ; i++) {
        auto bp = khs::backpointer_t {size_t (dis (gen)), size_t (dis (gen))};
        backpointers.push_back (bp);
    }

    return backpointers;
}

// return a vector with n labeledbackpointers, each one randomly created with
// two values in the range [0, MAX_VALUE)
vector<khs::labeledbackpointer_t> randVectorLabeledBackpointer (int n) {

    // create a vector of labeledbackpointers
    vector<khs::labeledbackpointer_t> backpointers;

    // create a random number generator
    random_device rd;
    mt19937 gen (rd ());
    uniform_int_distribution<> dis (0, MAX_VALUE);

    // create n backpointers
    for (auto i = 0 ; i < n ; i++) {
        auto bp = khs::labeledbackpointer_t {size_t (dis (gen)), int (dis (gen))};
        backpointers.push_back (bp);
    }

    return backpointers;
}

// create a random instance of the N-Pancake with the given length
const npancake_t randInstance (int length) {

    // first, create the identity permutation of the given length
    vector<int> goal;
    for (auto i = 0 ; i < length ; goal.push_back (i++));

    // and just shuffle the goal
    random_device rd;
    mt19937 generator (rd());
    shuffle (goal.begin (), goal.end (), generator);

    // and return the random instance
    npancake_t result {goal};
    return result;
}

// Generate a correct path from the given start state with the specified number
// of transitions. It also returns the cost of the solution found under the cost
// model used in the initialization of the init table outside this function
const pair<vector<npancake_t>, int> randPath (const npancake_t& start, const int length) {

    int cost = 0;
    vector<npancake_t> path;

    // add the start state to the path
    path.push_back (start);

    // starting from the given start state create a descendant and add it to
    // the path
    while (path.size () < length) {

        // compute all children of the last state in the path
        vector<tuple<int, int, npancake_t>> successors;
        path.back ().children (0, start, successors);

        // and randomly select one
        int idx = rand () % successors.size ();

        // and add it to the path and update the cost
        path.push_back (get<2> (successors[idx]));
        cost += get<0> (successors[idx]);
    }

    // and finally return a pair with the path and the cost
    return make_pair (path, cost);
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
