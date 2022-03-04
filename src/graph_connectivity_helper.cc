#include "graph_connectivity_helper.hh"
#define NIL -1
using namespace ga;

//-- custom headers - start
#include <limits>
#include <queue>
#include <set>
//-- custom headers - end

GraphConnectivityHelper::GraphConnectivityHelper(GraphConnectivity* _gc_mo) {
	gc_mo = _gc_mo;
	//- custom -- start
	
	//- custom -- end
}

GraphConnectivityHelper::~GraphConnectivityHelper() {
	//- custom deletions -- start
	//delete(parents); //delete(distances);
	//delete(to_visit);
	//delete(visited);
	//- custom deletions -- end
}

void GraphConnectivityHelper::init(int n)
{
	//- custom inits
}

int GraphConnectivityHelper::bfs(vector<vector<int> > &rGraph, int V, int s, int t, int parent[])
{
    // Create a visited array and mark all vertices as not visited
	bool visited[V];
	memset(visited, 0, sizeof(visited));
 
    // Create a queue, enqueue source vertex and mark source vertex
    // as visited
	queue <int> q;
	q.push(s);
	visited[s] = true;
	parent[s] = -1;
    // Standard BFS Loop
	while (!q.empty()){
		    
		int u = q.front();
		q.pop();
 
		for (int v=0; v<V; v++){
        
			if (visited[v]==false && rGraph[u][v] > 0){
            			q.push(v);
                		parent[v] = u;
                		visited[v] = true;
            		}
        	}
    	}
    // If we reached sink in BFS starting from source, then return
    // true, else false
    	return (visited[t] == true);
}
 
// A DFS based function to find all reachable vertices from s.  The function
// marks visited[i] as true if i is reachable from s.  The initial values in
// visited[] must be false. We can also use BFS to find reachable vertices
void GraphConnectivityHelper::dfs(vector<vector<int> > &rGraph, int V, int s, bool visited[])
{
	visited[s] = true;
    	for (int i = 0; i < V; i++)
		if (rGraph[s][i] && !visited[i])
           		dfs(rGraph, V, i, visited);
}
 
// Prints the minimum s-t cut
int GraphConnectivityHelper::minCut(const Graph*  graph, int s, int t)
{
	int u, v,count=0;
	int minimum=std::numeric_limits<int>::max();
        int V = graph->num_vertices;
    // Create a residual graph and fill the residual graph with
    // given capacities in the original graph as residual capacities
    // in residual graph
//	int rGraph[V][V]; // rGraph[i][j] indicates residual capacity of edge i-j
//	std::vector<std::vector<int>> rGraph;
	std::vector<std::vector<int>> rGraph(V, std::vector<int>(V));
	for (u = 0; u < graph->num_vertices; u++)
		for (v = 0; v < graph->num_vertices; v++)
			if (graph->is_edge(u,v))
				rGraph[u][v] = 1;
			else
				rGraph[u][v]=0;

/*	for (int i = 0; i < rGraph.size(); i++)
		{
    		for (int j = 0; j < rGraph[i].size(); j++)
    			{
        		cout << rGraph[i][j];
    		}
    		cout<<endl;
	}

*/

	int parent[graph->num_vertices];  // This array is filled by BFS and to store path
 
	FORix(V)
	   FORjx(V){
		s=ix;
		t=jx;
		if (!graph->is_edge(s,t) && s!=t){
    // Augment the flow while tere is path from source to sink
			while (bfs(rGraph, V, s, t, parent)){
        // Find minimum residual capacity of the edges along the
        // path filled by BFS. Or we can say find the maximum flow
        // through the path found.
				int path_flow = std::numeric_limits<int>::max();
//                cout<<"value of s"<<s<<endl;
//                cout<<"value of t"<<t<<endl;
				for (v=t; v!=s; v=parent[v]){
        				u = parent[v];
					path_flow = min(path_flow, rGraph[u][v]);
				}		
 
        // update residual capacities of the edges and reverse edges
        // along the path
				for (v=t; v != s; v=parent[v]){
					u = parent[v];
					rGraph[u][v] -= path_flow;
					rGraph[v][u] += path_flow;
				}
			}
 
    // Flow is maximum now, find vertices reachable from s
			bool visited[graph->num_vertices];
    			memset(visited, false, sizeof(visited));
			dfs(rGraph, V, s, visited);
 
    // Print all edges that are from a reachable vertex to
    // non-reachable vertex in the original graph
			FORix(graph->num_vertices){
				FORjx(graph->num_vertices)
					if (visited[ix] && !visited[jx] && graph->is_edge(ix,jx)){
			//			cout << ix << " - " << jx << endl;
						count=count+1;
						//cout<<"#####"<<count<<"******"<<endl;
				}	
			}
			if (count!=0 && count<minimum){
				minimum=count;
			}
			count=0;	
 		}	
	  }	
	//cout<<"print the val"<<minimum<<endl;
	return minimum;
}
void GraphConnectivityHelper::process_graph(const Graph* graph)
{
	//- the actual logic of how to process this graph
	init_for_graph(graph);
	GraphConnectivity::Graph_Conn_map gc;
	gc["vertex connectivity"] = std::to_string(vertexConnectivity(graph)==std::numeric_limits<int>::max()? (graph->num_vertices -1):vertexConnectivity(graph));
	gc["edge connectivity"] = std::to_string(edgeConnectivity(graph));

	#pragma omp critical(save_parameters)
	{
		gc_mo->save(graph, gc);
	}
}

/// length of a shortest cycle contained in the graph
/// If the graph does not contain any cycles (i.e. it's an acyclic graph),
///    its girth is defined to be infinity
/*int GraphConnectivityHelper::vertexConnectivity(const ga::Graph *graph)
{
	return 1;
}
*/
 
// The function to do DFS traversal. It uses recursive function APUtil()
int GraphConnectivityHelper::vertexConnectivity(const ga::Graph *graph)
{
    // Mark all the vertices as not visited
        int vconn=0;
	vconn=minCut(graph,0,0);
	return vconn;
}

/*
void GraphTopologicalPropertiesHelper::init_girth_for_vertex(int num_vertices)
{
	visited->reset();
	to_visit->reset();
	FORix(num_vertices) {
		parents[ix]   = -1;
		distances[ix] =  0;
	}
	
}
*/
int GraphConnectivityHelper::edgeConnectivity(const ga::Graph *graph)
{
	return -1;
}

void GraphConnectivityHelper::init_for_graph(const Graph* graph)
{
}

void GraphConnectivityHelper::init_for_new_n(int num_vertices)
{
	/*
	if(visited != nullptr)   delete(visited);
	if(to_visit != nullptr) delete(to_visit);
	visited  = new VertexSet(num_vertices);
	to_visit = new VertexSet(num_vertices);

	if(parents != nullptr) free(parents);
	if(distances != nullptr) free(distances);
	parents   = (int*) calloc(num_vertices, sizeof(int));
	distances = (int*) calloc(num_vertices, sizeof(int));
*/
}

//-- custom methods


