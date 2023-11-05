// -*- coding: utf-8 -*-
// npancake_t.h
// -----------------------------------------------------------------------------
//
// Started on <sáb 26-02-2022 18:07:39.045877735 (1645895259)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Definition of a state of the N-pancake
//

#ifndef _NPANCAKE_T_H_
#define _NPANCAKE_T_H_

#include<algorithm>
#include<cassert>
#include<iostream>
#include<iterator>
#include<stdexcept>
#include<string>
#include<tuple>
#include<utility>
#include<vector>


// Class definition
//
// Defintion of a state of the N-pancake
class npancake_t {

private:

    // INVARIANT: a state in the N pancake is characterized by its length n and
    // a permutation of ints in the range [0, n)
    static int _n;                                 // length of the permutation
    std::vector<int> _perm;                                      // permutation

    // increment holds the net change of the gap heuristic when flipping an
    // arbitrary number of discs in a specific variant. For a given disc in the
    // head of the pancake (index #1) it computes the net change in the
    // evaluation function when been located at the position that contains
    // another disc (index #2) which is adjacent to a third disc (index #3).
    // Note that the third index might be the table
    static std::string _variant;
    static std::vector<std::vector<std::vector<int> > > _increment;

    // methods

    // flip the first k positions of this permutation
    std::vector<int> _flip (int k);

public:

    // Default constructors are forbidden by default
    npancake_t () = delete;

    // A permutation can be constructed from a vector of integers. This
    // constructor assumes that all integers are distinct and belong to the
    // range [0, n)
    npancake_t (std::vector<int> perm) :
        _perm { perm }
        { _n = perm.size (); }

    // And also with an initializer list
    npancake_t (std::initializer_list<int> perm) :
        _perm { perm }
        { _n = perm.size (); }

    // getters
    static const int get_n () {
        return _n;
    }
    const std::vector<int>& get_perm () const {
        return _perm;
    }
    const std::string& get_variant () const {
        return _variant;
    }

    // operator overloading

    // this instance is less than another if its permutation precedes it
    bool operator<(const npancake_t& right) const {
        return _perm < right.get_perm ();
    }

    // two instances are the same if they have the same permutation
    bool operator==(const npancake_t& right) const {
        return _perm == right.get_perm ();
    }

    // two instancse are not the same if they have different permutations
    bool operator!=(const npancake_t& right) const {
        return _perm != right.get_perm ();
    }


    // get the contents of the i-th location. In case i is out of bounds the
    // results are undefined
    const int operator[](int i) const {
        return _perm[i];
    }

    friend std::ostream& operator<<(std::ostream& stream, const npancake_t& right) {

        // capture the permutation
        auto perm = right.get_perm ();

        // Show the permutation indicating the length of the side
        std::ostream_iterator<int> put (stream, " ");
        std::copy (perm.begin(), perm.end(), put);

        return stream;
    }

    // methods

    // INIT pre-computes the incremental evaluation function table. For a given
    // disc in the head of the pancake (index #1) it computes the net change in
    // the evaluation function when been located at the position that contains
    // another disc (index #2) which is adjacent to a third disc (index #3).
    // Note that the third index might be the table:
    //
    //    Unit variant: moving any tile costs 1
    //    Heavy-cost variant: moving tile x costs x
    //
    // This function initializes the static data member _variant which, by
    // default is always the unit domain
    static void init (const std::string& variant = "unit") {

        // copy the domain variant used for initializing the increment table
        npancake_t::_variant = variant;

        // reserve space enough for holding the increment table. The first and
        // second discs might be any in the range [0, _n) whereas the last one
        // should include the table so that it takes values in the range [0, _n]
        npancake_t::_increment = std::vector<std::vector<std::vector<int> > >(_n,
                                                                              std::vector<std::vector<int> > (_n,
                                                                                                              std::vector<int>(1+_n,
                                                                                                                               0)));
        // next process all combinations for the head (disc #1), tail (disc #2)
        // and next (disc #3). Note that while head and tail iterate over all
        // discs, next gets to n, the table
        for (int head = 0 ; head < _n ; head++) {
            for (int tail = 0 ; tail < _n ; tail++) {
                if (head != tail) {
                    for (int next = 0 ; next <= _n ; next++) {
                        if (head != next && tail != next) {

                            // compute the net change in the value of the gap
                            // heuristic when the head is moved to the location
                            // where the tail resides taking into account that
                            // tail is adjacent to next (and next might be equal
                            // to the size of the permutation representing the
                            // table)

                            // gap1 checks whether there is a gap between tail
                            // and next. gap2 checks whether there is a gap
                            // between head and next
                            bool gap1 = (abs (tail - next) != 1);
                            bool gap2 = (abs (head - next) != 1);

                            // and now annotate the net change of the gap
                            // heuristic for this combination which can be
                            // either zero, -1 or +1 in the unit variant and an
                            // arbitrary amount in the heavy-cost variant. Note
                            // that zero is not written down since the whole
                            // table is initialized to zeroes.
                            if (variant == "unit") {

                                // in the case of the unit variant, just simply
                                // take the differences in the number of gaps:
                                // those created in the child minus those
                                // created for the parent
                                _increment[head][tail][next] = int (gap2) - int (gap1);
                            } else if (variant == "heavy-cost") {

                                // in the heavy-cost variant, weight the same
                                // difference computed in the unit variant with
                                // the minimum disc involved in the gap
                                _increment[head][tail][next] = std::min (head, next) * int (gap2) -
                                    std::min (tail, next) * int (gap1);
                            }
                        }
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
    void children (int h, const npancake_t& goal,
                   std::vector<std::tuple<int, int, npancake_t>>& successors);

    // return the heuristic distance to get from this permutation to the
    // identity permutation, i.e., the given goal is ignored
    int h (const npancake_t& goal) const;

}; // class npancake_t

namespace std {

    // Definition of a hash function for instances of the n-pancake. The
    // definition is included in the std namespace so that it can be used
    // directly by the functions implemented in that namespace
    template<>
    struct hash<::npancake_t> {

        // hash function
        size_t operator() (const npancake_t& right) const {

            // return the hash value of the vector of integers representing this
            // specific permutation
            // https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector
            size_t seed = right.get_perm ().size();
            for(auto& i : right.get_perm ()) {
                seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }

    }; // struct hash<npancake_t>
}

#endif // _NPANCAKE_T_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
