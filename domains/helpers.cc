// -*- coding: utf-8 -*-
// helpers.cc
// -----------------------------------------------------------------------------
//
// Started on <dom 27-02-2022 19:43:18.800790041 (1645987398)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>   Ian Herman <iankherman@gmail.com>

//
// Common definitions for the implementation of different domains
//

#include "helpers.h"

// Return information on all cpus available in the system
const std::string get_cpu_info() {

    std::string cmd = "lscpu";
    std::string result = "";
    FILE* pipe = popen (cmd.c_str(), "r");
    if (!pipe) throw std::runtime_error ("popen() failed!");
    try {
        while (feof (pipe) == 0) {
            char buffer[128];
            if (fgets (buffer, 128, pipe) != NULL)
                result += buffer;
        }
    } catch (...) {
        pclose (pipe);
        throw;
    }
    pclose (pipe);

    return result;
}

// Return information on the available memory in the system
const std::string get_mem_info() {

    std::string cmd = "free -h";
    std::string result = "";
    FILE* pipe = popen (cmd.c_str(), "r");
    if (!pipe) throw std::runtime_error ("popen() failed!");
    try {
        while (feof (pipe) == 0) {
            char buffer[128];
            if (fgets (buffer, 128, pipe) != NULL)
                result += buffer;
        }
    } catch (...) {
        pclose (pipe);
        throw;
    }
    pclose (pipe);

    return result;
}

// given the name of a solver, it returns true if it is a brute-force variant,
// and false otherwise. The name of the brute-force search algorithms are stored
// in a separate set
bool brute_force (const std::string& solver_name) {

    std::regex solver_regex(solver_regex_spec);
    std::smatch matches;
    std::regex_search(solver_name, matches, solver_regex);
    std::string nameProcessed = matches[1];

    // and now use the solver name to determine whether this is a brute force
    // search algorithm or not
    return (nameProcessed == "mDijkstra" or
            nameProcessed == "BELA0" or
            nameProcessed == "K0" or
            nameProcessed == "bBELA0" or
            nameProcessed == "sBELA0");
}

// return a vector with the same length than the first vector whose values are
// taken from the second vector unless an empty item is found or the second
// vector is exhausted in which case items from the first vector are taken
const std::vector<std::string> union_string (const std::vector<std::string> defs, std::vector<std::string> vals) {

    std::vector<std::string> result;

    // Note that vectors are not *assumed* to have the same length. In spite of
    // this, it is necessary to process all items in the first vector
    for (size_t i = 0 ; i < defs.size () ; ++i) {

        if (i >= vals.size () or vals[i] == "") {
            result.push_back (defs[i]);
        } else {
            result.push_back (vals[i]);
        }
    }

    // and return the elements computed so far
    return result;
}

// process a single user selection for the values of k and issue an error in
// case they are incorrect. Otherwise, return a tuple of integers with the the
// first k, the second and the increment between successive values of k. If
// necessary, these slots are filled in with default values
const std::tuple<int, int , int> split_k (std::string& params) {

    int k0=0, k1=0, incr=0;

    // create a regexp to split the input string by blanks
    std::regex regex("\\s+");
    std::sregex_token_iterator it(params.begin(), params.end(), regex, -1);
    std::sregex_token_iterator end;

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
                        throw std::invalid_argument{" The stopping value of k must be larger or equal than the initial value"};
                    }
                    break;
                case 2:
                    incr = value;
                    break;
            }
        } catch (const char* msg) {
            throw std::invalid_argument{" error while processing the values of k: " + std::string (msg)};
        }
    }

    // and return the user selection of solvers
    return std::tuple<int, int, int> {k0, k1, incr};
}

// process the entire user selection for the values of k and issue an error in
// case they are incorrect. Otherwise, return a vector of tuples of integers
// containing: first, the the initial value of k; next, the last value of k;
// finally, the increment between successive values of k. If necessary, these
// slots are filled in with default values
const std::vector<std::tuple<int, int , int>> split_ks (std::string& params){

    std::vector<std::tuple<int, int , int>> result;

    // split the given string into single specifications which are then
    // processed separately
    std::regex regex (";");
    std::sregex_token_iterator it(params.begin(), params.end(), regex, -1);
    std::sregex_token_iterator end;

    // and now process each single specification separately
    for (auto idx=0 ; it != end; ++it, idx++) {
        std::string spec = *it;
        result.push_back (split_k (ltrim (rtrim (spec))));
    }

    return result;
}

// process a user selection provided as a blank separated list of strings and
// returns a vector with each component. The argument options provide the valid
// options, so that if any is found not to be in options an exception is raised
const std::vector<std::string> split_option (std::string& value, const std::vector<std::string> options) {

    // initialization
    std::vector<std::string> values;

    // create a regexp to split the input string by blanks
    std::regex regex("\\s+");
    std::sregex_token_iterator it (value.begin(), value.end(), regex, -1);
    std::sregex_token_iterator end;

    // and now process each token separately
    for ( ; it != end; ++it) {

        // if this value is not among those honoured by this program
        if (find (options.begin (), options.end (), *it) == options.end ()) {

            // then raise an exception. I know, that's rude!
            throw std::invalid_argument ("[split_option (vector<string>)] Unrecognized option: " + std::string(*it));
        } else {

            // otherwise, add it to the selection of solvers to run
            values.push_back (*it);
        }
    }

    // and return the user selection of solvers
    return values;
}

// process a user selection provided as a blank separated list of strings and
// returns a vector with each component. The argument options provide the valid
// options, so that if any is found not to be in options an exception is raised
const std::vector<std::string> split_option (std::string& value, const std::vector<std::regex> options) {

    // initialization
    std::vector<std::string> values;

    // create a regexp to split the input string by blanks
    std::regex regex("\\s+");
    std::sregex_token_iterator it (value.begin(), value.end(), regex, -1);
    std::sregex_token_iterator end;

    // and now process each token separately
    for ( ; it != end; ++it) {
        bool matchFound = false;
        for (auto & r : options)
            if (std::regex_match(it->first, it->second, r)) {
                matchFound = true;
                break;
            }

        // if this value is not among those honoured by this program
        if (!matchFound) {

            // then raise an exception. I know, that's rude!
            throw std::invalid_argument ("[split_option (vector<regex>)] Unrecognized option: " + std::string(*it));
        } else {

            // otherwise, add it to the selection of solvers to run
            values.push_back (*it);
        }
    }

    // and return the user selection of solvers
    return values;
}

// transform the input string to lower case and return it
const std::string tolower (std::string& input) {

    for_each(input.begin(), input.end(), [](char & c){
        c = ::tolower(c);
    });
    return input;
}

// open the specified filename and retrieve the name of every instance given in
// each line and a vector of strings with the contents of the same line
// following immediately after
void get_problems (const std::string& filename,
                   std::vector<std::string>& names,
                   std::vector<std::vector<std::string>>& instances)
{

    std::ifstream stream (filename);

    // read the instances line by line. Note that parsing is necessary because
    // the length of the instances is unknown
    std::string line;
    while (getline(stream, line)) {

        // create a regexp to split this line blank separated tokens
        std::regex regex ("\\s+");
        std::sregex_token_iterator it(line.begin(), line.end(), regex, -1);
        std::sregex_token_iterator end;

        // and now process each line separately getting all tokens as strings
        // and storing them separately
        int idx = 0;
        std::vector<std::string> contents;
        for ( ; it != end ; ++it) {

            // if we are at the first position, then store it as its name
            if (!idx) {
                names.push_back (*it);
                idx++;
            } else {

                // otherwise, store this token as part of the specification of
                // the task to solve
                contents.push_back (*it);
            }
        }

        // and add this instance
        instances.push_back (contents);
    }
}

// Given a list of choices, update the first parameter to the one matching one
// in choices, and return true. If there is no match, return false
//
// A match happens when the choice and any of the choices are exactly the same.
// Two characters are the same even if they are shown in different case.
bool get_choice (std::string& choice, const std::vector<std::string>& choices) {

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
