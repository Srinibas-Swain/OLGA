#ifndef NODE_HH
#define NODE_HH

namespace ga {
	class Node {
	public:
		Node(int _val);
		~Node();

		void init();
		void reset();

		Node *prev, *next;
		int val;
	private:

	};
}

#endif /* NODE_HH */

