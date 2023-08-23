// -*- coding: utf-8 -*-
// helpers.cc
// -----------------------------------------------------------------------------
//
// Started on <dom 27-02-2022 19:43:18.800790041 (1645987398)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Common definitions for the implementation of different domains
//

#include "helpers.h"

using namespace std;

// process a single user selection for the values of k and issue an error in
// case they are incorrect. Otherwise, return a tuple of integers with the the
// first k, the second and the increment between successive values of k. If
// necessary, these slots are filled in with default values
const tuple<int, int , int> split_k (string& params) {

    int k0=0, k1=0, incr=0;

    // create a regexp to split the input string by blanks
    regex regex("\\s+");
    sregex_token_iterator it(params.begin(), params.end(), regex, -1);
    sregex_token_iterator end;

    // and now process each token separately
    for (auto idx=0 ; it != end; ++it, idx++) {

        try {
            auto value = stoi (*it);
            switch (idx) {
                case 0:
                    // copy the user selection and update the default values for the
                    // other parameters
                    k0 = value;
                    k1 = value;
                    incr = 1;
                    break;
                case 1:
                    k1 = value;
                    if (k1 < k0) {
                        throw invalid_argument{" The stopping value of k must be larger or equal than the initial value"};
                    }
                    break;
                case 2:
                    incr = value;
                    break;
            }
        } catch (const char* msg) {
            throw invalid_argument{" error while processing the values of k: " + string (msg)};
        }
    }

    // and return the user selection of solvers
    return tuple<int, int, int> {k0, k1, incr};
}

// process the entire user selection for the values of k and issue an error in
// case they are incorrect. Otherwise, return a vector of tuples of integers
// containing: first, the the initial value of k; next, the last value of k;
// finally, the increment between successive values of k. If necessary, these
// slots are filled in with default values
const vector<tuple<int, int , int>> split_ks (std::string& params){

    std::vector<std::tuple<int, int , int>> result;

    // split the given string into single specifications which are then
    // processed separately
    regex regex (";");
    sregex_token_iterator it(params.begin(), params.end(), regex, -1);
    sregex_token_iterator end;

    // and now process each single specification separately
    for (auto idx=0 ; it != end; ++it, idx++) {
        string spec = *it;
        result.push_back (split_k (ltrim (rtrim (spec))));
    }

    return result;
}

// process the user selection of solvers and issue an error if any is not
// recognized. Otherwise, return a vector of strings with the signatures of the
// solvers to run
const vector<string> split_solver (string& solver) {

    // initialization
    vector<string> solvers;

    // create a regexp to split the input string by blanks
    regex regex("\\s+");
    sregex_token_iterator it(solver.begin(), solver.end(), regex, -1);
    sregex_token_iterator end;

    // and now process each token separately
    for ( ; it != end; ++it) {

        // if this solver is not among those honoured by this program
        if (find (ack_solvers.begin (), ack_solvers.end (), *it)==ack_solvers.end ()) {

            // then raise an exception. I know, that's rude!
            throw invalid_argument ("Unrecognized solver: " + string(*it));
        } else {

            // otherwise, add it to the selection of solvers to run
            solvers.push_back (*it);
        }
    }

    // and return the user selection of solvers
    return solvers;
}

// transform the input string to lower case and return it
const string tolower (string& input) {

    for_each(input.begin(), input.end(), [](char & c){
        c = ::tolower(c);
    });
    return input;
}

// Given a list of choices, update the first parameter to the one matching one
// in choices, and return true. If there is no match, return false
//
// A match happens when the choice and any of the choices are exactly the same.
// Two characters are the same even if they are shown in different case.
bool get_choice (string& choice, const vector<string>& choices) {

    // for all choices
    for (auto ichoice : choices) {

        // if these two strings are the same in spite of the case, then return
        // the choice matching the variant
        if (tolower (choice) == tolower (ichoice)) {
            choice = ichoice;
            return true;
        }
    }

    // at this point, no match was found
    return false;
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
