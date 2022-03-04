#include "gtest/gtest.h"
#include "graph_utils.hh"

#include <iostream>

class GraphUtilsTest : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

TEST_F(GraphUtilsTest, TerminationTest) {
  GraphUtils* gu = new GraphUtils();
  //ASSERT_EQ(false, gu);
  delete(gu);
}

