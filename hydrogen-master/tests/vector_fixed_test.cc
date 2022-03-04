#include "gtest/gtest.h"
#include "vector_fixed.hh"

#include <iostream>

class VectorFixedTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(VectorFixedTest, SizeTestZeroth) {
	VectorFixed* vf = new VectorFixed(5);
	ASSERT_EQ(0, vf->size());
	delete(vf);
}

TEST_F(VectorFixedTest, SizeTestTwo) {
	VectorFixed* vf = new VectorFixed(5);
	vf->add(5);
	vf->add(10);
	ASSERT_EQ(2, vf->size());
	delete(vf);
}

TEST_F(VectorFixedTest, ResetTest) {
	VectorFixed* vf = new VectorFixed(5);
	vf->add(5);
	vf->add(10);
	vf->reset();
	ASSERT_EQ(0, vf->size());
	delete(vf);
}

TEST_F(VectorFixedTest, NextTest) {
	VectorFixed* vf = new VectorFixed(5);
	vf->add(5);
	vf->add(10);
	ASSERT_EQ(5, vf->next());
	ASSERT_EQ(10, vf->next());
	delete(vf);
}

TEST_F(VectorFixedTest, BeginTest) {
	VectorFixed* vf = new VectorFixed(5);
	vf->add(5);
	vf->add(10);
	vf->add(15);
	int *data = vf->begin();
	ASSERT_EQ(5, data[0]);
	ASSERT_EQ(10, data[1]);
	delete(vf);
}

TEST_F(VectorFixedTest, EndTest) {
	VectorFixed* vf = new VectorFixed(5);
	vf->add(5);
	vf->add(10);
	vf->add(15);
	int *data = vf->end();
	data--;
	ASSERT_EQ(15, *data);
	delete(vf);
}

TEST_F(VectorFixedTest, IteratorTest) {
	VectorFixed* vf = new VectorFixed(5);
	vf->add(5);
	vf->add(10);
	vf->add(15);
	int expected[] = {5, 10, 15};
	int ix=0;
	for(auto v : *vf) {
		ASSERT_EQ(expected[ix++], v);
	}
	delete(vf);
}



