#include "gtest/gtest.h"
#include "graph_pool.hh"

#include <iostream>

class GraphPoolTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(GraphPoolTest, ClearTest) {
	GraphPool* gp = new GraphPool(5);
	gp->clear();
	delete(gp);
}

TEST_F(GraphPoolTest, GetTest) {
	GraphPool* gp = new GraphPool(5);
	Graph* graph;
	for(int ix=0;ix<5;ix++) {
		graph = gp->get(ix);
		ASSERT_EQ(ix, graph->num_vertices);
	}
	delete(gp);
}



