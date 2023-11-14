// -*- coding: utf-8 -*-
// solver.h
// -----------------------------------------------------------------------------
//
// Started on <lun 28-08-2023 00:46:32.176420194 (1693176392)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Services of a domain-dependent solver for the k shortest-path problem
//

#ifndef _SOLVER_H_
#define _SOLVER_H_

#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "helpers.h"
#include "../src/ksearch.h"

// Definition of an instance
template <typename T>
class instance_t {

private:

    // INVARIANT: every instance consists of a name and a pair of states, the
    // start and goal state
    std::string _name;
    T _start;
    T _goal;

public:

    // Default constructors are forbidden
    instance_t () = delete;

    // Explicit constructor
    instance_t (std::string name, T start, T goal) :
        _name { name },
        _start { start },
        _goal { goal }
        {}

    // getters
    std::string get_name () const { return _name; }
    T get_start () const { return _start; }
    T get_goal () const { return _goal; }

}; // class instance_t<T>

// Definition of a "generic" domain-dependent solver for the k shortest-path
// problem. This solver registers a domain and a variant, a long with a number
// of k values to try and run specific solvers over a selection of instances.
// Results are automatically stored in a container
// ----------------------------------------------------------------------------
template <typename D>
class solver {

private:

    // INVARIANT: A domain-dependent solver is applied over a specific domain
    // and variant ---where the typename D is the domain type of application,
    // e.g., npancake. A vector of instances must be given, and also a
    // specification of the k values to try which are given as a string that has
    // to be decoded
    std::string _domain;                                              // Domain
    std::string _variant;                                            // Variant
    std::vector<instance_t<D>> _instances;                         // instances
    std::string _kspec;                        // specification of the k values

    // the k values to try are internally stored as a vector of tuples, each
    // specifying the first value to try, the last one and the step between
    // successive values of k
    std::vector<std::tuple<int, int, int>> _ks;

    // the results of the execution are internally stored in a container of
    // solutions of instances of the k shortest-path problem
    khs::ksolutions_t<D> _results;

    // methods

    // return a (plain) pointer to a specific solver for solving instances in
    // the specified domain D according to the given name. The solver is
    // initialized with the following data:
    //
    // * start: instance to solve
    // * goal: goal to reach
    // * k: number of paths to find
    khs::bsolver<D>* _get_solver (const std::string name,
                                  const D& start, const D& goal,
                                  const int k) {

        // create a pointer to a solver
        khs::bsolver<D>* m;

        // and now choose according to the given name
        if (name == "mDijkstra") {
            m = new khs::mA<D> (k, start, goal);
        } else if (name == "belA0") {
            m = new khs::bela<D> (k, start, goal, true);
        } else if (name == "K0") {
            m = new khs::kStarBlind<D> (k, start, goal, true, 20, 20, false);
        } else if (name == "K*") {
            m = new khs::kStar<D>(k, start, goal, true, 20, 20, false);
        } else if (name == "mA*") {
            m = new khs::mA<D> (k, start, goal);
        } else if (name == "belA*") {
            m = new khs::bela<D> (k, start, goal, false);
        } else {
            throw std::invalid_argument{"Unknown solver!"};
        }

        // and return a pointer to the selected solver
        return m;
    }

    public:

    // Default constructors are forbidden
    solver () = delete;

    // Explicit constructor. Both the domain and variant should be given, along
    // with a vector of instances to solve, and the specification of the k
    // values to try
    solver (std::string domain, std::string variant,
            std::vector<instance_t<D>> instances, std::string kspec) :
        _domain { domain },
        _variant { variant },
        _instances { instances },
        _kspec { kspec }
        {

            // process the specification of values of k and store the specific
            // limits for every application
            _ks = split_ks (_kspec);
        }

    // getters
    std::string get_domain () const { return _domain; }
    std::string get_variant () const { return _variant; }
    std::vector<instance_t<D>> get_instances () const { return _instances; }
    std::string get_kspec () const { return _kspec; }

    // methods

    // the following service automates the execution of the given solver over
    // all instances found in the test file with all values obtained from the
    // specification of the k values, and stores the results internally. Unless
    // no_doctor takes the value true, all single solution paths and also the
    // solutions to every instance of the k shortest-path problem are checked
    // for correctness. In case want_verbose takes the value true additional
    // information is shown on the standard output
    void run (std::string solver_name, bool no_doctor, bool want_verbose) {

        // set the domain and variant in the container of ksolutions
        _results.set_domain (_domain);
        _results.set_variant (_variant);

        // for all values of k selected by the user
        for (auto ispec: _ks) {

            // initialize a container for storing all solutions generated in this
            // execution
            khs::ksolutions_t<D> bag;
            bag.set_domain (_domain);
            bag.set_variant (_variant);

            // consider this single specification of k values
            for (auto k = std::get<0>(ispec) ; k <= std::get<1>(ispec) ; k+= std::get<2>(ispec)) {

                // for all instances
                std::cout << std::endl;
                std::cout << " ⏺ " << solver_name << " ( k=" << k <<  " ): " << std::endl;

                for (auto i = 0 ; i < _instances.size () ; i++) {

                    // create a manager to solve this specific instance
                    auto m = _get_solver (solver_name,
                                          _instances[i].get_start (),
                                          _instances[i].get_goal (),
                                          k);

                    std::cout << " ⏵ "; std::cout.flush ();
                    auto ksolution = m->solve ();

                    // give a name to every individual solution
                    for (auto j = 0 ; j < ksolution.size () ; j++) {
                        ksolution[j].set_name (_instances[i].get_name () + "/" + std::to_string (1+j));
                    }

                    // and show the result on the standard output. Prior to
                    // that, give it a name so that it can be easily recognized
                    // and record also the name of the solver used
                    ksolution.set_name (_instances[i].get_name ());
                    ksolution.set_solver (m->signature ());
                    if (!no_doctor) {
                        ksolution.doctor ();
                    }
                    std::cout << ksolution << std::endl;

                    // add this solution to all solutions generated by this
                    // solver
                    bag += ksolution;

                    // in case verbose output was requested, show every solution
                    // path to this instance
                    if (want_verbose) {
                        for (auto i = 0 ; i < ksolution.size (); i++) {
                            std::cout << "   → " << ksolution[i] << std::endl;
                        }
                    }

                    // free the manager
                    delete m;
                }
            }

            // add all solutions generated to the results to report
            _results += bag;
        }
    }

    // show an error summary of the results obtained by the different
    // invocations to run only if no_doctor is false
    void show_error_summary (bool no_doctor) const {

        std::cout << std::endl;
        if (no_doctor) {
            std::cout << " 📊 Error summary: <doctor disabled>" << std::endl;
        } else {

            // show the error summary of the results obtained by the different
            // invocations to run
            std::cout << " 📊 Error summary: " << std::endl;
            std::cout << _results.get_error_summary () << std::endl;
        }
    }

    // store the results in a csv file in case any has been given
    void write_csv (const std::string& filename) {

        // only in case a filename for the output csv file has been given
        if (filename.size ()) {

            // create the output file and truncate its contents in case any exists
            std::ofstream csvstream {filename, std::ios::out | std::ios::trunc};
            csvstream << _results;
            std::cout << " 🛈 CSV file generated!" << std::endl;
        } else {
            std::cout << " ⚠ No CSV output generated!" << std::endl;
        }
    }

}; // class solver<D>


#endif // _SOLVER_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
