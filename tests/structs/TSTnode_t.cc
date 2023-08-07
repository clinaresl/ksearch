// -*- coding: utf-8 -*-
// TSTnode_t.cc
// -----------------------------------------------------------------------------
//
// Started on <lun 07-08-2023 16:35:56.825833552 (1691418956)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Unit tests for the node_t class
//

#include<string>
#include<vector>

#include "../fixtures/TSTnodefixture.h"

using namespace std;

// Checks the creation of int nodes with default values
// ----------------------------------------------------------------------------
TEST_F (NodeFixture, DefaultValuesInt) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds an array of integers randomly
        // generated
        auto values = randVectorInt (MAX_VALUES, MAX_VALUES, true);
        khs::node_t<vector<int>> n {values};

        // next, verify that all attributes of this node take the default values
        ASSERT_EQ (n.get_g (), 0);
        ASSERT_EQ (n.get_h (), 0);
        ASSERT_EQ (n.get_f (), 0);

        // make sure also the stable "pointers" in closed are initialized to
        // an impossible value
        ASSERT_EQ (n.get_pointer (), string::npos);
    }
}

// Checks the creation of string nodes with default values
// ----------------------------------------------------------------------------
TEST_F (NodeFixture, DefaultValuesString) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds a string randomly generated
        auto value = randString (MAX_VALUES);
        khs::node_t<string> n {value};

        // next, verify that all attributes of this node take the default values
        ASSERT_EQ (n.get_g (), 0);
        ASSERT_EQ (n.get_h (), 0);
        ASSERT_EQ (n.get_f (), 0);

        // make sure also the stable "pointers" in closed are initialized to an
        // impossible value
        ASSERT_EQ (n.get_pointer (), string::npos);
    }
}

// Checks that int nodes preserve the values of of its type
// ----------------------------------------------------------------------------
TEST_F (NodeFixture, SetValuesInt) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds an array of integers randomly
        // generated
        auto values = randVectorInt (MAX_VALUES, MAX_VALUES, true);
        khs::node_t<vector<int>> n {values};

        // verify next that the state of this node is precisely the one given
        ASSERT_EQ (n.get_state (), values);
    }
}

// Checks that string nodes preserve the values of of its type
// ----------------------------------------------------------------------------
TEST_F (NodeFixture, SetValuesString) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds a string randomly generated
        auto value = randString (MAX_VALUES);
        khs::node_t<string> n {value};

        // verify next that the state of this node is precisely the one given
        ASSERT_EQ (n.get_state (), value);
    }
}

// Checks that string nodes correctly implement equality
// ----------------------------------------------------------------------------
TEST_F (NodeFixture, EqualString) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds a string randomly generated
        auto value = randString (MAX_VALUES);
        khs::node_t<string> n {value};

        // create another node using two different methods: copy constructor and
        // assignment
        auto n1 {n};
        auto n2 = n;

        // verify that these states are all equal
        ASSERT_EQ (n, n1);
        ASSERT_EQ (n, n2);
        ASSERT_EQ (n1, n2);
    }
}

// Checks that string nodes correctly implement not-equal
// ----------------------------------------------------------------------------
TEST_F (NodeFixture, NotEqualString) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds a string randomly generated
        auto value = randString (MAX_VALUES);
        khs::node_t<string> n {value};

        // randomly choose a location and change its content. A while statement
        // is used because we ensure that the previous string is effectively
        // modified
        int idx = rand () % value.size ();
        char prev = value[idx];
        char last = (char) (rand () % 95 + 32);
        while (last == prev) {
            last = (char) (rand () % 95 + 32);
        }
        value [idx] = last;

        // create another node with this new value and verify now they are not
        // equal
        auto n1 {value};
        ASSERT_NE (n, n1);
    }
}

// Checks that string nodes correctly implement not-equal
// ----------------------------------------------------------------------------
TEST_F (NodeFixture, LessThanString) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds a string randomly generated
        auto value = randString (MAX_VALUES);
        khs::node_t<string> n {value};

        // randomly choose a location and change its content. A while statement
        // is used because we ensure that the previous string is effectively
        // modified
        auto backup = value;
        int idx = rand () % backup.size ();
        char prev = backup[idx];
        char last = (char) (rand () % 95 + 32);
        while (last == prev) {
            last = (char) (rand () % 95 + 32);
        }
        backup [idx] = last;

        // create another node with this new value and verify that they are
        // sorted correctly
        khs::node_t n1 {backup};
        ASSERT_TRUE ( (value < backup && n < n1) ||
                      (value > backup && n1 < n) );
    }
}

// Checks that setting the g-value of an int node produces the expected result
// ----------------------------------------------------------------------------
TEST_F (NodeFixture, SetgInt) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds an array of integers randomly
        // generated
        auto values = randVectorInt (MAX_VALUES, MAX_VALUES, true);
        khs::node_t<vector<int>> n {values};

        // set the g-value of this node to any value
        int val = rand () % MAX_VALUE;
        n.set_g (val);

        // make sure the g-value is the expected one and that its f-value is
        // also automatically updated
        ASSERT_EQ (n.get_g (), val);
        ASSERT_EQ (n.get_f (), val);

        // in passing, update the h-value and verify that it is also correctly
        // updated along with the new f-value
        val = rand () % MAX_VALUE;
        n.set_h (val);
        ASSERT_EQ (n.get_h (), val);
        ASSERT_EQ (n.get_f (), n.get_g () + n.get_h ());
    }
}

// Checks that setting the g-value of a string node produces the expected result
// ----------------------------------------------------------------------------
TEST_F (NodeFixture, SetgString) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds a string randomly generated
        auto value = randString (MAX_VALUES);
        khs::node_t<string> n {value};

        // set the g-value of this node to any value
        int val = rand () % MAX_VALUE;
        n.set_g (val);

        // make sure the g-value is the expected one and that its f-value is
        // also automatically updated
        ASSERT_EQ (n.get_g (), val);
        ASSERT_EQ (n.get_f (), val);

        // in passing, update the h-value and verify that it is also correctly
        // updated along with the new f-value
        val = rand () % MAX_VALUE;
        n.set_h (val);
        ASSERT_EQ (n.get_h (), val);
        ASSERT_EQ (n.get_f (), n.get_g () + n.get_h ());
    }
}

// Checks that setting the h-value of an int node produces the expected result
// ----------------------------------------------------------------------------
TEST_F (NodeFixture, SethInt) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds an array of integers randomly
        // generated
        auto values = randVectorInt (MAX_VALUES, MAX_VALUES, true);
        khs::node_t<vector<int>> n {values};

        // set the h-value of this node to any value
        int val = rand () % MAX_VALUE;
        n.set_h (val);

        // make sure the h-value is the expected one and that its f-value is
        // also automatically updated
        ASSERT_EQ (n.get_h (), val);
        ASSERT_EQ (n.get_f (), val);

        // in passing, update the g-value and verify that it is also correctly
        // updated along with the new f-value
        val = rand () % MAX_VALUE;
        n.set_g (val);
        ASSERT_EQ (n.get_g (), val);
        ASSERT_EQ (n.get_f (), n.get_g () + n.get_h ());
    }
}

// Checks that setting the h-value of a string node produces the expected result
// ----------------------------------------------------------------------------
TEST_F (NodeFixture, SethString) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds a string randomly generated
        auto value = randString (MAX_VALUES);
        khs::node_t<string> n {value};

        // set the h-value of this node to any value
        int val = rand () % MAX_VALUE;
        n.set_h (val);

        // make sure the h-value is the expected one and that its f-value is
        // also automatically updated
        ASSERT_EQ (n.get_h (), val);
        ASSERT_EQ (n.get_f (), val);

        // in passing, update the g-value and verify that it is also correctly
        // updated along with the new f-value
        val = rand () % MAX_VALUE;
        n.set_g (val);
        ASSERT_EQ (n.get_g (), val);
        ASSERT_EQ (n.get_f (), n.get_g () + n.get_h ());
    }
}

// Local Variables:
// mode:cpp
// fill-column:80
// End:
