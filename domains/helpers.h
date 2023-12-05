// -*- coding: utf-8 -*-
// helpers.h
// -----------------------------------------------------------------------------
//
// Started on <dom 27-02-2022 19:38:13.192951816 (1645987093)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>   Ian Herman <iankherman@gmail.com>   Ian Herman <iankherman@gmail.com>

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
static std::vector<std::string> ack_solvers {"mDijkstra", "belA0", "K0",
    "mA*", "belA*", "K*"};

// signature of brute-force search variants
static std::vector<std::string> brute_force_solvers {"mDijkstra", "belA0", "K0"};

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

// Return the output of the command "git describe"
const std::string git_describe ();

// Return information on all cpus available in the system
const std::string get_cpu_info();

// Return information on the available memory in the system
const std::string get_mem_info();

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

// open the specified filename and retrieve the name of every instance given in
// each line and a vector of strings with the contents of the same line
// following immediately after
void get_problems (const std::string& filename,
                   std::vector<std::string>& names,
                   std::vector<std::vector<std::string>>& instances);

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
