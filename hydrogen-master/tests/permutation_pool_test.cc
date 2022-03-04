#include "gtest/gtest.h"
#include "permutation_pool.hh"

#include <iostream>

class PermutationPoolTest : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

TEST_F(PermutationPoolTest, TerminationTest) {
  PermutationPool* pp = new PermutationPool();
  //ASSERT_EQ(false, pp);
  delete(pp);
}

