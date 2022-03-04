#ifndef GRAPH_CONNECTIVITY_HELPER_HH
#define GRAPH_CONNECTIVITY_HELPER_HH

#include "menu_option.hh"
#include "menu_option_helper.hh"

#include "graph_connectivity.hh"
#include "utils.hh"

//- other headers -- start
//- other headers -- end

namespace ga {
        class GraphConnectivityHelper : public MenuOptionHelper {
        public: 
                GraphConnectivityHelper(GraphConnectivity* _gc_mo);
                
                ~GraphConnectivityHelper();
                
                void init(int n) override;
                void process_graph(const Graph*);
                void init_for_new_n(int num_vertices) override;
		//void APUtil(const ga::Graph *graph, int , bool [], int [], int [], int [], bool []);
                int bfs(vector< vector<int> >& , int, int , int , int []);
		void dfs(vector< vector<int> >&, int, int , bool []);
		int minCut(const Graph* graph, int, int );
                
                //- public functions -- start
                //- public functions -- end
          
        private:
                GraphConnectivity* gc_mo;
                
                void init_for_graph(const Graph* graph);
                
                //- custom private -- start
                ///- vertex connectivity
                int vertexConnectivity(const Graph* graph);
                ///- edge Connectivity
                int edgeConnectivity(const Graph* graph);
        };
}

#endif /* GRAPH_ONNECTIVITY_HELPER_HH */

