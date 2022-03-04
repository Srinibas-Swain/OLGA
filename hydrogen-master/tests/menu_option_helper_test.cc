#include "gtest/gtest.h"
#include "menu_option_helper.hh"

#include <iostream>

class MenuOptionHelperTest : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

TEST_F(MenuOptionHelperTest, TerminationTest) {
  MenuOptionHelper* moh = new MenuOptionHelper();
  //ASSERT_EQ(false, moh);
  delete(moh);
}

