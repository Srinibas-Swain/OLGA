#include "gtest/gtest.h"
#include "bitmap.hh"

#include <iostream>

class BitmapTest : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

class BitmapTester : public Bitmap
{
public:
	BitmapTester(int n) : Bitmap(n) { }
	bool is_ix_set(int ix) {return data[ix];}
};

TEST_F(BitmapTest, SetTest) {
  BitmapTester* b = new BitmapTester(10);
  b->set(3);
  b->set(7);
  for(int ix=0; ix<10; ix++) {
	  if(ix==3 || ix==7)
		  ASSERT_EQ(true, b->is_ix_set(ix));
	  else
		  ASSERT_EQ(false, b->is_ix_set(ix));
  }
  delete(b);
}

TEST_F(BitmapTest, IsSetTest) {
	BitmapTester* b = new BitmapTester(10);
	b->set(3);
	b->set(8);
	for(int ix=0; ix<10; ix++) {
		if(ix==3 || ix==8)
			ASSERT_EQ(true, b->is_set(ix));
		else
			ASSERT_EQ(false, b->is_set(ix));
	}
	delete(b);
}

TEST_F(BitmapTest, UnsetTest) {
	BitmapTester* b = new BitmapTester(10);
	b->set(3);
	b->set(5);
	b->set(8);

	b->unset(8);
	
	for(int ix=0; ix<10; ix++) {
		if(ix==3 || ix==5)
			ASSERT_EQ(true, b->is_set(ix));
		else
			ASSERT_EQ(false, b->is_set(ix));
	}
	delete(b);
}

TEST_F(BitmapTest, ResetTest) {
	BitmapTester* b = new BitmapTester(10);
	b->set(3);
	b->set(5);
	b->set(8);
	b->reset();

	for(int ix=0; ix<10; ix++) {
		ASSERT_EQ(false, b->is_set(ix));
	}
	delete(b);
}

TEST_F(BitmapTest, NextTest1) {
	BitmapTester* b = new BitmapTester(10);
	for(int ix=9; ix>=0; ix--) {
		b->set(ix);
		ASSERT_EQ(ix, b->next());
	}
	delete(b);
}

TEST_F(BitmapTest, NextTest2) {
	BitmapTester* b = new BitmapTester(10);
	ASSERT_EQ(10, b->next());
	for(int ix=9; ix>=0; ix--) {
		b->set(ix);
		ASSERT_EQ(ix, b->next(ix-1));
	}
	delete(b);
}

TEST_F(BitmapTest, EqualTest) {
	BitmapTester* b1 = new BitmapTester(10);
	BitmapTester* b2 = new BitmapTester(10);

	ASSERT_EQ(true, (*b1) == b2);

	b1->set(4);
	b2->set(5);

	ASSERT_EQ(false, (*b1) == b2);

	b1->set(5);
	b2->set(4);

	ASSERT_EQ(true, (*b1) == b2);


	delete(b1);
	delete(b2);
}

TEST_F(BitmapTest, MatchesTest) {
	BitmapTester* b1 = new BitmapTester(10);
	BitmapTester* b2 = new BitmapTester(10);

	ASSERT_EQ(10, b1->num_matches(b2));

	b1->set(4);
	b2->set(4);

	ASSERT_EQ(10, b1->num_matches(b2));

	b1->set(3);
	b2->set(4);

	ASSERT_EQ(9, b1->num_matches(b2));

	delete(b1);
	delete(b2);
}

TEST_F(BitmapTest, SubsetTest) {
	BitmapTester* b1 = new BitmapTester(10);
	BitmapTester* b2 = new BitmapTester(10);

	ASSERT_EQ(true, (*b1) <= b2);

	b1->set(4);
	b2->set(5);

	ASSERT_EQ(false, (*b1) <= b2);

	b1->set(5);
	b2->set(4);

	ASSERT_EQ(true, (*b1) <= b2);

	delete(b1);
	delete(b2);
}

TEST_F(BitmapTest, ProperSubsetTest) {
	BitmapTester* b1 = new BitmapTester(10);
	BitmapTester* b2 = new BitmapTester(10);

	ASSERT_EQ(false, (*b1) < b2);

	b1->set(4);
	b2->set(5);

	ASSERT_EQ(false, (*b1) < b2);

	b1->set(5);
	b2->set(4);

	ASSERT_EQ(false, (*b1) < b2);

	b2->set(3);
	ASSERT_EQ(true, (*b1) < b2);
	
	delete(b1);
	delete(b2);
}
