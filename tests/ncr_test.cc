#include "gtest/gtest.h"
#include "ncr.hh"

#include <iostream>

class NcrTest : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

TEST_F(NcrTest, TerminationTest) {
  Ncr* n = new Ncr();
  //ASSERT_EQ(false, n);
  delete(n);
}

