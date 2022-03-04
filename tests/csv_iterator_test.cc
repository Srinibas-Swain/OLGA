#include "gtest/gtest.h"
#include "csv_iterator.hh"

#include <iostream>

class CSVIteratorTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(CSVIteratorTest, TerminationTest) {
	CSVIterator* ci = new CSVIterator();
	//ASSERT_EQ(false, ci);
	delete(ci);
}

