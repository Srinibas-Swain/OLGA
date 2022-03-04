#ifndef NODE_LIST_HH
#define NODE_LIST_HH

#include "node.hh"
#include "utils.hh"

#include <string>

namespace ga {
	class NodeList {
	public:
		NodeList(int _direction);
		~NodeList();

		void init(int _num_nodes);
		bool is_present(int ix_node);
		bool is_present(Node *node);
		void prepend(Node* node);
		void append(Node* node);
		//void reverse();
		void print(std::string str_prefix="") const;

	private:
		int direction;
		int num_nodes;
		Node **nodes;
		bool *present;
		int head;

		void reallocate(int _num_nodes);
		void free_data();
		void validate_bounds(int ix_node);
		void validate_duplicate(Node *node);
	};
}

#endif /* NODE_LIST_HH */

