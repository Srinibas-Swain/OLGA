#include "gtest/gtest.h"
#include "combination_generator.hh"

class CombinationGeneratorTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(CombinationGeneratorTest, AddRangeWithGap1) {
	int n = 5;
	CombinationGenerator* cg = new CombinationGenerator(n);
	delete(cg);
}
