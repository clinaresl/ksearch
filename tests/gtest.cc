// -*- coding: utf-8 -*-
// gtest.cc
// -----------------------------------------------------------------------------
//
// Started on <lun 07-08-2023 16:04:30.433788026 (1691417070)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Main entry point of Google Tests
//

#include "gtest/gtest.h"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    // Run specific tests only
    // ::testing::GTEST_FLAG(filter) = "NodeFixture.*";
    // ::testing::GTEST_FLAG(filter) = "ClosedFixture.*";
    // ::testing::GTEST_FLAG(filter) = "ClassicalPerimeterFixture.*";
    // ::testing::GTEST_FLAG(filter) = "SolutionFixture*";
    // ::testing::GTEST_FLAG(filter) = "SolutionsFixture*";
    // ::testing::GTEST_FLAG(filter) = "NPuzzleFixture.*";
    // ::testing::GTEST_FLAG(filter) = "IDAFixture.*";
    // ::testing::GTEST_FLAG(filter) = "BIDAFixture.*";

    // Exclude specific tests
    // ::testing::GTEST_FLAG(filter) = "-NodeFixture.*";

    // and run the selection of tests
    return RUN_ALL_TESTS();
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
