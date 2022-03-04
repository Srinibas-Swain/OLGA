#include "gtest/gtest.h"
#include "aliases.hh"

#include <iostream>

class AliasesTest : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

