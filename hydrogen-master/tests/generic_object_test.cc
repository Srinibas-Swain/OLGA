#include "gtest/gtest.h"
#include "generic_object.hh"

#include <iostream>

class GenericObjectTest : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

// TEST_F(GenericObjectTest, TerminationTest) {
//   GenericObject* go = new GenericObject();
//   //ASSERT_EQ(false, go);
//   delete(go);
// }

