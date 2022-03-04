#include "gtest/gtest.h"
#include "blocks.hh"

#include <iostream>

class BlocksTest : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

TEST_F(BlocksTest, TerminationTest) {
  Blocks* b = new Blocks();
  //ASSERT_EQ(false, b);
  delete(b);
}

