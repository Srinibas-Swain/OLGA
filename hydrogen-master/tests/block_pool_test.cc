#include "gtest/gtest.h"
#include "block_pool.hh"

#include <iostream>

class BlockPoolTest : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

TEST_F(BlockPoolTest, TerminationTest) {
  BlockPool* bp = new BlockPool();
  //ASSERT_EQ(false, bp);
  delete(bp);
}

