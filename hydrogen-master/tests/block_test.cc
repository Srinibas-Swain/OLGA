

#include "gtest/gtest.h"
#include "block.hh"

class BlockTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

// TEST_F(BlockTest, AddRangeWithGap1) {
// 	int n = 5;
// 	Block1 *block = new Block1(0, n, 1);
// 	ASSERT_EQ(n, block->size());
// 	for(int ix=0; ix<n; ix++) {
// 		EXPECT_EQ(ix, block->at(ix));
// 	}
// 	delete(block);
// }

// TEST_F(BlockTest, AddRangeWithGap2) {
// 	int n_start = 1;
// 	int n_end = 9;
// 	int gap = 2;
// 	Block1 *block = new Block1(n_start, n_end, gap);
// 	int size_expected = (n_end-n_start+1)/gap;
// 	ASSERT_EQ(size_expected, block->size());
// 	for(int ix=0; ix<size_expected; ix++) {
// 		EXPECT_EQ(ix*gap+n_start, block->at(ix));
// 	}
// 	delete(block);
// }



