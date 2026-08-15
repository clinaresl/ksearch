// -*- coding: utf-8 -*-
// npancake_t.h
// -----------------------------------------------------------------------------
//
// Started on <sáb 26-02-2022 18:07:39.045877735 (1645895259)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>   Ian Herman <iankherman@gmail.com>

//
// Definition of a state of the N-pancake
//

#ifndef _NPANCAKE_T_H_
#define _NPANCAKE_T_H_

#include<algorithm>
#include<cassert>
#include<filesystem>
#include<iostream>
#include<iterator>
#include<memory>
#include<sstream>
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
    // a permutation of ints in the range [1, n]
    static int _n;                                 // length of the permutation
    std::vector<int> _perm;                                      // permutation

    // this implementation acknowledges different variants:
    //
    //    + unit: the cost of every operator is the same and equal to one
    //
    //    + heavy-cost: the cost of every operator is the disc id of the disc
    //    *below* the spatula, i.e., the first disc not being transposed. This
    //    is intentionally done to ensure that both the direct and inverse
    //    application of the same operator have the same cost, and also because
    //    it is possible to use a consistent heuristic for this optimization
    //    task. The usage of this variant is inspired from Hatem, M.; and Ruml,
    //    W. 2014. Bounded suboptimal search in linear space: New results. In
    //    Proceedings of SoCS-14.
    //
    static std::string _variant;

    // increment holds the net change of the gap heuristic when flipping an
    // arbitrary number of discs in either the unit or heavy-cost variant. It is
    // used as a consistent heuristic.
    //
    // For a given disc in the head of the pancake (index #1) it computes the
    // net change in the evaluation function when been located at the position
    // that contains another disc (index #2) which is adjacent to a third disc
    // (index #3). Note that the third index might be the table
    static std::vector<std::vector<std::vector<int> > > _increment;

    // methods

    // flip the first k positions of this permutation
    std::vector<int> _flip(int k) const {

        std::vector<int> perm = _perm;
        for (auto i=0 ; i<=k/2 ; i++) {
            std::swap (perm [i], perm [k-i]);
        }
        return perm;
    }

    // return the gap heuristic to get from the given permutation to the
    // identity permutation under the current variant. If a second argument is
    // given, then all discs with an id less or equal than it are ignored.
    int _gap (const std::vector<int>& perm, int ignore = 0) const {

        int h=0;                // heuristic value computed as the gap function
        for (auto i=0 ; i <_n -1 ; i++) {      // for all locs but the last one
            if (_variant == "unit") {

                if (perm[i] > ignore and perm[i+1] > ignore) {

                    // in the unit variant, just count the number of gaps.
                    h += (int) (abs ((perm   [i]) -
                                     (perm [i+1])) != 1);
                }
            } else if (_variant == "heavy-cost") {

                if (perm[i] > ignore and perm[i+1] > ignore) {

                    // in the heavy-cost variant, weight each gap with the minimum
                    // radius between the discs involved in the gap. Taking the
                    // minimum is mandatory to enforce consistency
                    h += ((int) (abs ((perm   [i]) -
                                      (perm [i+1])) != 1)) * std::min (perm[i], perm[1+i]);
                }
            } else {
                throw std::invalid_argument ("[npancake_t::h] Unknown variant!");
            }
        }

        // Additionally, sum one gap in case the last position is not N-1
        if (perm[_n - 1] > ignore and perm [_n - 1] != _n) {
            if (_variant == "unit") {
                h += 1;
            } else if (_variant == "heavy-cost") {

                // The last disc is weighted with the size of the last disc
                // because it is necessarily lower than _n+1
                h += perm[_n-1];
            }
        }

        // and return the heuristic value
        return h;
    }

public:

    // Default constructors are forbidden by default
    npancake_t () = delete;

    // A permutation can be constructed from a vector of integers. This
    // constructor assumes that all integers are distinct and belong to the
    // range [1, n]
    npancake_t (std::vector<int> perm) :
        _perm       { perm }
        {
            // store the size of the permuation
            _n = perm.size ();
        }

    // And also with an initializer list
    npancake_t (std::initializer_list<int> perm) :
        _perm       { perm }
        {
            // store the size of the permuation
            _n = perm.size ();
        }

    // getters
    static int get_n () {
        return _n;
    }
    const std::vector<int>& get_perm () const {
        return _perm;
    }
    static const std::string& get_variant () {
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
    int operator[](int i) const {
        return _perm[i];
    }

    friend std::ostream& operator<<(std::ostream& stream, const npancake_t& right) {

        // capture the permutation
        auto perm = right.get_perm ();

        // Show the permutation indicating the length of the side
        std::stringstream ss;
        for (auto i = 0 ; i < int (perm.size ()) ; i++) {
            if (i==0) {
                ss << perm[i];
            } else {
                ss << " " << perm[i];
            }
        }

        stream << ss.str ();
        return stream;
    }

    // methods

    // INIT pre-computes the incremental evaluation function table for the gap
    // heuristic.
    //
    // For a given disc in the head of the pancake (index #1) it computes the
    // net change in the evaluation function when been located at the position
    // that contains another disc (index #2) which is adjacent to a third disc
    // (index #3). Note that the third index might be the table:
    //
    //    Unit variant: all operators have the same cost, 1
    //
    //    Heavy-cost variant: the prefix reversal #1-#2 has a cost which is
    //                        equal to the id of the pancake at #3. Using the
    //                        pancake id at #3 guarantees that both operators
    //                        (fwd and bwd) have the same cost.
    //
    // This function initializes also the static data member _variant which is,
    // by default, unit
    static void init (const std::string& variant = "unit") {

        // copy the domain variant used for initializing the increment table
        npancake_t::_variant = variant;

        // reserve space enough for holding the increment table. The first and
        // second discs might be any in the range [1, _n] whereas the last one
        // should include the table so that it takes values in the range [1, _n+1]
        npancake_t::_increment = std::vector<std::vector<std::vector<int> > >(_n,
                                                                              std::vector<std::vector<int> > (_n,
                                                                                                              std::vector<int>(1+_n,
                                                                                                                               0)));
        // next process all combinations for the head (disc #1), tail (disc #2)
        // and next (disc #3). Note that while head and tail iterate over all
        // discs, next gets to n+1, the table
        for (int head = 1 ; head <= _n ; head++) {
            for (int tail = 1 ; tail <= _n ; tail++) {
                if (head != tail) {
                    for (int next = 1 ; next <= _n+1 ; next++) {
                        if (head != next && tail != next) {

                            // compute the net change in the value of the gap
                            // heuristic when the head is moved to the tail,
                            // which is adjacent to next (and next might be
                            // equal to the size of the permutation representing
                            // the table)

                            // gap1 checks whether there is a gap between tail
                            // and next. gap2 checks whether there is a gap
                            // between head and next
                            bool gap1 = (abs (tail - next) != 1);
                            bool gap2 = (abs (head - next) != 1);

                            // and now annotate the net change of the gap
                            // heuristic for this combination which can be
                            // either zero, -1 or +1, or an arbitrary amount in
                            // the heavy-cost variant.
                            if (variant == "unit") {

                                // in the case of the unit variant, just simply
                                // take the differences in the number of gaps:
                                // those created in the child minus those
                                // created inthe parent
                                _increment[head-1][tail-1][next-1] = int (gap2) - int (gap1);
                            } else if (variant == "heavy-cost") {

                                // in the heavy-cost variant, weight the same
                                // difference computed in the unit variant with
                                // the minimum radius of the discs involved in
                                // the gap
                                _increment[head-1][tail-1][next-1] = std::min (head, next) * int (gap2) -
                                    std::min (tail, next) * int (gap1);
                            } else {
                                throw std::invalid_argument ("[npancake_t::init] Unknown variant!");
                            }
                        }
                    }
                }
            }
        }
    }

    // process each child separately through the use of a callable that has to
    // receive exactly three arguments: cost_t g, cost_t h and the successor
    // state. It is the responsibility of the caller to provide the right
    // callable at the calling site.
    template<typename F>
    requires std::invocable<F&, int, int, npancake_t&&>
    void children (int h, npancake_t const& goal, F&& callable) const {
        
        for (auto i=1; i < _n; i++) {                         // for all locations

            // flip the first 1+i discs of the permutation
            npancake_t successor (_flip (i));
            
            // create the successor

            // retrieve the net change in the computation of the gap
            // heuristic. There are two cases, either we are flipping the
            // entire permutation in which case the last position has to be
            // taken directly, or we are changing a prefix of it, in which
            // case, the content of the last position is considered. The
            // indices used to access the increment table take into account
            // that we are using ids in the range [1, _n]
            int delta_h = (i == _n-1)
                ? _increment[_perm[0]-1][_perm[i]-1][_n]
                : _increment[_perm[0]-1][_perm[i]-1][_perm[i+1]-1];

            // The net change in the gap heuristic is retrieved from the
            // _increment lookup table. In the unit variant, the cost of
            // every operator is always equal to 1, whereas in the heavy
            // cost variant, the cost of the operator is the radius of the
            // disc immediately below the spatula, i.e., the first one not
            // being transposed.
            callable ((_variant == "unit") ? 1 : ((i==_n-1) ? _n+1 : _perm[1+i]),
                      h + delta_h,
                      std::move (successor));
        }
    }
    
    // return the heuristic estimate to get from this permutation to the
    // identity permutation, i.e., the given goal is ignored in all cases
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
