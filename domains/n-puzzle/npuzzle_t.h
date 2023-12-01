// -*- coding: utf-8 -*-
// npuzzle_t.h
// -----------------------------------------------------------------------------
//
// Started on <sáb 04-12-2021 13:28:51.392403965 (1638620931)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Definition of a state of the N-puzzle
//

#ifndef _NPUZZLE_T_H_
#define _NPUZZLE_T_H_

#include<algorithm>
#include<initializer_list>
#include<iostream>
#include<iterator>
#include<string>
#include<tuple>
#include<utility>
#include<vector>

// Class definition
//
// Definition of a state of the N-puzzle
class npuzzle_t {

private:

    // INVARIANT: a state in the N sliding tile puzzle is characterized by its
    // length n, a permutation of ints in the range [0, n^2-1), and the location
    // of the blank
    static int _n;             // length of each side of the permutation puzzle
    int _blank;                // location of the blank in the range [0, n^2-1)
    std::vector<int> _perm;                                      // permutation

    // the following attributes speed up the expansion of nodes and the
    // heuristic evaluation by using precomputed tables which are necessarily
    // static
    static std::vector<std::vector<int>> _oprs;  // next locations of the blank

    // increment holds the net change of the Manhattan distance when moving a
    // tile (index #1) from its source location (index #2) to its destiny (index
    // #3) in a specific variant
    static std::string _variant;
    static std::vector<std::vector<std::vector<int> > > _increment;

    // methods

    // swaps the contents of locations i and j. It updates other internal data
    // if necessary
    void _swap (int i, int j);

public:

    // Default constructors are forbidden by default
    npuzzle_t () = delete;

    // A puzzle can be constructed from a vector of integers. This constructor
    // assumes the sliding-tile puzzle is squared and also correct, i.e., there
    // are no duplicates and all values are in the range [0, n^2)
    npuzzle_t (std::vector<int> perm);

    // In addition, it is also possible to create an instance of a sliding-tile
    // puzzle providing an intialization list
    npuzzle_t (std::initializer_list<int> perm) :
        npuzzle_t (std::vector<int>(perm))
        {}

    // getters
    static const int get_n () {
        return _n;
    }
    const int get_blank () const {
        return _blank;
    }
    const std::vector<int>& get_perm () const {
        return _perm;
    }
    const std::string& get_variant () const {
        return _variant;
    }

    // operator overloading

    // this instance is less than another if its permutation precedes it
    bool operator<(const npuzzle_t& right) const {
        return _perm < right.get_perm ();
    }

    // two instances are the same if they have the same permutation
    bool operator==(const npuzzle_t& right) const {
        return _perm == right.get_perm ();
    }

    // two instances are not the same if they have different permutations
    bool operator!=(const npuzzle_t& right) const {
        return _perm != right.get_perm ();
    }

    // get the contents of the i-th location. In case i is out of bounds the
    // results are undefined
    const int operator[](int i) const {
        return _perm[i];
    }

    friend std::ostream& operator<<(std::ostream& stream, const npuzzle_t& right) {

        // capture the permutation
        auto perm = right.get_perm ();

        // Show the permutation indicating the length of the side
        std::ostream_iterator<int> put (stream, " ");
        std::copy (perm.begin(), perm.end(), put);

        return stream;
    }

    // methods

    // the following function initialize all operators. Operators are the same
    // in either variant
    static void initops () {

        // reserve space enough for holding the operators table
        npuzzle_t::_oprs = std::vector<std::vector<int>> (_n*_n,
                                                          std::vector<int> ());

        // fill the operators table
        for (auto blank = 0 ; blank < _n*_n ; blank++) {

            // the following code is adapted from Korf's implementation of IDA*
            if (blank > _n - 1)                                 // not top edge
                _oprs[blank].push_back (blank-_n);             // add a move up
            if (blank < _n*_n - _n)                          // not bottom edge
                _oprs[blank].push_back (blank+_n);           // add a move down
            if (blank % _n < _n - 1)                          // not right edge
                _oprs[blank].push_back (blank+1);           // add a move right
            if (blank % _n > 0)                                // not left edge
                _oprs[blank].push_back (blank-1);            // add a move left
        }
    }

    // INIT pre-computes the incremental evaluation function table. For a given
    // tile moving from a given source position to a given destination position,
    // it returns the net change in the value of the Manhattan distance, which
    // is either +1 or -1:
    //
    //    Unit variant: moving any tile costs 1
    //    Heavy-cost variant: moving tile x costs x
    //
    // This function initializes the static data member _variant which, by
    // default is always the unit domain
    static void init (const std::string& variant = "unit") {

        int tile;                                       // tile to be moved
        int source, dest;               // source and destination positions
        int destindex;               // destination index in operator table

        // copy the domain variant used for initializing the increment table
        npuzzle_t::_variant = variant;

        // reserve space enough for holding the increment table
        npuzzle_t::_increment = std::vector<std::vector<std::vector<int> > >(_n*_n,
                                                                             std::vector<std::vector<int> > (_n*_n,
                                                                                                             std::vector<int>(_n*_n,
                                                                                                                              0)));

        // the following code is adapted from Korf's implementation of IDA*
        for (tile = 1; tile < _n*_n; tile++) {
            for (source = 0; source < _n*_n; source++) {
                for (destindex = 0; destindex < (int)_oprs[source].size (); destindex++) {

                    // consider all the legal destinations of source. 'dest' is
                    // the new position of the blank
                    dest = _oprs[source][destindex];

                    // compute the increment in the value of the heuristic
                    // function for moving 'tile' from 'source' to 'dest'
                    if (variant == "unit") {
                        _increment[tile][source][dest]  = abs((tile % _n) - (dest % _n))
                            - abs((tile % _n) - (source % _n))
                            + abs((tile / _n) - (dest / _n))
                            - abs((tile / _n) - (source / _n));
                    } else if (variant == "heavy-cost") {
                        _increment[tile][source][dest]  = tile * (
                            abs((tile % _n) - (dest % _n))
                            - abs((tile % _n) - (source % _n))
                            + abs((tile / _n) - (dest / _n))
                            - abs((tile / _n) - (source / _n)));
                    } else {
                        throw std::invalid_argument ("[npuzzle_t::init] Unknown variant!");
                    }
                }
            }
        }
    }

    // return the children of this state which is known to have the given
    // heuristic value as a vector of tuples, each containing: first, the cost
    // of the operator; secondly, its heuristic value; thirdly, the successor
    // state. Note the goal is also given in case that incremental policies of
    // the computation of the heuristic value are not used
    void children (int h, const npuzzle_t& goal,
                   std::vector<std::tuple<int, int, npuzzle_t>>& successors);

    // return the heuristic distance to get from this permutation to the
    // identity permutation, i.e., the given goal is ignored
    int h (const npuzzle_t& goal) const;

}; // class npuzzle_t

namespace std {

    // Definition of a hash function for instances of the sliding-tile puzzle.
    // The definition is included in the std namespace so that it can be used
    // directly by the functions implemented in that namespace
    template<>
    struct hash<::npuzzle_t> {

        // hash function
        size_t operator() (const npuzzle_t& right) const {

            // return the hash value of the vector of integers representing this
            // specific permutation
            // https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector
            size_t seed = right.get_perm ().size();
            for(auto& i : right.get_perm ()) {
                seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }

    }; // struct hash<npuzzle_t>
}


#endif // _NPUZZLE_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
