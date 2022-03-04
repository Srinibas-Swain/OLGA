#include "gtest/gtest.h"
#include "v_pool.hh"

#include <iostream>

class VPoolTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

class VPool_Element {
public:
	int _id;
};

TEST_F(VPoolTest, TerminationTest) {
	std::function<VPool_Element*()> func = [&] () { return new VPool_Element();  };
	VPool<VPool_Element* > vp;
}

