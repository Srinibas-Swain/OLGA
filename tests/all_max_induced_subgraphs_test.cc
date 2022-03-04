#include "gtest/gtest.h"
#include "all_max_induced_subgraphs.hh"

#include <iostream>

class AllMaxInducedSubgraphsTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(AllMaxInducedSubgraphsTest, TerminationTest) {
	AllMaxInducedSubgraphs* amis = new AllMaxInducedSubgraphs();
	//ASSERT_EQ(false, amis);
	delete(amis);
}

