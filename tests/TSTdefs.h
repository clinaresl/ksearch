 // -*- coding: utf-8 -*-
// TSTdefs.h
// -----------------------------------------------------------------------------
//
// Started on <sáb 07-08-2021 00:58:59.720149236 (1628290739)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Definitions
//

#ifndef _TSTDEFS_H_
#define _TSTDEFS_H_

#include <cmath>
#include <iostream>
#include <limits>
#include <tuple>
#include <utility>
#include <vector>

// predefined number of executions of each unit test
constexpr int NB_TESTS = 10'000;

// predefined maximum value
constexpr int MAX_VALUE = std::numeric_limits<int>::max ();

// predefined number of values
constexpr int MAX_VALUES = 100;

// maximum number of buckets
constexpr int MAX_NB_BUCKETS = 1'000'000;

// minimum and maximum number of characters in strings randomly generated
constexpr int MIN_NB_CHARS = 40;
constexpr int MAX_NB_CHARS = 80;

// number of discs used in the simultation of the pancake puzzle
constexpr int NB_DISCS = 20;

// maximum length of paths
constexpr int MAX_PATH_LENGTH = 100;

#endif // _TSTDEFS_H_

// Local Variables:
// mode:cpp
// fill-column:80
// End:
