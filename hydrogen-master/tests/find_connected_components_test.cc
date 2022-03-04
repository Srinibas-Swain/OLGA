#include "gtest/gtest.h"
#include "find_connected_components.hh"

#include <iostream>

class FindConnectedComponentsTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(FindConnectedComponentsTest, ForGraphTest) {
	Graph* graph = new Graph(9);
	graph->set_edge(0,1);	graph->set_edge(4,5); graph->set_edge(4,6);
	graph->set_edge(1,2);	graph->set_edge(5,7);
							graph->set_edge(7,8);

	std::vector<Graph*> components = FindConnectedComponents::for_graph(graph);

	Graph* component;

	component = components[0];
	ASSERT_EQ(3, component->num_vertices);
	ASSERT_EQ(true, component->is_edge(0, 1));
	ASSERT_EQ(false, component->is_edge(0, 2));
	ASSERT_EQ(true, component->is_edge(1, 2));

	component = components[1];
	ASSERT_EQ(1, component->num_vertices);
	ASSERT_EQ(false, component->is_edge(0, 0));

	component = components[2];
	ASSERT_EQ(5, component->num_vertices);
	ASSERT_EQ(true,  component->is_edge(0, 1)); // 4,5
	ASSERT_EQ(false, component->is_edge(0, 2)); // 4,7
	ASSERT_EQ(false, component->is_edge(0, 3)); // 4,8
	ASSERT_EQ(true,  component->is_edge(0, 4)); // 4,6

	ASSERT_EQ(true,  component->is_edge(1, 2)); // 5,7
	ASSERT_EQ(false, component->is_edge(1, 3)); // 5,8
	ASSERT_EQ(false, component->is_edge(1, 4)); // 5,6
	
	ASSERT_EQ(true,  component->is_edge(2, 3)); // 7,8
	ASSERT_EQ(false, component->is_edge(2, 4)); // 7,6
	
	ASSERT_EQ(false, component->is_edge(3, 4)); // 8,6
	
	for(auto comp : components)
		delete(comp);
	
	delete(graph);
}

