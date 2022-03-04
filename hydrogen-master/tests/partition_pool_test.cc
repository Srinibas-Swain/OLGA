#include "gtest/gtest.h"
#include "partition_pool.hh"

#include <iostream>

class PartitionPoolTest : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

TEST_F(PartitionPoolTest, TerminationTest) {
  PartitionPool* pp = new PartitionPool();
  //ASSERT_EQ(false, pp);
  delete(pp);
}

