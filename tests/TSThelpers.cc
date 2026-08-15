// -*- coding: utf-8 -*-
// TSThelpers.cc
// -----------------------------------------------------------------------------
//
// Started on <lun 07-08-2023 16:09:23.879483705 (1691417363)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

//
// Helper functions for all tests
//

#include "TSThelpers.h"

// Return the binomial coefficient of n choose k
size_t binomial_coefficient (int n, int k) {

    // create a bidimensional matrix nxn
    std::vector<std::vector<size_t>> bc (n+1, std::vector<size_t> (n+1));

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
std::string randString (int length, std::string exclude) {

  // just randomly sample a character from the set until the string gets the
  // requested length
  char randchr;
  std::string randstr = "";
  for (int i = 0 ; i < length ; i++) {
    do {
      randchr = (char) (rand () % (126 - 32 + 1) + 32);
    } while (exclude.find (randchr) != std::string::npos);
    randstr += randchr;
  }

  return randstr;
}

// Generate a random string with a number of characters randomly selected in the
// range [l0, l1) in the sequence ASCII(32) - ASCII(126) which do not appear in
// the string exclude
std::string randString (int l0, int l1, std::string exclude)
{
    // randomly determine the length of the string
    int length = l0 + random () % (l1 - l0);

    // and return a string randomly generated with this length
    return randString (length, exclude);
}

// return a vector with a list of numbers from 1 up to n
std::vector<int> succListInt (int n) {

    // First create a vector of integers of the desired size
    std::vector<int> output = std::vector<int>(n, 0);

    // and next overwrite its contents using the succ function defined over
    // index_t
    generate (output.begin (), output.end (), index_t ());
    return output;
}

// return a vector with n numbers randomly generated in the interval [0, m). If
// remove_duplicates takes the value true, then no values are duplicated
std::vector<int> randVectorInt (int n, int m, bool remove_duplicates) {

    // create an empty vector
    std::set<int> lookup;
    std::vector<int> result;

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
std::vector<std::string> randVectorString (int n, int m, bool remove_duplicates) {

    // create an empty vector
    std::set<std::string> lookup;
    std::vector<std::string> result;

    // generate n random strings
    for (auto i = 0 ; i < n ; i++) {

        // generate a new random string avoiding duplicates if requested
        std::string item = randString (m);
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
std::vector<khs::backpointer_t> randVectorBackpointer (int n) {

    // create a vector of backpointers
    std::vector<khs::backpointer_t> backpointers;

    // create a random number generator
    std::random_device rd;
    std::mt19937 gen (rd ());
    std::uniform_int_distribution<> dis (0, MAX_VALUE);

    // create n backpointers
    for (auto i = 0 ; i < n ; i++) {
        auto bp = khs::backpointer_t {size_t (dis (gen)), size_t (dis (gen))};
        backpointers.push_back (bp);
    }

    return backpointers;
}

// return a vector with n labeledbackpointers, each one randomly created with
// two values in the range [0, MAX_VALUE)
std::vector<khs::labeledbackpointer_t> randVectorLabeledBackpointer (int n) {

    // create a vector of labeledbackpointers
    std::vector<khs::labeledbackpointer_t> backpointers;

    // create a random number generator
    std::random_device rd;
    std::mt19937 gen (rd ());
    std::uniform_int_distribution<> dis (0, MAX_VALUE);

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
    std::vector<int> goal = succListInt (length);

    // and just shuffle the goal
    std::random_device rd;
    std::mt19937 generator (rd());
    shuffle (goal.begin (), goal.end (), generator);

    // and return the random instance
    npancake_t result {goal};
    return result;
}

// Generate a correct path from the given start state with the specified number
// of transitions. It also returns the cost of the solution found under the cost
// model used in the initialization of the init table outside this function
const std::pair<std::vector<npancake_t>, int> randPath (const npancake_t& start, const int length) {

    int cost = 0;
    std::vector<npancake_t> path;

    // add the start state to the path
    path.push_back (start);

    // starting from the given start state create a descendant and add it to
    // the path
    while (path.size () < length) {

        // compute all children of the last state in the path. To do this
        // accumulate all children in a vector of tuples with the g-value of
        // each descendant and the descendant itself
        auto back = path.back ();
        std::vector<std::tuple<int, npancake_t>> successors;
        back.children (
            0,
            start,
            [&] (int g, int h, npancake_t&& successor) {

                // and add this node
                successors.push_back ({g, successor});
            });
        
        // and randomly select one
        int idx = rand () % successors.size ();

        // and add it to the path and update the cost
        path.push_back (get<npancake_t> (successors[idx]));
        cost += get<int> (successors[idx]);
    }

    // and finally return a pair with the path and the cost
    return std::make_pair (path, cost);
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
