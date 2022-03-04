#include "gtest/gtest.h"
#include "csv_row.hh"

#include <iostream>

class CSVRowTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(CSVRowTest, TerminationTest) {
	CSVRow* cr = new CSVRow();
	//ASSERT_EQ(false, cr);
	delete(cr);
}

