#include "gtest/gtest.h"
#include "vector_int_pool.hh"

#include <iostream>

class VectorIntPoolTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(VectorIntPoolTest, TerminationTest) {
	VectorIntPool* vip = new VectorIntPool();
	//ASSERT_EQ(false, vip);
	delete(vip);
}

