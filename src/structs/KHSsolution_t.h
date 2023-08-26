// -*- coding: utf-8 -*-
// KHSsolution_t.h
// -----------------------------------------------------------------------------
//
// Started on <mié 09-08-2023 11:38:56.226565865 (1691573936)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Representation of a single solution to any instance
//

#ifndef _KHSSOLUTION_T_H_
#define _KHSSOLUTION_T_H_

#include "../KHSdefs.h"
#include "KHSclosed_t.h"
#include "KHSnode_t.h"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <type_traits>

namespace khs {

    using namespace std;

    // Definition of different error types
    enum class solution_error {
        UNCHECKED,
        ERR_EXPANSIONS,
        ERR_START,
        ERR_GOAL,
        ERR_ADJACENT,
        ERR_SOLUTION_COST,
        ERR_INCR_COST,
        ERR_NUM_SOLUTIONS,
        ERR_DUPLICATE_PATH,
        NO_ERROR
    };

    template<typename T>
    class solution_t {

        // INVARIANT: a single solution of an instance (identified by a
        // distinctive name) of the k shortest-path problem consists of a
        // concatenation of states that get to the goal from the start state,
        // which is known to have a initial heuristic value. The solution has a
        // solution length (which is derived from the solution path) and a cost,
        // which are not necessarily the same. To compute the solution, a number
        // of nodes is expanded and a specific CPU running time is used, so that
        // the number of nodes expanded per second is automatically computed.
        string _name;                                          // instance name
        int _k;    // number of paths to compute in the k-shortest path problem
        T _start;                                                // start state
        T _goal;                                                  // goal state
        vector<T> _solution;           // solution path as a sequence of states
        int _h0;                       // heuristic distance of the start state
        int _length;                  // solution length, i.e., number of steps
        int _cost;           // solution cost, i.e., sum of the cost of all ops
        size_t _expansions;                       // total number of expansions
        double _cpu_time;                                   // elapsed CPU time
        double _expansions_per_second;       // number of expansions per second
        string _solver;

        // Finally, when verifying a solution, an error code shall be given
        solution_error _error_code;

    public:

        // Default constructors are strictly forbidden
        solution_t () = delete;

        // Explicit constructor - all data has to be provided but: the name
        // which is empty by default; the solution path, and the solution length
        // which is computed automatically from the solution path
        solution_t (const int k,
                    const vector<T>& solution, const T& start, const T& goal,
                    const int h0, const int cost, const size_t expansions,
                    const double cpu_time, const string solver) :
            _name       { ""         },
            _k          { k },
            _start      { start      },
            _goal       { goal       },
            _solution   { solution   },
            _h0         { h0         },
            _cost       { cost       },
            _expansions { expansions },
            _cpu_time   { cpu_time   },
            _solver     { solver     }
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
        const string& get_name () const
            { return _name; }
        const int get_k () const
            { return _k; }
        const T& get_start () const
            { return _start; }
        const T& get_goal () const
            { return _goal; }
        const vector<T>& get_solution () const
            { return _solution; }
        const int get_h0 () const
            { return _h0; }
        const int get_length () const
            { return _length; }
        const int get_cost () const
            { return _cost; }
        const size_t get_expansions () const
            { return _expansions; }
        const double get_cpu_time () const
            { return _cpu_time; }
        const double get_expansions_per_second () const
            { return _expansions_per_second; }
        const string& get_solver () const
            { return _solver; }
        const solution_error& get_error_code () const
            { return _error_code; }

        // setters
        void set_name (const string value)
            { _name = value;}
        void set_cpu_time (const double value)
            {
                // update the CPU time and in passing the number of expansions
                // per second
                _cpu_time = value;
                _expansions_per_second = double(_expansions) / _cpu_time;
            }

        // operator overloading
        solution_t& operator=(const solution_t&) = default;
        solution_t& operator=(solution_t&&) = default;

        bool operator==(const solution_t right) const {
            return _start == right.get_start () &&
                _goal == right.get_goal () &&
                _solution == right.get_solution () &&
                _length == right.get_length () &&
                _cost == right.get_cost () &&
                _expansions == right.get_expansions () &&
                _cpu_time == right.get_cpu_time ();
        }

        // methods

        // the following binarypredicate just verifies that the second item is a
        // descendant of the first one
        static bool adjacent (T item1, T item2) {

            // compute the children of item1. Neither the heuristic value of
            // this node, nor the goal matter as we are interested only in the
            // "textual" representation of children
            vector<tuple<int, int, T>> successors;
            item1.children (0, item2, successors);

            // now, verify whether item2 is any of these
            for (auto isuccessor : successors) {

                // if this is the item given second, return true
                if (get<2>(isuccessor) == item2) {
                    return true;
                }
            }

            // at this point, the element has not been found and thus, return
            // false
            return false;
        }

        // return the edge cost of the operator that gets to item2 from item1.
        // In case item2 is not among the successors of item1 a negative value
        // is returned
        static int edge_cost (T item1, T item2) {

            // compute the children of item1. Neither the heuristic value of
            // this node, nor the goal matter as we are interested only in the
            // "textual" representation of children
            vector<tuple<int, int, T>> successors;
            item1.children (0, item2, successors);

            // now, look for item2 among the successors
            for (auto isuccessor : successors) {

                // if this is the item given second, return the cost of this
                // edge
                if (get<2>(isuccessor) == item2) {
                    return get<0>(isuccessor);
                }
            }

            // at this point, the element has not been found and thus, return
            // a negative value
            return -1;
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

        // doctor verifies that this solution is correct by means of using check
        // with the static member adjacent. It also verifies the first state in
        // the path is the start state and that the last state in the path is
        // the goal
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

            // Finally, verify that the cost of the path equals the sum of the
            // edge costs in the solution path
            int cost = 0;
            for (auto it = _solution.begin () ; it != _solution.end ()-1 ; ++it) {
                cost += edge_cost (*it, *(1+it));
            }
            if (_cost != cost) {
                _error_code = solution_error::ERR_SOLUTION_COST;
            }
            return (_cost == cost);
        }

        // return a string representing the given code
        static const string get_error_msg (const solution_error& code) {
            switch (code) {
                case solution_error::UNCHECKED: return "? Unchecked";
                    break;
                case solution_error::NO_ERROR: return "✔ No error";
                    break;
                case solution_error::ERR_EXPANSIONS: return "✘ No solution found!";
                    break;
                case solution_error::ERR_START: return "✘ Error start";
                    break;
                case solution_error::ERR_GOAL: return "✘ Error goal";
                    break;
                case solution_error::ERR_ADJACENT: return "✘ Error adjacency";
                    break;
                case solution_error::ERR_SOLUTION_COST: return "✘ Error solution cost";
                    break;
                case solution_error::ERR_INCR_COST: return "✘ Error increasing cost";
                    break;
                case solution_error::ERR_DUPLICATE_PATH: return "✘ Error duplicate solution path";
                    break;
                case solution_error::ERR_NUM_SOLUTIONS: return "✘ Error number of solutions";
                    break;
            }
        }

        // stream out --- according to the .csv format using semicolon as a
        // separator
        friend ostream& operator<< (ostream& stream, const solution_t& solution) {

            // Data is written in the same specific order. Note: first, that no
            // header is generated and this falls within the responsibility of
            // the owner of a solution; second, the solution is not shown
            stream << solution.get_name () << ";";
            stream << solution.get_k () << ";";
            stream << solution.get_start () << ";";
            stream << solution.get_goal () << ";";
            stream << solution.get_h0 () << ";";
            stream << solution.get_length () << ";";
            stream << solution.get_cost () << ";";
            stream << solution.get_expansions () << ";";
            stream << solution.get_cpu_time () << ";";
            stream << solution.get_expansions_per_second () << ";";
            stream << solution.get_solver () << ";";
            stream << solution.get_error_msg (solution.get_error_code ());

            return stream;
        }

    }; // class solution_t<T>
} // namespace khs

#endif // _KHSSOLUTION_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
