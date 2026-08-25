// -*- coding: utf-8 -*-
// KHSsolution_t.h
// -----------------------------------------------------------------------------
//
// Started on <mié 09-08-2023 11:38:56.226565865 (1691573936)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

//
// Representation of a single solution to any instance
//

#ifndef _KHSSOLUTION_T_H_
#define _KHSSOLUTION_T_H_

#include "../KHSdefs.h"
#include "KHSiomanip.h"
#include "KHSclosed_t.h"
#include "KHSnode_t.h"

#include <algorithm>
#include <format>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>
#include <type_traits>

namespace khs {

    // Definition of different error types
    enum class solution_error {
        UNCHECKED,
        ERR_EXPANSIONS,
        ERR_START,
        ERR_GOAL,
        ERR_ADJACENT,
        ERR_SOLUTION_COST,
        ERR_INCR_COST,
        ERR_DUPLICATE_PATH,
        ERR_NUM_SOLUTIONS,
        ERR_NON_SIMPLE_PATH,
        NO_ERROR
    };

    template<typename T, template <typename...> class path_t>
    class solution_t {

        // INVARIANT: a single solution of an instance (identified by a
        // distinctive name) of the k shortest-path problem consists of a
        // concatenation of states that get to the goal from the start state,
        // which is known to have a initial heuristic value, and which are
        // discovered by using a number of centroids. The solution has a
        // solution length (which is derived from the solution path) and a cost,
        // which are not necessarily the same. To compute the solution, a number
        // of nodes is expanded and a specific CPU running time and memory are
        // used, so that the number of nodes expanded per second is
        // automatically computed.
        std::string _name;                                     // instance name
        int _k;    // number of paths to compute in the k-shortest path problem
        T _start;                                                // start state
        T _goal;                                                  // goal state
        path_t<T> _solution;           // solution path as a sequence of states
        int _z_exp;                            // number of centroid expansions
        int _z_gen;                            // number of generated centroids
        int _h0;                       // heuristic distance of the start state
        int _length;                  // solution length, i.e., number of steps
        int _cost;           // solution cost, i.e., sum of the cost of all ops
        size_t _expansions;                       // total number of expansions
        double _cpu_time;                                   // elapsed CPU time
        double _expansions_per_second;       // number of expansions per second
        size_t _mem_usage;                                // Memory usage in MB

        // every solution records the solver name that generates it and whether
        // that solver generates simple paths or not
        std::string _solver;                                     // solver name
        bool _simple;         // whether solution paths should be simple or not

        // Also, and only in the context of bBELA we are interested in computing
        // the *extra* number of paths that are necessary to get k paths, i.e.,
        // the number of non-simple paths generated while looking for k simple
        // paths. This parameter is reported with value 0 for all the other
        // "simple algorithms"
        int _nbpaths;

        // Finally, when verifying a solution, an error code shall be given
        solution_error _error_code;

    public:

        // Default constructors are strictly forbidden
        solution_t () = delete;

        // Explicit constructor - all data has to be provided but: the name
        // which is empty by default; the solution path, and the solution length
        // which is computed automatically from the solution path. Memory usage
        // is always zero for every single solution, but the last one, i.e.,
        // memory usage is measured only when the k shortest paths have been found
        solution_t (const int k,
                    const path_t<T>& solution, const T& start, const T& goal,
                    const int z_exp, const int z_gen, const int h0, const int cost,
                    const size_t expansions, const double cpu_time,
                    const int nbpaths, const std::string solver, const bool simple) :
            _name              { ""                },
            _k                 { k                 },
            _start             { start             },
            _goal              { goal              },
            _z_exp             { z_exp             },
            _z_gen             { z_gen             },
            _solution          { solution          },
            _h0                { h0                },
            _cost              { cost              },
            _expansions        { expansions        },
            _cpu_time          { cpu_time          },
            _mem_usage         { 0                 },
            _nbpaths           {           nbpaths },
            _solver            { solver            },
            _simple            { simple            }
            {

                // automatically compute the length of the solution path and the
                // number of expansions per second
                _length = _solution.size () - 1;
                _expansions_per_second = double(_expansions) / _cpu_time;

                // Initially solutions are not checked unless the doctor service
                // is invoked
                _error_code = solution_error::UNCHECKED;
            }

        // copy constructors and move constructors are used by default
        solution_t (const solution_t&) = default;
        solution_t (solution_t&&) = default;

        // getters
        const std::string& get_name () const
            { return _name; }
        int get_k () const
            { return _k; }
        const T& get_start () const
            { return _start; }
        const T& get_goal () const
            { return _goal; }
        int get_z_exp () const
            { return _z_exp; }
        int get_z_gen () const
            { return _z_gen; }
        const path_t<T>& get_solution () const
            { return _solution; }
        int get_h0 () const
            { return _h0; }
        int get_length () const
            { return _length; }
        int get_cost () const
            { return _cost; }
        size_t get_expansions () const
            { return _expansions; }
        double get_cpu_time () const
            { return _cpu_time; }
        size_t get_mem_usage () const
            { return _mem_usage; }
        double get_expansions_per_second () const
            { return _expansions_per_second; }
        int get_nbpaths () const
            { return _nbpaths; }
        const std::string& get_solver () const
            { return _solver; }
        bool get_simple () const
            { return _simple; }
        const solution_error& get_error_code () const
            { return _error_code; }

        // setters
        void set_name (const std::string value)
            { _name = value;}
        void set_cpu_time (const double value) {

            // update the CPU time and in passing the number of expansions
            // per second
            _cpu_time = value;
            _expansions_per_second = double(_expansions) / _cpu_time;
        }
        // memory usage has to be explicitly given
        void set_mem_usage (const size_t value)
            { _mem_usage = value; }

        // operator overloading
        solution_t& operator=(const solution_t&) = default;
        solution_t& operator=(solution_t&&) = default;

        bool operator==(const solution_t right) const {
            return _start == right.get_start () &&
                _goal == right.get_goal () &&
                _z_exp == right.get_z_exp () &&
                _z_gen == right.get_z_gen () &&
                _solution == right.get_solution () &&
                _length == right.get_length () &&
                _cost == right.get_cost () &&
                _expansions == right.get_expansions () &&
                _cpu_time == right.get_cpu_time ();
        }

        // Conversion operator to make solution stored as vector
        //
        // Allows us to homogenize solutions after the fact if we want to store
        // them with solutions from solvers which return solutions in a
        // different container
        operator solution_t<T, std::vector>() {
            return solution_t<T, std::vector>(this->_k, std::vector<T>(_solution.begin(), _solution.end()),
                                              this->_start, this->_goal, this->_z_exp, this->_z_gen, 
                                              this->_h0, this->_cost, this->_expansions,
                                              this->_cpu_time, this->_nbpaths, this->_solver, this->_simple);
        }


        // methods

        // the following binarypredicate just verifies that the second item is a
        // descendant of the first one
        static bool adjacent (T item1, T item2) {

            // compute the children of item1. Neither the heuristic value of
            // this node, nor the goal matter as we are interested only in the
            // "textual" representation of children. Admittedly, this
            // implementation examines all children even if the child of
            // interest is found early. My current implementation of children
            // does not allow stopping the generation of all descendants
            bool found = false;
            item1.children (
                0,
                item2,
                [&] (int g, int h, T&& successor) {

                    // if this is the item given second, annotate it
                    // was found
                    found |= (successor == item2);
                });

            // and return whether the element was found, or not
            return found;
        }

        // return the edge cost of the operator that gets to item2 from item1.
        // In case item2 is not among the successors of item1 a negative value
        // is returned
        static int edge_cost (T item1, T item2) {

            // compute the children of item1 and check whether item2 is among
            // its descendants. If so, return the cost of the operator. Neither
            // the heuristic value of this node, nor the goal matter as we are
            // interested only in the "textual" representation of children.
            // Admittedly, this implementation examines all children even if the
            // child of interest is found early. My current implementation of
            // children does not allow stopping the generation of all
            // descendants
            int op_cost = -1;
            item1.children (
                0,
                item2,
                [&] (int g, int h, T&& successor) {

                    // if this is the item given second, annotate
                    // the operator cost
                    if (successor == item2) {
                        op_cost = g;
                    }
                });

            // and return the cost of the operator that generates item2, or -1
            // if it was not found
            return op_cost;
        }

        // while the operator== verifies that all data members of two different
        // single solutions are equal, the following service verifies if both
        // instances have the same solution path or not
        bool same_solution_path (const solution_t& right) const {

            if (_solution.size () != right.get_solution ().size ()) {
                return false;
            }

            // verify that all states in the solution path are the same
            for (auto i = 0 ; i < _solution.size () ; i++) {
                if (!(_solution[i] == right.get_solution ()[i])) {
                    return false;
                }
            }

            // at this point, both solution paths are known to be strictly equal
            return true;
        }

        // given a function which returns true for a pair of states T if the
        // transition between them is *correct* it is possible to verify that
        // all transitions are correct if no pair of adjacent states return
        // false
        template<class BinaryPredicate>
        bool check (BinaryPredicate func) const {
            return adjacent_find_not (_solution.begin (), _solution.end (), func) == _solution.end ();
        }

        // doctor verifies that this solution is correct:
        //
        // 1. empty solution paths are generated without ever expanding a single
        //    node. This is not true if the given instance had no solution but
        //    this never happens in the experiments performed here.
        //
        // 2. verify every solution path starts with the start state and ends in
        //    the goal state
        //
        // 3. verify that every path contains states which are indeed adjacent
        //
        // 4. verify that the solution cost equals the sum of the cost edges
        //
        // 5. in case the manager generating this solution is required to
        //    generate solution paths, then whether the solution contains loops or
        //    not is checked also.
        bool doctor () {

            // By default, no error is detected
            _error_code = solution_error::NO_ERROR;

            // in case there is no solution path, verify whether there are
            // expansions. If none, then the solution is correct (typical case
            // when start==goal); however, if a number of expansions were
            // performed then the solution is incorrect (typical case when a
            // solution is not found)
            if (!_solution.size ()) {
                if (_expansions != 0) {
                    _error_code = solution_error::ERR_EXPANSIONS;
                }
                return _expansions == 0;
            }

            // Otherwise, verify the first state in the solution path is the
            // start state
            if (!(_start == _solution[0])) {
                _error_code = solution_error::ERR_START;
                return false;
            }

            // Likewise, ensure the last state of the solution path is the goal
            // state as well
            if (!(_goal == _solution[_solution.size ()-1])) {
                _error_code = solution_error::ERR_GOAL;
                return false;
            }

            // Verify that each state is a descendant of the previous state in
            // the solution path
            if (!check (adjacent)) {
                _error_code = solution_error::ERR_ADJACENT;
                return false;
            }

            // Verify that the cost of the path equals the sum of the
            // edge costs in the solution path
            int cost = 0;
            for (auto it = _solution.begin () ; it != _solution.end ()-1 ; ++it) {
                cost += edge_cost (*it, *(1+it));
            }
            if (_cost != cost) {
                _error_code = solution_error::ERR_SOLUTION_COST;
                return false;
            }

            // Finally, in case the manager generating this solution is required
            // to generate only simple solution paths, then it is verified
            // whether the path is loopless or not
            if (_simple) {

                // traverse the entire solution path and store every state in an
                // unordered set (which has amortized constant time).
                std::unordered_set<T> visited;
                for (const auto& istate: _solution) {

                    // first, verify this node has not been traversed yet
                    if (visited.find (istate) != visited.end ()) {
                        _error_code = solution_error::ERR_NON_SIMPLE_PATH;
                        return false;
                    }

                    // If not found, add it to the unordered set
                    visited.insert (istate);
                }
            }

            // At this point, the solution is known to be correct
            return true;
        }

        // return a string representing the given code. If "color" takes the
        // value true then it is coloured and false otherwise
        static const std::string get_error_msg (const solution_error& code, bool const color = false) {
            switch (code) {
            case solution_error::UNCHECKED: return std::format ("{}{:>{}}{}", (color ? ansi::LightYellow : ""), "? Unchecked", doctor_width, (color ? ansi::reset : ""));
                    break;
            case solution_error::NO_ERROR: return std::format ("{}{:>{}}{}", (color ? ansi::LightSteelBlue : ""), "✔ No error", doctor_width, (color ? ansi::reset : ""));
                    break;
                case solution_error::ERR_EXPANSIONS: return std::format ("{}{:>{}}{}", (color ? ansi::Tomato : ""), "✘ No solution found!", doctor_width, (color ? ansi::reset : ""));
                    break;
                case solution_error::ERR_START: return std::format ("{}{:>{}}{}", (color ? ansi::Tomato : ""), "✘ Error start", doctor_width, (color ? ansi::reset : ""));
                    break;
                case solution_error::ERR_GOAL: return std::format ("{}{:>{}}{}", (color ? ansi::Tomato : ""), "✘ Error goal", doctor_width, (color ? ansi::reset : ""));
                    break;
                case solution_error::ERR_ADJACENT: return std::format ("{}{:>{}}{}", (color ? ansi::Tomato : ""), "✘ Error adjacency", doctor_width, (color ? ansi::reset : ""));
                    break;
                case solution_error::ERR_SOLUTION_COST: return std::format ("{}{:>{}}{}", (color ? ansi::Tomato : ""), "✘ Error solution cost", doctor_width, (color ? ansi::reset : ""));
                    break;
                case solution_error::ERR_INCR_COST: return std::format ("{}{:>{}}{}", (color ? ansi::Tomato : ""), "✘ Error increasing cost", doctor_width, (color ? ansi::reset : ""));
                    break;
                case solution_error::ERR_DUPLICATE_PATH: return std::format ("{}{:>{}}{}", (color ? ansi::Tomato : ""), "✘ Error duplicate solution path", doctor_width, (color ? ansi::reset : ""));
                    break;
            case solution_error::ERR_NUM_SOLUTIONS: return std::format ("{}{:>{}}{}", (color ? ansi::Tomato : ""), "✘ Error number of solutions", doctor_width, (color ? ansi::reset : ""));
                    break;
                case solution_error::ERR_NON_SIMPLE_PATH: return std::format ("{}{:>{}}{}", (color ? ansi::Tomato : ""), "✘ Error non-simple path", doctor_width, (color ? ansi::reset : ""));
                    break;
            }
        }

        // provide a numbers of headers with the name of all fields when
        // printing the contents of this instance on a stream. The headers must
        // be given a formatter to know how to show the headers
        std::string headers (const formatter& fmt) const {

            std::stringstream ss;

            // When printing a single solution, all the information in this
            // instance is shown in csv mode. However, in console mode only the
            // most relevant fields are shown
            if (fmt.mode == "csv") {
                ss << "id;k;start;goal;h0;length;cost;#expansions;Xz(k);Nz(k);#paths;mem usage;runtime;expansions/sec.;#inconsistencies;air;solver;doctor" << std::endl;
            } else {

                ss << std::format(
                    "{}"
                    "{:>{}}{:>{}}{:>{}}"
                    "{:>{}}{:>{}}{:>{}}"
                    "{:>{}}{:>{}}{:>{}}{:>{}}"
                    "{:>{}}"
                    "{}",

                    (fmt.mode == "color" ? ansi::DimGoldenrod : ""),
                    
                    "id",     6+name_width,
                    "length", length_width, 
                    "cost",   cost_width, 
                    
                    "#expansions", expansions_width, 
                    "Xz(k)",       centroids_width, 
                    "Nz(k)",       centroids_width, 
                    
                    "#paths",          paths_width, 
                    "mem usage",       memory_width, 
                    "runtime",         runtime_width, 
                    "expansions/sec.", expansions_sec_width, 
                    
                    "doctor",           doctor_width,

                    (fmt.mode == "color" ? ansi::reset : "")
                    );
            }

            return ss.str ();
        }

        // stream out --- according to the .csv format using semicolon as a
        // separator
        friend std::ostream& operator<< (std::ostream& stream, const solution_t& solution) {

            // Get the internal flags in ::ios and show contents accordingly
            std::stringstream ss;
            int mode = stream.iword (index ());
            if (mode == 0) {

                // Data is written in the same specific order. Note: first, that no
                // header is generated and this falls within the responsibility of
                // the owner of a solution; second, the solution is not shown
                ss << solution.get_name () << ";";
                ss << solution.get_k () << ";";
                ss << solution.get_start () << ";";
                ss << solution.get_goal () << ";";
                ss << solution.get_h0 () << ";";
                ss << solution.get_length () << ";";
                ss << solution.get_cost () << ";";
                ss << solution.get_expansions () << ";";
                ss << solution.get_z_exp () << ";";
                ss << solution.get_z_gen () << ";";
                ss << solution.get_nbpaths () << ";";
                ss << solution.get_mem_usage () << ";";
                ss << solution.get_cpu_time () << ";";
                ss << solution.get_expansions_per_second () << ";";
                ss << solution.get_solver () << ";";
                ss << solution.get_error_msg (solution.get_error_code ());
            } else {

                // in console mode use the same width used in headers, with the
                // only exception of the name which is six characters shorter.
                // This is hand-tailored and the reason is that solvers take
                // exactly three characters to prefix each line                
                ss << std::format (
                    "{}{:>{}}{}"
                    "{}{:>{}}{:>{}}"
                    "{:>{}}{:>{}}{:>{}}"
                    "{:>{}}{:>{}}{:>{}.{}f}{:>{}.{}f}"
                    "{}"
                    "{:>{}}",

                    (mode == 2 ? ansi::DimLightSteelBlue : ""),
                    solution.get_name (),   name_width,
                    (mode == 2 ? ansi::reset : ""),

                    (mode == 2 ? ansi::DimTan : ""),
                    solution.get_length (), length_width,
                    solution.get_cost (),   cost_width,

                    solution.get_expansions (),  expansions_width,
                    solution.get_z_exp (),       centroids_width,
                    solution.get_z_gen (),       centroids_width,
                    
                    solution.get_nbpaths (),               paths_width,
                    solution.get_mem_usage (),             memory_width,
                    solution.get_cpu_time (),              runtime_width, precision,
                    solution.get_expansions_per_second (), expansions_sec_width, precision,
                    (mode == 2 ? ansi::reset : ""),

                    solution_t<T, path_t>::get_error_msg (solution.get_error_code (), (mode==2)), doctor_width
                    );                
            }

            // and now redirect the contents of the string stream to the given
            // stream
            stream << ss.str ();

            return stream;
        }

    }; // class solution_t<T>
} // namespace khs

#endif // _KHSSOLUTION_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
