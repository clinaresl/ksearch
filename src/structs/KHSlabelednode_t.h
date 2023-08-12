// -*- coding: utf-8 -*-
// KHSlabelednode_t.h
// -----------------------------------------------------------------------------
//
// Started on <vie 11-08-2023 19:11:53.773431333 (1691773913)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Definition of nodes as used by BELA*
//

#ifndef _KHSLABELEDNODE_T_H_
#define _KHSLABELEDNODE_T_H_

#include "KHSnode_t.h"

namespace khs {

    // Labeled backpointers are used to reconstruct the solution path and also
    // to discover bridges in a graph: if the optimal cost of the start state
    // plus the cost of an operator plus the cost of a suffix is larger than the
    // cost of any optimal path through this edge, a bridge has been found.
    // Because the cost of an optimal path to the start vertex of the bridge is
    // stored in the node, and all the different costs of all suffixes of the
    // ending vertex of the bridge are stored also in the node, the cost of the
    // operator has to be stored in the labeled backpointer.
    struct labeledbackpointer_t {

        // INVARIANT: every laeled backpointer contains a pointer to its parent
        // and it also stores the cost of the operator that generates the child
        // from its parent
        size_t _pointer;
        int _cost;

        // Constructor
        labeledbackpointer_t (size_t pointer, int cost) :
            _pointer { pointer },
            _cost { cost } {}

        // operators
        bool operator==(const labeledbackpointer_t& other) const {
            return _pointer == other._pointer && _cost == other._cost;
        }

        // getters
        const size_t& get_pointer () const { return _pointer; }
        const int& get_cost () const { return _cost; }
    }; // labeledbackpointer_t

    template <typename T>
    class labelednode_t : public node_t<T> {

    private:

        // INVARIANT: labeled backnodes use a vector of labeled backpointers to
        // reconstruct the solution path, and also to discover bridges.
        std::vector<labeledbackpointer_t> _backpointers;

        // In addition, every labeled backnode is equipped with a vector of
        // backward g-values to store the distinct costs of all suffixes from
        // them to the goal.
        std::vector<int> _gb;

    public:

        // Default constructors are forbidden
        labelednode_t () = delete;

        // Explicit constructors
        labelednode_t (const T& state, int h=0, int g=0) :
            node_t<T> { state, h, g }
        {}

        // Copy constructor and assignment constructor by default
        labelednode_t (const labelednode_t&) = default;
        labelednode_t (labelednode_t&&) = default;

        // getters
        const std::vector<labeledbackpointer_t>& get_backpointers () const
            { return _backpointers; }
        const labeledbackpointer_t& get_backpointer (size_t index) const
            { return _backpointers[index]; }
        const std::vector<int>& get_gb () const { return _gb; }
        const int get_gb (size_t index) const { return _gb[index]; }

        // operators

        // Adding inserts a new labeled backpointer to the list of labeled
        // backpointers of this node
        labelednode_t& operator+=(const labeledbackpointer_t& backpointer) {
            _backpointers.push_back(backpointer);
            return *this;
        }

        // Or, alternatively, the backward g-value of a new suffix
        labelednode_t& operator+=(const int gb) {
            _gb.push_back(gb);
            return *this;
        }

        friend std::ostream& operator<< (std::ostream& stream, const labelednode_t& node) {

            stream << " <" << node.get_state ();
            stream << ", " << node.get_g ();
            stream << " + " << node.get_h ();
            stream << " = " << node.get_f ();
            stream << "> (";
            for (auto bp: node.get_backpointers ()) {
                stream << "{" << bp.get_pointer () << ", " << bp.get_cost () << "} ";
            }
            stream << ") [";
            for (auto gb: node.get_gb ()) {
                stream << gb << " " ;
            }
            stream << " ]";

            return stream;
        }
    }; // labelednode_t<T>
} // namespace khs

namespace std {

    // Definition of a hash function for labelednodes. The definition is
    // included in the std namespace so that it can be used directly by the
    // functions implemented in that namespace
    template<typename T>
    struct hash<khs::labelednode_t<T>> {

        // hash function
        size_t operator() (const khs::labelednode_t<T>& node) const {

            // just return the hash value implemented in the hash function of
            // the underlying type which has to be defined in the std namespace
            return std::hash<T>{}(node.get_state ());
        }
    }; // struct hash<labelednode_t<T>>
} // namespace std


#endif // _KHSLABELEDNODE_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
