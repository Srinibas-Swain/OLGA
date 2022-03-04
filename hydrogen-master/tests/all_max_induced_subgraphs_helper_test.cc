#include "gtest/gtest.h"
#include "all_max_induced_subgraphs_helper.hh"

#include <iostream>

class AllMaxInducedSubgraphsHelperTest : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

TEST_F(AllMaxInducedSubgraphsHelperTest, TerminationTest) {
  AllMaxInducedSubgraphsHelper* amish = new AllMaxInducedSubgraphsHelper();
  //ASSERT_EQ(false, amish);
  delete(amish);
}

