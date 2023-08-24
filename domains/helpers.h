// -*- coding: utf-8 -*-
// helpers.h
// -----------------------------------------------------------------------------
//
// Started on <dom 27-02-2022 19:38:13.192951816 (1645987093)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Common definitions for the implementation of different domains
//

#ifndef _HELPERS_H_
#define _HELPERS_H_

#include<algorithm>
#include<ctype.h>
#include<fstream>
#include<iostream>
#include<regex>
#include<stdexcept>
#include<string>
#include<tuple>
#include<utility>
#include<vector>

#include "../src/ksearch.h"

// signatures of acknowledged solvers
static std::vector<std::string> ack_solvers {"mDijkstra", "belA0"};

// Left trimming a string
static inline std::string& ltrim (std::string& s) {
    s.erase (s.begin(), std::find_if (s.begin(), s.end(), [](int ch) {
                return !std::isspace(ch);
            }));
    return s;
}

// Right trimming a string
static inline std::string& rtrim (std::string& s) {
    s.erase (std::find_if (s.rbegin(), s.rend(), [](int ch) {
                return !std::isspace(ch);
            }).base(), s.end());
    return s;
}

// process a single user selection for the values of k and issue an error in
// case they are incorrect. Otherwise, return a tuple of integers with the the
// first k, the second and the increment between successive values of k. If
// necessary, these slots are filled in with default values
const std::tuple<int, int , int> split_k (std::string& params);

// process the entire user selection for the values of k and issue an error in
// case they are incorrect. Otherwise, return a vector of tuples of integers
// containing: first, the the initial value of k; next, the last value of k;
// finally, the increment between successive values of k. If necessary, these
// slots are filled in with default values
const std::vector<std::tuple<int, int , int>> split_ks (std::string& params);

// process the user selection of solvers and issue an error if any is not
// recognized. Otherwise, return a vector of strings with the signatures of the
// solvers to run
const std::vector<std::string> split_solver (std::string& solver);

// transform the input string to lower case and return it
const std::string tolower (std::string& input);

// open the specified filename and retrieve the names and the instances given in
// each line:
//
// 1. This function assumes T has a explicit constructor that receives a vector
//    of integers
//
// 2. No error checking is performed. The following function assumes that each
//    line consists first of an index problem and then a sequence of integers.
//    If required, these sequences might satisfy the specific conditions of each
//    domain, i.e., distinct integers in a specific range, etc.
template<typename T>
void get_problems (const std::string& filename,
                   std::vector<std::string>& names, std::vector<T>& instances) {

    std::ifstream stream (filename);

    // read the instances line by line. Note that parsing is necessary because
    // the length of the instances is unknown
    std::string line;
    while (std::getline(stream, line)) {

        // create a regexp to split this line into integers
        std::regex regex ("\\s+");
        std::sregex_token_iterator it(line.begin(), line.end(), regex, -1);
        std::sregex_token_iterator end;

        // and now process each disc separately. Note the first one is the
        // instance index which has to be stored as its name
        int idx = 0;
        std::vector<int> permutation;
        for ( ; it != end ; ++it) {

            // if we are at the first position, then store it as its name
            if (!idx) {
                names.push_back (*it);
                idx++;
            } else {

                // otherwise, store this disc as part of the permutation
                permutation.push_back (stoi (*it));
            }
        }

        // and add this instance
        instances.push_back (T (permutation));
    }
}

// write the contents of the given container of solutions in the specified file
// in csv format, if and only if filename is a non-empty string. Otherwise, do
// nothing other than alerting the user that no csv file has been generated
template<typename T>
void write_csv (const std::string& filename, const khs::ksolutions_t<T>& bag) {

    // and write all results to a csv file only in case the user provided a name
    // for the output csv file
    if (filename.size ()) {

        // create the output file and truncate its contents in case any exists
        std::ofstream csvstream {filename, std::ios::out | std::ios::trunc};
        csvstream << bag;
        std::cout << " 🛈 CSV file generated!" << std::endl;
    } else {
        std::cout << " ⚠ No CSV output generated!" << std::endl;
    }
}

// return a (plain) pointer to a specific solver for solving instances in the
// specified domain D according to the given name. The solver is initialized
// with the following data:
//
// * start: instance to solve
// * goal: goal
// * k: number of paths to find
template<typename D>
khs::bsolver<D>* get_solver (const std::string name,
                             const D& start, const D& goal,
                             const int k) {

    // create a pointer to a solver
    khs::bsolver<D>* m;

    // and now choose according to the given name
    if (name == "mDijkstra") {
        m = new khs::mA<D> (k, start, goal);
    } else if (name == "belA0") {
        m = new khs::bela<D> (k, start, goal);
    } else {
        throw std::invalid_argument{"Unknown solver!"};
    }

    // and return a pointer to the selected solver
    return m;
}

// Given a list of choices, update the first parameter to the one matching one
// in choices, and return true. If there is no match, return false
//
// A match happens when the choice and any of the choices are exactly the same.
// Two characters are the same even if they are shown in different case.
bool get_choice (std::string& choice, const std::vector<std::string>& choices);


#endif // _HELPERS_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
