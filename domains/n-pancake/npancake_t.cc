// -*- coding: utf-8 -*-
// npancake_t.cc
// -----------------------------------------------------------------------------
//
// Started on <sáb 26-02-2022 18:10:41.081648137 (1645895441)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>   Ian Herman <iankherman@gmail.com>

//
// Definition of a state of the N-pancake
//

#include "npancake_t.h"

// Static vars
int npancake_t::_n = 0;
std::string npancake_t::_variant = "unit";
std::vector<std::vector<std::vector<int>>> npancake_t::_increment;

// return the heuristic estimate to get from this permutation to the identity
// permutation, i.e., the given goal is ignored in all cases
int npancake_t::h (const npancake_t& goal) const {

    return _gap (_perm);
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
