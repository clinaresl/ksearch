// -*- coding: utf-8 -*-
// npancake.cc
// -----------------------------------------------------------------------------
//
// Started on <lun 29-11-2021 18:48:45.490311369 (1638208125)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Implementation of a k-shortest path solver for the n-pancake
//

#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include <getopt.h>

#include "../solver.h"
#include "../../src/ksearch.h"

#include "npancake_t.h"

#define EXIT_OK 0
#define EXIT_FAILURE 1

using namespace std;

extern "C" {
    char *xstrdup (char *p);
}

/* Globals */
char *program_name;                       // The name the program was run with,

static struct option const long_options[] =
{
    {"solver", required_argument, 0, 's'},
    {"file", required_argument, 0, 'f'},
    {"variant", required_argument, 0, 'r'},
    {"k", required_argument, 0, 'k'},
    {"csv", required_argument, 0, 'C'},
    {"no-doctor", no_argument, 0, 'D'},
    {"summary", no_argument, 0, 'S'},
    {"verbose", no_argument, 0, 'v'},
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'V'},
    {NULL, 0, NULL, 0}
};

const string get_domain ();
const string get_variant ();
static int decode_switches (int argc, char **argv,
                            string& solver_name, string& filename, string& variant,
                            string& k_params, string& csvname, bool& no_doctor, bool& want_summary,
                            bool& want_verbose);
static void usage (int status);

// main entry point
int main (int argc, char** argv) {

    string solver_name;                            // user selection of solvers
    string filename;                            // file with all cases to solve
    string variant;                                    // variant of the domain
    string k_params;                       // user selection of the values of k
    string csvname;                          // name of the output csv filename
    bool no_doctor;                    // whether the doctor is disabled or not
    bool want_summary;      // whether a summary of results is requested or not
    bool want_verbose;                  // whether verbose output was requested
    chrono::time_point<chrono::system_clock> tstart, tend;          // CPU time

    // variables
    program_name = argv[0];
    vector<string> variant_choices = {"unit", "heavy-cost"};

    // arg parse
    decode_switches (argc, argv, solver_name, filename, variant, k_params, csvname, no_doctor, want_summary, want_verbose);

    // process the solver names and get a vector with the signatures of all
    // solvers to execute
    vector<string> solvers = split_solver (solver_name);

    // and also process the user selection of the K values
    auto kspec = split_ks (k_params);

    // parameter checking

    // --file
    if (filename == "") {
        cerr << "\n Please, provide a file with the information of all start states to solve" << endl;
        cerr << " wrt the identity permutation" << endl;
        cerr << " See " << program_name << " --help for more details" << endl << endl;
        exit(EXIT_FAILURE);
    }

    if (!get_choice (variant, variant_choices)) {
        cerr << "\n Please, provide a correct name for the variant with --variant" << endl;
        cerr << " See " << program_name << " --help for more details" << endl << endl;
        exit(EXIT_FAILURE);
    }

    /* do the work */

    /* !------------------------- INITIALIZATION --------------------------! */

    // open the given file and retrieve all cases from it
    vector<string> names;
    vector<vector<string>> instances;
    get_problems (filename, names, instances);
    if (!instances.size ()) {
        cerr << endl;
        cerr << " Error: The file '" << filename << "' contains no instances to solve!" << endl;
        cerr << endl;
        exit (EXIT_FAILURE);
    }

    // create the goal state which is represented as the identity permutation.
    // All instances are assumed to have the same length, so use the length of
    // any as the length of the N-Pancake to solve next
    vector<int> permutation;
    for (auto i = 0 ; i < instances[0].size () ; permutation.push_back (i++));
    npancake_t goal (permutation);

    // and now create a vector of tasks to solve
    vector<instance_t<npancake_t>> tasks;
    for (auto i = 0 ; i < instances.size () ; i++) {
        vector<int> permutation;
        for (auto& disc: instances[i]) {
            permutation.push_back (stoi (disc));
        }
        tasks.push_back (instance_t<npancake_t> {names[i], permutation, goal});
    }

    // initialize the list of operators and also the incremental table with the
    // updates of the manhattan distance
    npancake_t::init (variant);

    /* !-------------------------------------------------------------------! */

    cout << endl;
    cout << " solver       : " << solver_name << endl;
    cout << " file         : " << filename << " (" << instances.size () << " instances)" << endl;
    cout << " variant      : " << variant << endl;
    cout << " size         : " << npancake_t::get_n () << endl;
    cout << " K            : ";
    for (auto& ispec: kspec) {
        cout << "[" << get<0>(ispec) << ", " << get<1>(ispec) << ", " << get<2> (ispec) << "] ";
    }
    cout << endl;

    /* !----------------------------- SEARCH ------------------------------! */

    // start the clock
    tstart = chrono::system_clock::now ();

    // create an instance of the "generic" domain-dependent solver
    solver<npancake_t> manager (get_domain (), variant,
                                tasks, k_params);

    // solve all the instances with each solver selected by the user and in the
    // same order given
    for (auto isolver : solvers) {
        manager.run (isolver, no_doctor, want_summary, want_verbose);
    }

    // and stop the clock
    tend = chrono::system_clock::now ();

    // to conclude, show an error summary and store all the results in a csv
    // file in case any was given
    manager.show_error_summary (no_doctor);
    manager.write_csv (csvname);
    cout << " 🕒 CPU time: " << 1e-9*chrono::duration_cast<chrono::nanoseconds>(tend - tstart).count() << " seconds" << endl;
    cout << endl;

    /* !-------------------------------------------------------------------! */

    // Well done! Keep up the good job!
    return (EXIT_OK);
}

// return the domain of this solver
const string get_domain () {
    return "n-pancake";
}

// return the variant of the domain of this solver
const string get_variant () {
    return "unit";
}

// Set all the option flags according to the switches specified. Return the
// index of the first non-option argument
static int
decode_switches (int argc, char **argv,
                 string& solver_name, string& filename, string& variant,
                 string& k_params, string& csvname, bool& no_doctor, bool& want_summary,
                 bool& want_verbose) {

    int c;

    // Default values
    solver_name = "";
    filename = "";
    variant = "unit";
    k_params = "";
    csvname = "";
    no_doctor = false;
    want_summary = false;
    want_verbose = false;

    while ((c = getopt_long (argc, argv,
                             "s"  /* solver */
                             "f"  /* file */
                             "r"  /* variant */
                             "k"  /* k */
                             "C"  /* csv */
                             "D"  /* no-doctor */
                             "S"  /* summary */
                             "v"  /* verbose */
                             "h"  /* help */
                             "V", /* version */
                             long_options, (int *) 0)) != EOF) {
        switch (c) {
        case 's':  /* --solver */
            solver_name = optarg;
            break;
        case 'f':  /* --file */
            filename = optarg;
            break;
        case 'r': /* --variant */
            variant = optarg;
            break;
        case 'k':  /* --k */
            k_params = optarg;
            break;
        case 'C':  /* --csv */
            csvname = optarg;
            break;
        case 'D':  /* --no-doctor */
            no_doctor = true;
            break;
        case 'S':  /* --summary */
            want_summary = true;
            break;
        case 'v':  /* --verbose */
            want_verbose = true;
            break;
        case 'V':
            cout << " khs (n-pancake) " << CMAKE_VERSION << endl;
            cout << " " << CMAKE_BUILD_TYPE << " Build Type" << endl;
            exit (EXIT_OK);
        case 'h':
            usage (EXIT_OK);
        default:
            cout << endl << " Unknown argument!" << endl;
            usage (EXIT_FAILURE);
        }
    }
    return optind;
}


static void
usage (int status)
{
    cout << endl << " " << program_name << " implements various K shortest-path search algorithms in the N-Pancake using the GAP heuristic" << endl << endl;
    cout << " Usage: " << program_name << " [OPTIONS]" << endl << endl;
    cout << "\
 Mandatory arguments:\n\
      -s, --solver [STRING]+     K shortest-path algorithms to use. Choices are:\n\
                                    + Brute-force search algorithms:\n\
                                       > 'mDijkstra': brute-force variant of mA*\n\
                                       > 'K0': brute-force variant of K*\n\
                                       > 'belA0': brute-force variant of belA*\n\
                                    + Heuristic search algorithms:\n\
                                       > 'mA*': mA*\n\
                                       > 'K*': K*\n\
                                       > 'belA*': BELA*\n\
                                 It is possible to provide as many as desired in a blank separated list between double quotes,\n\
                                 e.g., \"mDijkstra belA0\"\n\
      -f, --file [STRING]        filename with a line for each instance to solve wrt to the identity permutation.\n\
                                 Each line consists of a list of numbers separated by spaces in the range [0, N)\n\
      -r, --variant [STRING]     Variant of the n-Pancake to consider. Choices are {unit, heavy-cost}. By default, unit\n\
                                 is used\n\
      -k, --k [NUMBER]+          Definition of the different values of K to test.\n\
                                 The entire specification consists of a semicolon separated list of single specifications\n\
                                 e.g., '1 5; 10 90 10; 100'\n\
                                 Every single specification consists of a blank separated list with up to three integers indicating\n\
                                 the first value of K, the last one and the increment between successive values of K.\n\
                                 If only one value is given (e.g., '100'), only one value of K is used; if only two are given\n\
                                 (e.g., '1 5'), all values of K between them are used with an increment equal to 1\n\
\n\
 Optional arguments:\n\
      -C, --csv [STRING]         name of the csv output files for storing results. If none is given, no file is generated\n\
      -D, --no-doctor            If given, the automated error checking is disabled. Otherwise, all solutions are automatically\n\
                                 checked for correctness\n\
      -S, --summary              If given, only the results of the last solution path found for every instance are shown. Otherwise,\n\
                                 the results of every single solution path are shown in the output csv file. It has no effect if\n\
                                 --csv is not given\n\
 Misc arguments:\n\
      --verbose                  print more information\n\
      -h, --help                 display this help and exit\n\
      -V, --version              output version information and exit\n\
\n";
    exit (status);
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
