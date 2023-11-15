// -*- coding: utf-8 -*-
// roadmap.cc
// -----------------------------------------------------------------------------
//
// Started on <jue 12-10-2023 14:03:45.527928974 (1697112225)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// The roadmap domain is the 9th DIMACS Implementation Challenge: Shortest
// Paths http://www.dis.uniroma1.it/~challenge9
//

#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include <getopt.h>

#include "../solver.h"
#include "../../src/ksearch.h"

#include "roadmap_t.h"

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
    {"graph", required_argument, 0, 'g'},
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
void get_testcases (const string& filename, vector<instance_t<roadmap_t>>& instances);
static int decode_switches (int argc, char **argv,
                            string& graph_name, string& solver_name, string& filename, string& variant,
                            string& k_params, string& csvname, bool& no_doctor, bool& want_summary,
                            bool& want_verbose);
static void usage (int status);

// main entry point
int main (int argc, char** argv) {

    string graph_name;                        // file with the graph definition
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
    vector<string> variant_choices = {"unit", "dimacs"};

    // arg parse
    decode_switches (argc, argv, graph_name, solver_name, filename, variant, k_params, csvname, no_doctor, want_summary, want_verbose);

    // process the solver names and get a vector with the signatures of all
    // solvers to execute
    vector<string> solvers = split_solver (solver_name);

    // and also process the user selection of the K values
    auto kspec = split_ks (k_params);

    // parameter checking

    // --graph
    if (graph_name == "") {
        cerr << "\n Please, provide a filename with the description of the graph" << endl;
        cerr << " See " << program_name << " --help for more details" << endl << endl;
        exit(EXIT_FAILURE);
    }

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

    // initialize the static data members of the definition of a roadmap
    roadmap_t::init (graph_name, variant);
    auto nbedges = roadmap_t::get_graph ().get_nbedges ();

    // open the given file and retrieve all cases from it
    vector<instance_t<roadmap_t>> instances;
    get_testcases (filename, instances);
    if (!instances.size ()) {
        cerr << endl;
        cerr << " Error: The file '" << filename << "' contains no instances to solve!" << endl;
        cerr << endl;
        exit (EXIT_FAILURE);
    }

    /* !-------------------------------------------------------------------! */

    cout << endl;
    cout << " graph        : " << graph_name << " (" << nbedges << " edges processed)" << endl;
    cout << " solver       : " << solver_name << endl;
    cout << " file         : " << filename << " (" << instances.size () << " instances)" << endl;
    cout << " variant      : " << variant << endl;
    cout << " K            : ";
    for (auto& ispec: kspec) {
        cout << "[" << get<0>(ispec) << ", " << get<1>(ispec) << ", " << get<2> (ispec) << "] ";
    }
    cout << endl;

    /* !----------------------------- SEARCH ------------------------------! */

    // start the clock
    tstart = chrono::system_clock::now ();

    // create an instance of the "generic" domain-dependent solver
    solver<roadmap_t> manager (get_domain (), variant,
                               instances, k_params);

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
    return "roadmap";
}

// return the variant of the domain of this solver
const string get_variant () {
    return "dimacs";
}

// open the specified filename and retrieve the name of every instance (assigned
// as an integer from 0 onwards) and a vector of strings with the contents of
// each case, i.e., the start and goal vertices separated by a blank space
void get_testcases (const string& filename, vector<instance_t<roadmap_t>>& instances) {

   ifstream stream (filename);

   // read both lines and store its contents in two separated vectors. The first
   // line contains all the starting vertices whereas the second line contains
   // all goals
   int lineno = 0;                          // line number: 0->starts; 1->goals
   string line;
   vector<string> starts, goals;
   while (getline (stream, line)) {

       // create a regexp to split this line
       regex regex ("\\s+");
       sregex_token_iterator it (line.begin (), line.end (), regex, -1);
       sregex_token_iterator end;

       // and now process each token separately
       for ( ; it != end ; ++it) {
           if (lineno == 0) {                                   // start states
               starts.push_back (*it);
           } else if (lineno == 1) {                             // goal states
               goals.push_back (*it);
           } else {
               cerr << " Syntax error: the file with the tests cases should consist of two lines only" << endl;
               cerr << "               See the documentation for more information" << endl << endl;
               exit(EXIT_FAILURE);
           }
       }

       // increment the line counter
       lineno += 1;
   }

   // verify there are the same number of start and goal states
   if (starts.size () != goals.size ()) {
       cerr << " Syntax error: The file with the test cases must contain the same number of start and goal states" << endl;
       cerr << "               See the documentation for more information" << endl << endl;
       exit(EXIT_FAILURE);
   }

   // now, create a vector with the information of each instance
   for (auto i = 0 ; i < starts.size () ; i++) {
       instances.push_back (instance_t{to_string (i),
           roadmap_t {size_t (stoll (starts[i]))},
           roadmap_t {size_t (stoll (goals[i]))}});
   }
}

// Set all the option flags according to the switches specified. Return the
// index of the first non-option argument
static int
decode_switches (int argc, char **argv,
                 string& graph_name, string& solver_name, string& filename, string& variant,
                 string& k_params, string& csvname, bool& no_doctor, bool& want_summary,
                 bool& want_verbose) {

    int c;

    // Default values
    graph_name = "";
    solver_name = "";
    filename = "";
    variant = "dimacs";
    k_params = "";
    csvname = "";
    no_doctor = false;
    want_summary = false;
    want_verbose = false;

    while ((c = getopt_long (argc, argv,
                             "g"  /* graph */
                             "n"  /* solver */
                             "s"  /* solver */
                             "f"  /* file */
                             "r"  /* variant */
                             "k"  /* k */
                             "C"  /* csv */
                             "S"  /* summary */
                             "D"  /* no-doctor */
                             "v"  /* verbose */
                             "h"  /* help */
                             "V", /* version */
                             long_options, (int *) 0)) != EOF) {
        switch (c) {
        case 'g':  /* --graph */
            graph_name = optarg;
            break;
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
            cout << " khs (" << get_domain () << ") " << CMAKE_VERSION << endl;
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
    cout << endl << " " << program_name << " implements various K shortest-path search algorithms in the 9th DIMACS Implementation Challenge: Shortest Paths" << endl << endl;
    cout << " Usage: " << program_name << " [OPTIONS]" << endl << endl;
    cout << "\
 Mandatory arguments:\n\
      -g, --graph [STRING]       filename with the graph to load. The file contents should be arranged according to the 9th DIMACS\n\
                                 Implementation Challenge: Shortest Paths. See the documentation for additional help\n\
      -s, --solver [STRING]+     K shortest-path algorithms to use. Choices are: 'mDijkstra', 'belA0' and 'K0'. It is possible\n\
                                 to provide as many as desired in a blank separated list between double quotes, e.g.\n\
                                  \"mDijkstra belA0\"\n\
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
      -f, --file [STRING]        filename with the test cases to solve. It consists of of precisely two lines. The i-th test case\n\
                                 uses the i-th vertex from the first line as the starting vertex and the i-th vertex from the second\n\
                                 line as the goal.\n\
      -r, --variant [STRING]     Variant of the problem to consider. Choices are {unit, dimacs}. By default, 'dimacs' is used\n\
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
