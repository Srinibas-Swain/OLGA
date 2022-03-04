#include "gtest/gtest.h"
#include "bool_iterator.hh"

#include <iostream>
#include <cmath>

class BoolIteratorTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(BoolIteratorTest, BasicTest) {
	bool data[] = {false, true, false, false, true};
	BoolIterator *bi = new BoolIterator();
	bi->init(data, 5);
	ASSERT_EQ(false, bi->is_done());
	delete(bi);
}

TEST_F(BoolIteratorTest, CountTest) {
	bool data[] = {false, true, false, false, true};
	BoolIterator *bi = new BoolIterator();
	bi->init(data, 5);

	int count=0;
	while(bi->not_yet_done())
		count++;
	
	ASSERT_EQ(2, count);
	delete(bi);
}

TEST_F(BoolIteratorTest, DataSanityTest) {
	bool data[] = {false, true, false, false, true};
	BoolIterator *bi = new BoolIterator();
	bi->init(data, 5);

	int elements[] = {1, 4}; int ix=0;
	int ix_current;
	while(bi->not_yet_done()) {
		ix_current = bi->current();
		ASSERT_EQ(elements[ix++], ix_current);
	}	
	delete(bi);
}

