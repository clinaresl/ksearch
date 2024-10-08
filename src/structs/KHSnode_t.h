// -*- coding: utf-8 -*-
// KHSnode_t.h
// -----------------------------------------------------------------------------
//
// Started on <lun 07-08-2023 15:25:29.491033546 (1691414729)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

//
// Definition of a generic node as used by a search algorithm
//

#ifndef _KHSNODE_T_H_
#define _KHSNODE_T_H_

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace khs {

    template<typename T>
    class node_t {

      private:

        // INVARIANT: every node refers to a particular state in a particular
        // domain and thus it stores that information. When generated by a
        // heuristic search algorithm it is also evaluated and its g-cost,
        // h-estimate and f-cost are computed.

        // Attributes
        T _state;                                                  // tree node
        int _h;                                                   // h estimate
        int _g;                                                       // g cost
        int _f;                                                      // f value

        // Nodes store a stable index to their position in closed list. Specific
        // definitions of nodes implement also backpointers to reconstruct the
        // solution path in different ways and thus, no definition of a
        // backpointer is given here
        size_t _pointer;

      public:

        // Default constructors are forbidden
        node_t () = delete;

        // Explicit constructors
        node_t (const T& state, int h=0, int g=0) :
            _state { state},
            _h { h },
            _g { g },
            _f { h + g },
            _pointer { std::string::npos }
        {}

        // Copy constructor and assignment constructor by default
        node_t (const node_t&) = default;
        node_t (node_t&&) = default;

        // get accessors
        const T& get_state () const
        { return _state; }
        int get_h () const
        { return _h; }
        int get_g () const
        { return _g; }
        int get_f () const
        { return _f; }
        size_t get_pointer () const
        { return _pointer; }

        // set modifiers

        // set the h-value of this node and update its f-value
        void set_h (int h) {
            _h = h;
            _f = _g + _h;
        }

        // set the g-value of this node and update its f-value
        void set_g (int g) {
            _g = g;
            _f = _g + _h;
        }

        // set the pointer of this node in closed
        void set_pointer (const size_t pointer) {
            _pointer = pointer;
        }

        // operator overloading
        node_t& operator=(const node_t&) = default;
        node_t& operator=(node_t&&) = default;

        virtual bool operator< (const node_t& right) const
        { return (_state < right.get_state ());}
        virtual bool operator==(const node_t& right) const
        { return _state == right.get_state ();}
        virtual bool operator!=(const node_t& right) const
        { return !(_state == right.get_state ()); }

        friend std::ostream& operator<< (std::ostream& stream, const node_t& node) {

            stream << " <" << node.get_state ();
            stream << ", " << node.get_g ();
            stream << " + " << node.get_h ();
            stream << " = " << node.get_f ();
            stream << ">";

            return stream;
        }

    }; // class node_t<T>
} // namespace khs

namespace std {

    // Definition of a hash function for nodes. The definition is included in
    // the std namespace so that it can be used directly by the functions
    // implemented in that namespace
    template<typename T>
    struct hash<khs::node_t<T>> {

        // hash function
        size_t operator() (const khs::node_t<T>& node) const {

            // just return the hash value implemented in the hash function of
            // the underlying type which has to be defined in the std namespace
            return std::hash<T>{}(node.get_state ());
        }

    }; // struct hash<node_t<T>>
} // namespace std


#endif // _KHSNODE_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
