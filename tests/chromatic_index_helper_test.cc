#include "gtest/gtest.h"
#include "chromatic_index_helper.hh"

#include <iostream>

class ChromaticIndexHelperTest : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

TEST_F(ChromaticIndexHelperTest, TerminationTest) {
  ChromaticIndexHelper* cih = new ChromaticIndexHelper();
  //ASSERT_EQ(false, cih);
  delete(cih);
}

