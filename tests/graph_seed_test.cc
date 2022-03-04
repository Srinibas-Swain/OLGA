#include "gtest/gtest.h"
#include "graph_seed.hh"

#include <iostream>

class GraphSeedTest : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

TEST_F(GraphSeedTest, TerminationTest) {
  GraphSeed* gs = new GraphSeed();
  //ASSERT_EQ(false, gs);
  delete(gs);
}

