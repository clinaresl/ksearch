// -*- coding: utf-8 -*-
// gtest.cc
// -----------------------------------------------------------------------------
//
// Started on <lun 07-08-2023 16:04:30.433788026 (1691417070)>
// Carlos Linares López <carlos.linares@uc3m.es>
// Ian Herman <iankherman@gmail.com>

//
// Main entry point of Google Tests
//

#include<fstream>

#include "gtest/gtest.h"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    // save the original clog and redirect its output to a logging file
    std::ofstream log_stream("libksearch_tests.log");
    std::streambuf* original_buf = std::clog.rdbuf();
    std::clog.rdbuf(log_stream.rdbuf());

    // run the selection of tests
    auto output = RUN_ALL_TESTS();

    // restore clog
    std::clog.rdbuf(original_buf);

    // and return the result
    return output;
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
