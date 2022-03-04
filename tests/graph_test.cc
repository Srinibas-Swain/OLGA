#include "gtest/gtest.h"
#include <cmath>
#include <iostream>

#include "graph.hh"

class GraphTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(GraphTest, FromGraphIdTest) {
	Graph *graph = Graph::from_graph_id(960);
	//graph->print();
	for(int ix=1; ix<5; ix++)
		ASSERT_EQ(1, graph->at(0,ix));
	delete(graph);
}

TEST_F(GraphTest, ConstructGraphIdTest) {
	int n=5;
	Graph* graph = new Graph(n);
	for(int ix=1; ix<n; ix++)
		graph->set_edge(0, ix);
	//graph->print();
	graph->construct_graph_id();
	ASSERT_EQ(960, graph->graph_id);
	delete(graph);
}

TEST_F(GraphTest, CopyTestException) {
	int n=5;
	Graph* k_n = Graph::K_n(n);
	Graph* dst = Graph::K_n(n-2);
	vector<int> vertices;
	for(int ix=0; ix<n; ix++)
		vertices.push_back(ix);
	try{
		k_n->copy(dst, vertices);
		FAIL();
	}
	catch(const char* msg) {
		ASSERT_EQ("graph-dst has less vertices", msg);
	}
	delete(dst);
	delete(k_n);
}

TEST_F(GraphTest, CopyTestException2) {
	int n=5;
	Graph* k_n = Graph::K_n(n);
	Graph* dst = Graph::K_n(n-2);
	vector<int> vertices;
	for(int ix=0; ix<n-2; ix++)
		vertices.push_back(ix);
	vertices[1] = n;
	try{
		k_n->copy(dst, vertices);
		FAIL();
	}
	catch(const char* msg) {
		ASSERT_EQ("graph-src has less vertices", msg);
	}
	delete(dst);
	delete(k_n);
}

TEST_F(GraphTest, CopyDataSanityTest) {
	int n=5;
	Graph* src = new Graph(n);
	Graph* dst = new Graph(n-2);

	src->set_edge(0,0);
	src->set_edge(0,2);
	src->set_edge(0,4);
	src->set_edge(2,4);
	//std::cout << src->at(4,4) << std::endl;
	vector<int> vertices;
	vertices.push_back(0);
	vertices.push_back(2);
	vertices.push_back(4);
	src->copy(dst, vertices);

	ASSERT_EQ(1, dst->at(0,0));
	ASSERT_EQ(1, dst->at(0,1));
	ASSERT_EQ(1, dst->at(0,2));
	ASSERT_EQ(1, dst->at(1,2));
	
	delete(dst);
	delete(src);
}

TEST_F(GraphTest, GetMaxDegreeTest) {
	Graph* graph = new Graph(3);
	std::vector<int> degrees(5, 0);
	degrees[0] = 10;
	degrees[1] = 30;
	degrees[2] = 20;
	degrees[3] = 50;
	degrees[4] = 40;
	int i_max = graph->get_max_degree(degrees);
	ASSERT_EQ(50, i_max);
	delete(graph);
}

TEST_F(GraphTest, CopyExceptTest) {
	Graph* src = new Graph(8);
	src->set_edge(0, 1);	src->set_edge(0, 3);	src->set_edge(0, 5);	src->set_edge(0, 7);
	src->set_edge(1, 2);	src->set_edge(1, 4);	src->set_edge(1, 6);
	src->set_edge(2, 3);	src->set_edge(2, 5);	src->set_edge(2, 7);
	src->set_edge(3, 4);	src->set_edge(3, 6);
	src->set_edge(4, 6);
	src->set_edge(5, 7);
	Graph* dst = new Graph(7);
	src->copy_except(dst, 3);

	ASSERT_EQ(true, dst->is_edge(0,1));
	ASSERT_EQ(false, dst->is_edge(0,2));
	ASSERT_EQ(false, dst->is_edge(0,3));
	ASSERT_EQ(true, dst->is_edge(0,4));
	ASSERT_EQ(false, dst->is_edge(0,5));
	ASSERT_EQ(true, dst->is_edge(0,6));
	
	ASSERT_EQ(true, dst->is_edge(1,2));
	ASSERT_EQ(true, dst->is_edge(1,3));
	ASSERT_EQ(false, dst->is_edge(1,4));
	ASSERT_EQ(true, dst->is_edge(1,5));
	ASSERT_EQ(false, dst->is_edge(1,6));

	ASSERT_EQ(false, dst->is_edge(2,3));
	ASSERT_EQ(true, dst->is_edge(2,4));
	ASSERT_EQ(false, dst->is_edge(2,5));
	ASSERT_EQ(true, dst->is_edge(2,6));

	ASSERT_EQ(false, dst->is_edge(3,4));
	ASSERT_EQ(true, dst->is_edge(3,5));
	ASSERT_EQ(false, dst->is_edge(3,6));

	ASSERT_EQ(false, dst->is_edge(4,5));
	ASSERT_EQ(true, dst->is_edge(4,6));

	
	delete(src);
	delete(dst);
}



