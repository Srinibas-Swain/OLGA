#include "gtest/gtest.h"
#include "find_connected_graphs_omp.hh"

#include <iostream>

class FindConnectedGraphsOmpTest : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

TEST_F(FindConnectedGraphsOmpTest, TerminationTest) {
  FindConnectedGraphsOmp* fcgo = new FindConnectedGraphsOmp();
  //ASSERT_EQ(false, fcgo);
  delete(fcgo);
}

