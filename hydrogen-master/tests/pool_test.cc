#include "gtest/gtest.h"

#include "pool.hh"

#include <iostream>

class PoolTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(PoolTest, TerminationTest) {
	Pool* p = new Pool();
	//ASSERT_EQ(false, p);
	delete(p);
}

