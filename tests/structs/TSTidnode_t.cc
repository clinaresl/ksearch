// -*- coding: utf-8 -*-
// TSTidnode_t.cc
// -----------------------------------------------------------------------------
//
// Started on <lun 02-06-2025 13:37:37.913915820 (1748864257)>
// Carlos Linares López <carlos.linares@uc3m.es>
//

//
// Unit tests of the idnode_t class
//

#include "../fixtures/TSTidnodefixture.h"

// Checks the creation of int nodes with default values
// ----------------------------------------------------------------------------
TEST_F (IdNodeFixture, DefaultValuesInt) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds an array of integers randomly
        // generated
        auto values = randVectorInt (MAX_VALUES, MAX_VALUES, true);
        khs::idnode_t<std::vector<int>> n {values};

        // next, verify that all attributes of this node take the default values
        ASSERT_EQ (n.get_g (), 0);
        ASSERT_EQ (n.get_h (), 0);
        ASSERT_EQ (n.get_f (), 0);
        ASSERT_TRUE (n.get_unwound ());

        // make sure also the stable "pointers" in closed are initialized to
        // an impossible value
        ASSERT_EQ (n.get_pointer (), std::string::npos);
        ASSERT_EQ (n.get_backpointers ().size (), 0);

        // also, ensure there are neither backward g-values nor pids in this
        // node
        ASSERT_EQ (n.get_gb ().size (), 0);
        ASSERT_EQ (n.get_pids ().size (), 0);
    }
}

// Checks the creation of string nodes with default values
// ----------------------------------------------------------------------------
TEST_F (IdNodeFixture, DefaultValuesString) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds a string randomly generated
        auto value = randString (MAX_VALUES);
        khs::idnode_t<std::string> n {value};

        // next, verify that all attributes of this node take the default values
        ASSERT_EQ (n.get_g (), 0);
        ASSERT_EQ (n.get_h (), 0);
        ASSERT_EQ (n.get_f (), 0);
        ASSERT_TRUE (n.get_unwound ());

        // make sure also the stable "pointers" in closed are initialized to
        // an impossible value
        ASSERT_EQ (n.get_pointer (), std::string::npos);
        ASSERT_EQ (n.get_backpointers ().size (), 0);

        // also, ensure there are neither backward g-values nor pids in this
        // node
        ASSERT_EQ (n.get_gb ().size (), 0);
        ASSERT_EQ (n.get_pids ().size (), 0);
    }
}

// Checks that int nodes preserve the values of of its type
// ----------------------------------------------------------------------------
TEST_F (IdNodeFixture, SetValuesInt) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds an array of integers randomly
        // generated
        auto values = randVectorInt (MAX_VALUES, MAX_VALUES, true);
        khs::idnode_t<std::vector<int>> n {values};

        // verify next that the state of this node is precisely the one given
        ASSERT_EQ (n.get_state (), values);
    }
}

// Checks that string nodes preserve the values of of its type
// ----------------------------------------------------------------------------
TEST_F (IdNodeFixture, SetValuesString) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds a string randomly generated
        auto value = randString (MAX_VALUES);
        khs::idnode_t<std::string> n {value};

        // verify next that the state of this node is precisely the one given
        ASSERT_EQ (n.get_state (), value);
    }
}

// Checks that string nodes correctly implement equality
// ----------------------------------------------------------------------------
TEST_F (IdNodeFixture, EqualString) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds a string randomly generated
        auto value = randString (MAX_VALUES);
        khs::idnode_t<std::string> n {value};

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
TEST_F (IdNodeFixture, NotEqualString) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds a string randomly generated
        auto value = randString (MAX_VALUES);
        khs::idnode_t<std::string> n {value};

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

        auto n2 = value;
        ASSERT_NE (n, n2);
    }
}

// Checks that string nodes correctly implement less than
// ----------------------------------------------------------------------------
TEST_F (IdNodeFixture, LessThanString) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds a string randomly generated
        auto value = randString (MAX_VALUES);
        khs::idnode_t<std::string> n {value};

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
        khs::idnode_t n1 {backup};
        ASSERT_TRUE ( (value < backup && n < n1) ||
                      (value > backup && n1 < n) );
    }
}

// Checks that setting the g-value of an int node produces the expected result
// ----------------------------------------------------------------------------
TEST_F (IdNodeFixture, SetgInt) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds an array of integers randomly
        // generated
        auto values = randVectorInt (MAX_VALUES, MAX_VALUES, true);
        khs::idnode_t<std::vector<int>> n {values};

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
TEST_F (IdNodeFixture, SetgString) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds a string randomly generated
        auto value = randString (MAX_VALUES);
        khs::idnode_t<std::string> n {value};

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
TEST_F (IdNodeFixture, SethInt) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds an array of integers randomly
        // generated
        auto values = randVectorInt (MAX_VALUES, MAX_VALUES, true);
        khs::idnode_t<std::vector<int>> n {values};

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
TEST_F (IdNodeFixture, SethString) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds a string randomly generated
        auto value = randString (MAX_VALUES);
        khs::idnode_t<std::string> n {value};

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

// Checks that an arbitrary number of backpointers can be set on int nodes
// ----------------------------------------------------------------------------
TEST_F (IdNodeFixture, SetBackPointersInt) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds an array of integers randomly
        // generated
        auto values = randVectorInt (MAX_VALUES, MAX_VALUES, true);
        khs::idnode_t<std::vector<int>> n {values};

        // randomly determine the number of backpointers to set ---to be at
        // least one
        auto bps = randVectorLabeledBackpointer(rand () % MAX_VALUES + 1);

        // set the backpointers one by one
        for (auto bp : bps) {
            n += bp;
        }

        // and verify that all of them have been properly set
        ASSERT_EQ (n.get_backpointers ().size (), bps.size ());

        // now, also verify that both vectors contain precisely the same items
        auto nbps = n.get_backpointers ();
        for (auto i = 0 ; i < bps.size () ; i++) {
            ASSERT_EQ (bps [i], nbps [i]);
        }
    }
}

// Checks that an arbitrary number of backpointers can be set on string nodes
// ----------------------------------------------------------------------------
TEST_F (IdNodeFixture, SetBackPointersString) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds a string randomly generated
        auto value = randString (MAX_VALUES);
        khs::idnode_t<std::string> n {value};

        // randomly determine the number of backpointers to set ---to be at
        // least one
        auto bps = randVectorLabeledBackpointer(rand () % MAX_VALUES + 1);

        // set the backpointers one by one
        for (auto bp : bps) {
            n += bp;
        }

        // and verify that all of them have been properly set
        ASSERT_EQ (n.get_backpointers ().size (), bps.size ());

        // now, also verify that both vectors contain precisely the same items
        auto nbps = n.get_backpointers ();
        for (auto i = 0 ; i < bps.size () ; i++) {
            ASSERT_EQ (bps [i], nbps [i]);
        }
    }
}

// Checks that an arbitrary number of backward g-values can be set on int nodes
// ----------------------------------------------------------------------------
TEST_F (IdNodeFixture, SetgBackwardInt) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds an array of integers randomly
        // generated
        auto values = randVectorInt (MAX_VALUES, MAX_VALUES, true);
        khs::idnode_t<std::vector<int>> n {values};

        // randomly determine the number of backward g-values to set ---to be at
        // least one. Make'em all different even if that does not mind, but the
        // true scenario will be this
        auto gbs = randVectorInt (rand () % MAX_VALUES + 1, MAX_VALUE, true);

        // set the backward g-values one by one
        for (auto gb : gbs) {
            n += gb;
        }

        // and verify that all of them have been properly set
        ASSERT_EQ (n.get_gb ().size (), gbs.size ());

        // now, also verify that both vectors contain precisely the same items
        auto ngbs = n.get_gb ();
        for (auto i = 0 ; i < gbs.size () ; i++) {
            ASSERT_EQ (gbs [i], ngbs [i]);
        }
    }
}

// Checks that an arbitrary number of backward g-values can be set on string nodes
// ----------------------------------------------------------------------------
TEST_F (IdNodeFixture, SetgBackwardString) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds a string randomly generated
        auto value = randString (MAX_VALUES);
        khs::idnode_t<std::string> n {value};

        // randomly determine the number of backward g-values to set ---to be at
        // least one. Make'em all different even if that does not mind, but the
        // true scenario will be this
        auto gbs = randVectorInt (rand () % MAX_VALUES + 1, MAX_VALUE, true);

        // set the backward g-values one by one
        for (auto gb : gbs) {
            n += gb;
        }

        // and verify that all of them have been properly set
        ASSERT_EQ (n.get_gb ().size (), gbs.size ());

        // now, also verify that both vectors contain precisely the same items
        auto ngbs = n.get_gb ();
        for (auto i = 0 ; i < gbs.size () ; i++) {
            ASSERT_EQ (gbs [i], ngbs [i]);
        }
    }
}

// Checks that gb-values are correctly found in int nodes
// ----------------------------------------------------------------------------
TEST_F (IdNodeFixture, FindgBackwardInt) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds an array of integers randomly
        // generated
        auto values = randVectorInt (MAX_VALUES, MAX_VALUES, true);
        khs::idnode_t<std::vector<int>> n {values};

        // randomly determine the number of backward g-values to set ---even
        // zero. Make'em all different even if that does not mind, but the true
        // scenario will be this
        auto gbs = randVectorInt (rand () % MAX_VALUES, MAX_VALUE, true);

        // set the backward g-values one by one
        for (auto gb: gbs) {
            n += gb;
        }

        // next, search for all gb-values in gbs
        for (auto gb : gbs) {
            ASSERT_TRUE (n.find_gb (gb));
        }
    }
}

// Checks that gb-values are correctly *not* found in int nodes
// ----------------------------------------------------------------------------
TEST_F (IdNodeFixture, FindNotgBackwardInt) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds an array of integers randomly
        // generated
        auto values = randVectorInt (MAX_VALUES, MAX_VALUES, true);
        khs::idnode_t<std::vector<int>> n {values};

        // randomly determine MAX_VALUES g-values to set. Make'em all different
        // so that inserting the first half will ensure that no item in the
        // second half can be found in the list of gb-values
        auto gbs = randVectorInt (MAX_VALUES, MAX_VALUE, true);

        // set the first half of backward g-values
        for (auto i = 0 ; i < MAX_VALUES / 2 ; i++) {
            n += gbs[i];
        }

        // next, search for all gb-values in gbs
        for (auto i = 0 ; i < MAX_VALUES / 2 ; i++) {

            // elements in the first half must be found, whereas elements in the
            // second half should not be present in the list of gb-values
            if (i < MAX_VALUES / 2) {
                ASSERT_TRUE (n.find_gb (gbs[i]));
            } else {
                ASSERT_FALSE (n.find_gb (gbs[i]));
            }
        }
    }
}

// Checks that gb-values are correctly found in string nodes
// ----------------------------------------------------------------------------
TEST_F (IdNodeFixture, FindgBackwardString) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds a string randomly generated
        auto value = randString (MAX_VALUES);
        khs::idnode_t<std::string> n {value};

        // randomly determine the number of backward g-values to set ---even
        // zero. Make'em all different even if that does not mind, but the true
        // scenario will be this
        auto gbs = randVectorInt (rand () % MAX_VALUES, MAX_VALUE, true);

        // set the backward g-values one by one
        for (auto gb: gbs) {
            n += gb;
        }

        // next, search for all gb-values in gbs
        for (auto gb : gbs) {
            ASSERT_TRUE (n.find_gb (gb));
        }
    }
}

// Checks that gb-values are correctly *not* found in string nodes
// ----------------------------------------------------------------------------
TEST_F (IdNodeFixture, FindNotgBackwardString) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds a string randomly generated
        auto value = randString (MAX_VALUES);
        khs::idnode_t<std::string> n {value};

        // randomly determine MAX_VALUES g-values to set. Make'em all different
        // so that inserting the first half will ensure that no item in the
        // second half can be found in the list of gb-values
        auto gbs = randVectorInt (MAX_VALUES, MAX_VALUE, true);

        // set the first half of backward g-values
        for (auto i = 0 ; i < MAX_VALUES / 2 ; i++) {
            n += gbs[i];
        }

        // next, search for all gb-values in gbs
        for (auto i = 0 ; i < MAX_VALUES / 2 ; i++) {

            // elements in the first half must be found, whereas elements in the
            // second half should not be present in the list of gb-values
            if (i < MAX_VALUES / 2) {
                ASSERT_TRUE (n.find_gb (gbs[i]));
            } else {
                ASSERT_FALSE (n.find_gb (gbs[i]));
            }
        }
    }
}

// Check that the unwound flag is properly set in int nodes and can be read
// ----------------------------------------------------------------------------
TEST_F (IdNodeFixture, SetUnwoundInt) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds an array of integers randomly
        // generated
        auto values = randVectorInt (MAX_VALUES, MAX_VALUES, true);
        khs::idnode_t<std::vector<int>> n {values};

        // set the unwound flag of this node to any value
        bool val = bool (rand () % 2);
        n.set_unwound (val);

        // verify the unwould value has been properly set
        ASSERT_EQ (n.get_unwound (), val);
    }
}

// Check that the unwound flag is properly set in string nodes and can be read
// ----------------------------------------------------------------------------
TEST_F (IdNodeFixture, SetUnwoundString) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds a string randomly generated
        auto value = randString (MAX_VALUES);
        khs::idnode_t<std::string> n {value};

        // set the unwound flag of this node to any value
        bool val = bool (rand () % 2);
        n.set_unwound (val);

        // verify the unwould value has been properly set
        ASSERT_EQ (n.get_unwound (), val);
    }
}

// Checks that an arbitrary number of path ids can be set on int nodes
// ----------------------------------------------------------------------------
TEST_F (IdNodeFixture, SetPathIdsInt) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds an array of integers randomly
        // generated
        auto values = randVectorInt (MAX_VALUES, MAX_VALUES, true);
        khs::idnode_t<std::vector<int>> n {values};

        // randomly choose a number of path ids
        auto pid = randVectorInt (MAX_VALUES, MAX_VALUES, true);

        // set the path ids one by one
        for (auto ipid : pid) {
            n.add_pid (ipid);
        }

        // and verify that all of them have been properly set
        ASSERT_EQ (n.get_pids ().size (), pid.size ());

        // now, also verify that both vectors contain precisely the same items
        auto pids = n.get_pids ();
        for (auto i = 0 ; i < pids.size () ; i++) {
            ASSERT_EQ (pid [i], pids [i]);
        }
    }
}

// Checks that an arbitrary number of path ids can be set on string nodes
// ----------------------------------------------------------------------------
TEST_F (IdNodeFixture, SetPathIdsString) {

    for (auto i = 0 ; i < NB_TESTS ; i++) {

        // create a node which just holds a string randomly generated
        auto value = randString (MAX_VALUES);
        khs::idnode_t<std::string> n {value};

        // randomly choose a number of path ids
        auto pid = randVectorInt (MAX_VALUES, MAX_VALUES, true);

        // set the path ids one by one
        for (auto ipid : pid) {
            n.add_pid (ipid);
        }

        // and verify that all of them have been properly set
        ASSERT_EQ (n.get_pids ().size (), pid.size ());

        // now, also verify that both vectors contain precisely the same items
        auto pids = n.get_pids ();
        for (auto i = 0 ; i < pids.size () ; i++) {
            ASSERT_EQ (pid [i], pids [i]);
        }
    }
}


// Local Variables:
// mode:cpp
// fill-column:80
// End:
