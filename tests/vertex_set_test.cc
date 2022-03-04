#include "gtest/gtest.h"
#include "vertex_set.hh"

#include <iostream>

class VertexSetTest : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

TEST_F(VertexSetTest, TerminationTest) {
  VertexSet* vs = new VertexSet();
  //ASSERT_EQ(false, vs);
  delete(vs);
}

