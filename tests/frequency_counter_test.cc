#include "gtest/gtest.h"
#include "frequency_counter.hh"

#include <iostream>

class FrequencyCounterTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(FrequencyCounterTest, PresentTest) {
	FrequencyCounter<int> fc;
	ASSERT_FALSE(fc.present(5));
}

TEST_F(FrequencyCounterTest, UpdateTest) {
	FrequencyCounter<int> fc;
	fc.update(5);
	ASSERT_TRUE(fc.present(5));
	ASSERT_FALSE(fc.present(6));
}

TEST_F(FrequencyCounterTest, GetTest) {
	FrequencyCounter<int> fc;
	fc.update(5);
	fc.update(6);
	fc.update(5);
	
	ASSERT_EQ(2, fc.get(5));
	ASSERT_EQ(1, fc.get(6));
	ASSERT_EQ(0, fc.get(7));
}

TEST_F(FrequencyCounterTest, ClearTest) {
	FrequencyCounter<int> fc;
	fc.update(5);
	fc.clear();
	ASSERT_EQ(0, fc.get(5));
	ASSERT_EQ(0, fc.get(7));
}

TEST_F(FrequencyCounterTest, MaxTest) {
	FrequencyCounter<int> fc;
	fc.update(5);
	fc.update(6);
	fc.update(5);
	fc.update(7);
	fc.update(7);
	bool result;
	for(int imax : fc.max())
		ASSERT_TRUE(imax==5 || imax==7);
}

