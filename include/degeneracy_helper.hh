#ifndef DEGENERACY_HELPER_HH
#define DEGENERACY_HELPER_HH

#include "menu_option.hh"
#include "menu_option_helper.hh"

#include "degeneracy.hh"

//-- other headers
#include "node_list.hh"
#include "node.hh"
#include "utils.hh"


namespace ga {
	class DegeneracyHelper : public MenuOptionHelper {
	public:
		DegeneracyHelper(Degeneracy* _degen_mo);
		~DegeneracyHelper();

		void init(int n) override;
		void process_graph(const Graph*);

		std::unordered_map<int, std::unordered_map<long long, int> > db_degen;

		//-- other helper functions
		
		bool validate_ss_condition(const Graph* graph, int k,
								   bool _b_debug=false, bool _b_debug2=false);
	  
	private:
		Degeneracy* degen_mo;
		
		//-- custom
		int num_vertices;
		long long graph_id;
		NodeList *L;
		Node **nodes;
		int *degrees;
		Node **D;
		Graph *graph_child;
		int current_degeneracy;

		int index_D_non_empty(int ix_ne=0);
		void validate_non_empty_D(int ix);
		void validate_bounds(int ix);
		Node* pop_from_D(int ix_ne);
		void update_D(int deg_old, int deg_new, Node *w);

		bool valid_w(const Graph *graph, Node *v, Node *w);
		void free_data();
		void reallocate(int _num_vertices);
		void print_D() const;

		void init_for_graph(const Graph* graph);
		void init_gchild_from(const Graph* graph);
		
		int max_degeneracy_components(const Graph*, std::ostringstream& oss_out, bool _b_debug);
	};
}

#endif /* DEGENERACY_HELPER_HH */

