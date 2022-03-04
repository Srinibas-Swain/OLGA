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

TEST_F(CombinationGeneratorTest, ShouldSetMaxIndex) {
	int n = 5;
	//CombinationGenerator* cg = new CombinationGenerator(n);
	//ASSERT_EQ(pow(2,5), cg->max_index);
	//delete(cg);
}
