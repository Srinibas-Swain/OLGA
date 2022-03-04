#include "gtest/gtest.h"
#include "frequent_induced_subgraph.hh"

#include <cmath>
#include <iostream>

class FrequentInducedSubgraphTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(FrequentInducedSubgraphTest, InitGraphPoolTest) {
	FrequentInducedSubgraph *fisg = new FrequentInducedSubgraph();
	fisg->graph_pool->init(2);
	delete(fisg);
}


