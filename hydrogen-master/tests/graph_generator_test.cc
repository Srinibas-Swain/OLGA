#include "gtest/gtest.h"
#include "graph_generator.hh"

#include <iostream>

class GraphGeneratorTest : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

TEST_F(GraphGeneratorTest, TerminationTest) {
  GraphGenerator* gg = new GraphGenerator();
  //ASSERT_EQ(false, gg);
  delete(gg);
}

