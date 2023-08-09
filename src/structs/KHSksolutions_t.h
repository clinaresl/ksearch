// -*- coding: utf-8 -*-
// KHSksolutions_t.h
// -----------------------------------------------------------------------------
//
// Started on <mié 09-08-2023 14:36:59.916187963 (1691584619)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Definition of a container of solutions of various instances of the k-shortest
// path problem
//

#ifndef _KHSKSOLUTIONS_T_H_
#define _KHSKSOLUTIONS_T_H_

#include <algorithm>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "KHSksolution_t.h"

namespace khs {

    template<typename T>
    class ksolutions_t {

        // INVARIANT: a sequential container of solutions of the k-shortest path
        // problem consist just of a vector of them that refers to the same
        // domain/variant
        std::vector<ksolution_t<T>> _ksolutions;      // container of solutions
        std::string _domain;
        std::string _variant;

    public:

        // Default constructor
        ksolutions_t () :
            _ksolutions { std::vector<ksolution_t<T>>() },
            _domain    { "" },
            _variant { "" }
            {}

        // copy and move constructors are forbidden
        ksolutions_t (const ksolutions_t&) = delete;
        ksolutions_t (ksolutions_t&&) = delete;

        // getters
        const std::vector<ksolution_t<T>>& get_ksolutions () const {
            return _ksolutions;
        }
        const std::string get_domain () const {
            return _domain;
        }
        const std::string get_variant () const {
            return _variant;
        }

        // setters
        void set_domain (const std::string domain) {
            _domain = domain;
        }
        void set_variant (const std::string variant) {
            _variant = variant;
        }

        // operator overloading

        // assignment and move operators are dissallowed as well
        ksolutions_t& operator=(const ksolutions_t&) = delete;
        ksolutions_t& operator=(ksolutions_t&&) = delete;

        // ksolutions can be augmented by either adding a single solution of the
        // k-shortest path problem or another container of ksolutions
        ksolutions_t& operator+= (const ksolution_t<T>& right) {

            // add this solution to the container
            _ksolutions.push_back (right);
            return *this;
        }
        ksolutions_t& operator+= (const ksolutions_t& right) {

            for (const auto& s: right.get_ksolutions ()) {
                _ksolutions.push_back (s);
            }
            return *this;
        }

        // random access operator
        const ksolution_t<T> operator[] (const size_t idx) const {
            if (idx >= _ksolutions.size ()) {
                throw std::out_of_range ("[ksolutions_t::get] out of bounds!");
            }
            return _ksolutions[idx];
        }

        // stream out
        friend ostream& operator<< (std::ostream& stream, const ksolutions_t& solutions) {

            // data is written in the csv format using the semicolon as
            // separator. Because each line results of the concatenation of data
            // provided by this instance and also its specific solutions, write
            // first to a string stream and then copy its output to the given
            // stream
            std::stringstream ss;
            ss << "domain;variant;id;start;goal;h0;cost;expansions;runtime;expansions/sec;solver" << endl;
            for (const auto& s : solutions.get_ksolutions ()) {
                ss << solutions.get_domain () << ";";
                ss << solutions.get_variant () << ";";
                ss << s << std::endl;
            }

            // and now redirect the contents of the string stream to the given
            // stream
            stream << ss.str ();

            return stream;
        }

        // methods
        size_t size () const {
            return _ksolutions.size ();
        }

    }; // class ksolutions_t
} // namespace khs

#endif // _KHSKSOLUTIONS_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
