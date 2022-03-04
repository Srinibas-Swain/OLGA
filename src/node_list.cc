#include "node_list.hh"
#include <iostream>
using namespace ga;

NodeList::NodeList(int _direction) {
	direction = _direction;
	num_nodes = 0;
}

NodeList::~NodeList() {
	free_data();
}

void NodeList::append(Node *node)
{
	// racist exception
	if(direction == -1)
		throw std::runtime_error("cant append to a backward list");

	validate_bounds(head);
	validate_duplicate(node);

	present[node->val] = true;
	nodes[head] = node;

	head += direction;
}

void NodeList::print(std::string str_prefix) const
{
	int ix_node = (direction==+1) ? 0 : num_nodes-1;
	Node *node = nodes[ix_node];
	std::cout << str_prefix << ": { ";
	while(node != nullptr) {
		std::cout << node->val << ",";
		ix_node += direction;
		node = nodes[ix_node];
	}
	std::cout << "}" << std::endl;
}

void NodeList::prepend(Node *node)
{
	// racist exception
	if(direction == +1)
		throw std::runtime_error("cant prepend to a forward list");

	validate_bounds(head);
	validate_duplicate(node);

	present[node->val] = true;
	nodes[head] = node;
	head += direction;
}

void NodeList::validate_duplicate(Node *node)
{
	if(is_present(node)) {
		std::ostringstream oss_err;
		oss_err << "NodeList duplicate error: " <<
			node->val << " already present";
		throw std::runtime_error(oss_err.str());
	}
}

bool NodeList::is_present(Node *node)
{
	return is_present(node->val);
}

bool NodeList::is_present(int ix_node)
{
	validate_bounds(ix_node);
	return present[ix_node];
}

// void NodeList::reverse() {
// 	Node *node_tmp;
// 	for(int ix=0; ix<num_nodes/2; ix++) {
// 		node_tmp = nodes[ix];
// 		nodes[ix] = nodes[num_nodes-1-ix];
// 		nodes[num_nodes-1-ix] = node_tmp;
// 	}
// }

void NodeList::init(int _num_nodes)
{
	if(num_nodes != _num_nodes) {
		// reallocate present, data
		reallocate(_num_nodes);
		num_nodes = _num_nodes;
	}
	FORix(num_nodes) {
		present[ix] = false;
		nodes[ix] = nullptr;
		head = (direction == +1) ? 0 : (num_nodes-1);
	}
}

void NodeList::reallocate(int _num_nodes)
{
	free_data();
	present = (bool*) calloc(_num_nodes, sizeof(bool));
	nodes = (Node**) calloc(_num_nodes, sizeof(Node*));
}

void NodeList::free_data()
{
	if(num_nodes==0)
		return;
	//std::cout << "free-data:num-nodes: " << num_nodes << std::endl;

	free(nodes);
	free(present);
}

void NodeList::validate_bounds(int ix_node)
{
	std::ostringstream oss_err;
	oss_err << "NodeList bounds error: ";
	if(direction == +1) {
		if(ix_node >= num_nodes) {
			oss_err << ix_node << " >= " << num_nodes;
			throw std::runtime_error(oss_err.str());
		}
	} else {
		if(ix_node < 0) {
			oss_err << ix_node << " < " << 0;
			throw std::runtime_error(oss_err.str());
		}
	}
}

