#include "node.hh"

using namespace ga;

Node::Node(int _val) {
	val = _val;
	reset();
}

Node::~Node() {

}

void Node::reset()
{
	prev = nullptr;
	next = nullptr;
}

void Node::init()
{
	reset();
}

