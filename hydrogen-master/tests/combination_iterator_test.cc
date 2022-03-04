#include "gtest/gtest.h"
#include "combination_iterator.hh"

#include <cmath>
#include <iostream>

class CombinationIteratorTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(CombinationIteratorTest, TerminationTest) {
	int n = 5;
	CombinationIterator* ci = new CombinationIterator(n, std::pow(2,n));
	ASSERT_EQ(false, ci->is_done());
	bool result = ci->next();
	ASSERT_EQ(false, result);
	ASSERT_EQ(true, ci->is_done());
	delete(ci);
}

TEST_F(CombinationIteratorTest, NumberOfIterationsTest) {
	int n = 10;
	CombinationIterator* ci = new CombinationIterator(n, 0);
	int count=0;
	while(ci->next()) {
		//std::cout << count << std::endl;
		++count;
	}
	ASSERT_EQ(std::pow(2,n), count);
	delete(ci);
}

TEST_F(CombinationIteratorTest, DataSanityTest) {
	int n = 12;
	CombinationIterator* ci = new CombinationIterator(n, 0);
	int count=100;
	int count_tmp=count;
	while((count_tmp>=0)) {
		ci->next();
		count_tmp--;
	}

	bool remainder;
	const bool* data = ci->get_elements();
	vector<int> expected;
	// todo: print the complete data in case of mismatch

	// checking the boolean array
	for(int ix=0; ix<n; ix++) {
		remainder = (count % 2)==1 ? true: false;
		if(remainder) expected.push_back(ix);
		ASSERT_EQ(remainder, data[ix]);
		count /= 2;
	}

	// checking the set of valid vertices present in the vectorfixed data
	int ix_element=0;
	for(auto element : *ci->get_current_vertices()) {
		ASSERT_EQ(expected[ix_element++], element);
	}
	delete(ci);
}

