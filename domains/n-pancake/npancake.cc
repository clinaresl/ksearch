// -*- coding: utf-8 -*-
// npancake.cc
// -----------------------------------------------------------------------------
//
// Started on <lun 29-11-2021 18:48:45.490311369 (1638208125)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Implementation of k-shortest path variants for the n-pancake
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

#include "../helpers.h"
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
    {"verbose", no_argument, 0, 'v'},
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'V'},
    {NULL, 0, NULL, 0}
};

const string get_domain ();
static int decode_switches (int argc, char **argv,
                            string& solver, string& filename, string& variant,
                            string& k_params, string& csvname,
                            bool& want_verbose);
static void usage (int status);

// main entry point
int main (int argc, char** argv) {

    string solver;                                 // user selection of solvers
    string filename;                            // file with all cases to solve
    string variant;                                    // variant of the domain
    string k_params;                       // user selection of the values of k
    string csvname;                          // name of the output csv filename
    bool want_verbose;                  // whether verbose output was requested
    chrono::time_point<chrono::system_clock> tstart, tend;          // CPU time

    // variables
    program_name = argv[0];
    vector<string> variant_choices = {"unit", "heavy-cost"};

    // arg parse
    decode_switches (argc, argv, solver, filename, variant, k_params, csvname, want_verbose);

    // process the solver names and get a vector with the signatures of all
    // solvers to execute
    vector<string> solvers = split_solver (solver);

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
    vector<npancake_t> instances;
    get_problems<npancake_t> (filename, names, instances);
    if (!instances.size ()) {
        cerr << endl;
        cerr << " Error: The file '" << filename << "' contains no instances to solve!" << endl;
        cerr << endl;
        exit (EXIT_FAILURE);
    }

    // create the goal state which is represented as the identity permutation
    vector<int> permutation;
    for (auto i = 0 ; i < npancake_t::get_n () ; permutation.push_back (i++));
    npancake_t goal (permutation);

    // initialize the list of operators and also the incremental table with the
    // updates of the manhattan distance
    npancake_t::init (variant);

    /* !-------------------------------------------------------------------! */

    cout << endl;
    cout << " solver       : " << solver << endl;
    cout << " file         : " << filename << " (" << instances.size () << " instances)" << endl;
    cout << " variant      : " << variant << endl;
    cout << " size         : " << npancake_t::get_n () << endl;
    cout << " K            : ";
    for (auto& ispec: kspec) {
        cout << "[" << get<0>(ispec) << ", " << get<1>(ispec) << ", " << get<2> (ispec) << "] ";
    }
    cout << endl;

    /* !----------------------------- SEARCH ------------------------------! */

    // initialize a container for storing all solutions and register relevant
    // information for all solvers
    khs::ksolutions_t<npancake_t> results;
    results.set_domain (get_domain ());
    results.set_variant (variant);

    // start the clock
    tstart = chrono::system_clock::now ();

    // solve all the instances with each solver selected by the user and in the
    // same order provided by the user. Solutions provided by different solvers
    // go to different containers of solutions
    for (auto isolver : solvers) {

        // initialize a container for storing all solutions and register
        // relevant information about it for this specific solver
        khs::ksolutions_t<npancake_t> bag;
        bag.set_domain (get_domain ());
        bag.set_variant (variant);

        // for all values of k selected by the user
        for (auto ispec: kspec) {

            // consider this single specification of k values
            for (auto k = get<0>(ispec) ; k <= get<1>(ispec) ; k+= get<2>(ispec)) {

                // for all instances
                cout << endl;
                cout << " ⏺ " << isolver << " ( k=" << k <<  " ): " << endl;

                for (auto i = 0 ; i < instances.size () ; i++) {

                    // create a manager to solve this specific instance
                    khs::bsolver<npancake_t>* m = get_solver (isolver, instances[i], goal, k);

                    cout << " ⏵ "; cout.flush ();
                    auto ksolution = m->solve ();

                    // give a name to every individual solution
                    for (auto j = 0 ; j < ksolution.size () ; j++) {
                        ksolution[j].set_name (names[i] + "/" + to_string (1+j));
                    }

                    // and show the result on the standard output. Prior to
                    // that, give it a name so that it can be easily recognized
                    // and record also the name of the solver used
                    ksolution.set_name (names[i]);
                    ksolution.set_solver (m->signature ());
                    ksolution.doctor ();
                    cout << ksolution << endl;

                    // add this solution to all solutions generated by this
                    // solver
                    bag += ksolution;

                    // in case verbose output was requested, show every solution
                    // path to this instance
                    if (want_verbose) {
                        for (auto i = 0 ; i < ksolution.size (); i++) {
                            cout << "   → " << ksolution[i] << endl;
                        }
                    }

                    // free the manager
                    delete m;
                }
            }
        }

        // add all solutions generated to the results to report
        results += bag;
    }

    // and stop the clock
    tend = chrono::system_clock::now ();

    // and write all results to a csv file only in case the user provided a name
    // for the output csv file along with a summary of the number of errors
    // generated
    cout << endl;
    cout << " 🖹 Error summary:" << endl;
    cout << results.get_error_summary () << endl;
    // write_csv<npancake_t> (csvname, results);
    cout << " 🕒 CPU time: " << 1e-9*chrono::duration_cast<chrono::nanoseconds>(tend - tstart).count() << " seconds" << endl;
    cout << endl;

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
                 string& solver, string& filename, string& variant,
                 string& k_params, string& csvname,
                 bool& want_verbose) {

    int c;

    // Default values
    solver = "";
    filename = "";
    variant = "unit";
    k_params = "";
    csvname = "";
    want_verbose = false;

    while ((c = getopt_long (argc, argv,
                             "s"  /* solver */
                             "f"  /* file */
                             "r"  /* variant */
                             "k"  /* k */
                             "C"  /* csv */
                             "v"  /* verbose */
                             "h"  /* help */
                             "V", /* version */
                             long_options, (int *) 0)) != EOF) {
        switch (c) {
        case 's':  /* --solver */
            solver = optarg;
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
      -s, --solver [STRING]+     K shortest-path algorithms to use. Choices are: 'mDijkstra', 'belA0'. It is possible to provide\n\
                                 as many as desired in a blank separated list between double quotes, e.g. \"mDijkstra belA0\"\n\
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
