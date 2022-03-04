#ifndef __GRAPH_HH
#define __GRAPH_HH

#include <stdexcept>

#ifndef eigen_assert
#define eigen_assert(x)											\
	if(!(x)) throw (std::runtime_error("Put your message here"));
#else
#undef eigen_assert
#define eigen_assert(x)											\
	if(!(x)) throw (std::runtime_error("Put your message here"));
#endif

#include <algorithm>
#include <Eigen/Dense>
#include <string>
#include <vector>
#include <cstdlib>

//#include "blocks.hh"
#include "vector_fixed.hh"

using namespace std;
using namespace Eigen;

namespace ga {
	
class Graph {
public:
	// static Graph* generate(int num_vertices, int n, Graph* graph = std::nullptr_t);
	
	int num_vertices;
	long long graph_id;
	
	Graph();
	Graph(int _num_vertices);

	// read from file
	Graph(string filename);
	~Graph();

	void print(string str_prefix="") const;
	void print2(string str_prefix="") const;
	std::string get_string(std::string str_prefix="") const;
	bool connected() const;
	Graph* clone() const;
	bool is_edge(int ix, int jx) const;
	void copy(Graph* dst) const;
	void copy(Graph* dst, VectorFixed* vertices) const;
	bool is_k_n(long long certificate=-1) const;
	bool is_tree(bool b_connected) const;
	std::vector<int> get_degrees() const;
	int get_max_degree(std::vector<int> degrees = std::vector<int>()) const;
	std::vector<int> get_cut_vertices() const;

	void copy_edges_from(Graph const* src, int ix_vertex);
	void copy_edges_from(Graph const* src, std::vector<int> vertices);
	void copy_sans_vertex_into(Graph* other_graph, int vertex) const;
	void copy_sans_blanket_into(Graph* other_graph, int vertex) const;
	int number_of_nbrs(int vertex) const;
	void copy_complement_to(Graph* dst) const;
	
	//bool is_tree();
	int number_of_edges();
	void add_edge(int ix, int jx);
	void set_edge(int row, int col);
	void remove_edge(int ix, int jx);
	void reset_vertex(int ix_vertex);
	void reset_edge(int row, int col);
	void set_all_edges();
	void reset_all_edges();
	void construct_graph_id();
	void imbibe_graph_id(long long graph_id);
	// not thread safe
	void complement();

	// generate a complete graph with n vertices
	static Graph* K_n(int n);
	// store and retrieve number of upper triangular entries
	static vector<long> NUM_UPPER_TRIANGULAR;
	static void init_upper_triangular(int n);
	static long num_upper_triangular(int n);
	static Graph* from_graph_id(long long graph_id);
	
private:
	// init the matrix with zeros
	void init_matrix(int num_vertices);
	void fill(bool value); // where all?
	void fill_diagonal(bool value); // where all?

	void set_num_vertices(string line);
	void read_adj_list(ifstream file);
	void update_reachability(int, bool*) const;
	void check_bounds(int row, int col) const;
	void validate_row_col(int row, int col, std::string requestor="") const;

	void get_cut_vertices_helper(int vertex,
								 bool *visited, int *discovered, int *lows, int *parents,
								 bool *art_points, int& time_cut_vertex) const;
	
	//MatrixXf mat;
	bool **data;
	int _num_edges;
};
}

#endif
