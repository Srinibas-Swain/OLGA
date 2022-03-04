#include "graph.hh"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cmath>

#include "utils.hh"

using namespace ga;

Graph::Graph(int _num_vertices) {
	num_vertices = _num_vertices;
	//mat.resize(num_vertices, num_vertices);
	init_matrix(num_vertices);
	//fill(false);
}

Graph::Graph(string filename)
{
	ifstream file(filename.c_str());
	string line;
	// read number of vertices
	getline(file, line);
	set_num_vertices(line);
	cout << "num: " << num_vertices << endl;
	// todo: move this to separate method
	init_matrix(num_vertices);
	int ix_row = 0;
	int vertex;
	while(getline(file, line)) {
		istringstream iss(line);
		while((iss >> vertex)) {
			// mat(ix_row, vertex) = 1;
			data[ix_row][vertex] = true;
		}
		ix_row++;
	}
}

void Graph::set_num_vertices(string line)
{
	istringstream iss(line);
	iss >> num_vertices;
}

void Graph::set_all_edges()
{
	for(int col=1; col<num_vertices; col++) {
		//FORix(num_vertices) {
		for(int row=0; row<col; row++) {
			data[row][col] = true;
			data[col][row] = true;
		}
	}
	construct_graph_id();
}

void Graph::reset_all_edges()
{
	fill(false);
	_num_edges = -1;
}

void Graph::init_matrix(int num_vertices)
{
	//mat.resize(num_vertices, num_vertices);
	data = (bool**) malloc(num_vertices * sizeof(bool*));
	FORix(num_vertices)
		data[ix] = (bool*) calloc(num_vertices, sizeof(bool));
	// memset equivalent? == fill
	// for (int ix = 0; ix < num_vertices; ix++)
	// 	for (int iy = 0; iy < num_vertices; iy++)
	// 		mat(ix, iy) = 0;
	_num_edges = -1;
}

void Graph::copy_sans_vertex_into(Graph* other_graph, int vertex) const
{
	int r_dst=0;
	for(int r_src=0; r_src<num_vertices; r_src++) {
		if(r_src==vertex) continue;
		for(int c_src=r_src+1, c_dst=r_dst+1; c_src<num_vertices; c_src++) {
			if(c_src==vertex) continue;
                        
			if(is_edge(r_src, c_src))
				other_graph->set_edge(r_dst, c_dst);
			else
				other_graph->reset_edge(r_dst, c_dst);
			++c_dst;
		}
		++r_dst;
	}
	other_graph->construct_graph_id();
}

void Graph::copy_sans_blanket_into(Graph* other_graph, int vertex) const
{
	int r_dst=-1;
	for(int r_src=0; r_src<num_vertices; r_src++) {
		if((r_src==vertex) ||
		   (is_edge(r_src, vertex))) continue;
		++r_dst;
		for(int c_src=r_src+1, c_dst=r_dst+1; c_src<num_vertices; c_src++) {
			if((c_src==vertex) ||
			   (is_edge(c_src, vertex))) continue;
			if(is_edge(r_src, c_src))
				other_graph->set_edge(r_dst, c_dst);
			else
				other_graph->reset_edge(r_dst, c_dst);
			++c_dst;
		}
	}
	other_graph->construct_graph_id();
}

int Graph::number_of_nbrs(int vertex) const
{
	int in_nbrs = 0;
	FORix(num_vertices) {
		if(is_edge(vertex, ix)) ++in_nbrs;
	}
	return in_nbrs;
}

void Graph::copy_complement_to(Graph* dst) const
{
	for(int row=0; row<num_vertices; row++) {
		for(int col=row+1; col<num_vertices; col++) {
			if(is_edge(row, col))
				dst->remove_edge(row, col);
			else
				dst->set_edge(row, col);
		}
	}
}

void Graph::imbibe_graph_id(long long _graph_id)
{
	long long _graph_id_orig = _graph_id;
	reset_all_edges();
	_num_edges = 0;
	long long remainder;
	for(int col=num_vertices-1; col>0; col--) {
		for(int row=col-1; row>=0; row--) {
			remainder = _graph_id % 2;
			if(remainder == 1) {
				set_edge(row, col);
				_num_edges += 1;
			}
			_graph_id /= 2;
			//std::cout << std::endl;
		}
	}
	construct_graph_id();
	if(_graph_id_orig != graph_id) {
		std::cout << "from_graph_id:: contrition"; NEWLINE;
	}
}

void Graph::complement()
{
	for(int row=0; row<num_vertices; row++) {
		for(int col=row+1; col<num_vertices; col++) {
			// ternary?
			if(is_edge(row, col))
				reset_edge(row, col);
			else
				set_edge(row, col);
		}
	}
}

Graph* Graph::K_n(int n)
{
	Graph *k_n  = new Graph(n);
	k_n->fill(true);
	k_n->fill_diagonal(false);
	k_n->construct_graph_id();
	k_n->_num_edges = n*(n-1)/2;
	return k_n;
}

void Graph::init_upper_triangular(int n)
{
	// DEBUG3 std::cout << "init upper triangular ... " << std::endl;
	// DEBUG3 std::cout << "old size: " << Graph::NUM_UPPER_TRIANGULAR.size() <<
	//	"new size: " << n << std::endl;
	Graph::NUM_UPPER_TRIANGULAR.reserve(n+1);
	FORix(n+1) {
		// todo: no need to multiply; just add in loop
		Graph::NUM_UPPER_TRIANGULAR[ix] = ix*(ix-1)/2;
	}
}

long Graph::num_upper_triangular(int n)
{
	return Graph::NUM_UPPER_TRIANGULAR[n];
}

// row major, from left to right
// with (0,1) as MSB and (n-2,n-1) as LSB
void Graph::construct_graph_id()
{
	graph_id = 0;
	for(int col=1; col<num_vertices; col++) {
		//FORix(num_vertices) {
		for(int row=0; row<col; row++) {
			graph_id <<= 1;
			//if(mat(ix,jx)==1) {
			if(data[row][col]) {
				graph_id |= 1;
			}
		}
	}
}

// row major, from left to right
// with (0,1) as MSB and (n-2,n-1) as LSB
Graph* Graph::from_graph_id(long long graph_id)
{
	int remainder;
	long in_bits = (long)ceil(log2(graph_id));
	//std::cout << "bits: " << in_bits << std::endl;
	int num_vertices = (-1 + sqrt(1+8*in_bits))/2.0 + 1.0;
	//std::cout << "nv: " << num_vertices << std::endl;
	Graph* graph = new Graph(num_vertices);
	graph->fill(false);
	graph->_num_edges = 0;
	for(int row=num_vertices-2; row>=0; row--) {
		for(int col=num_vertices-1; col>row; col--) {
			//std::cout << "(" << row << "," << col << ") " << graph_id;
			remainder = graph_id % 2;
			//std::cout << " r: " << remainder;
			if(remainder == 1) {
				//graph->set_edge(row, col);
				graph->data[row][col] = true;
				graph->_num_edges += 1;
			}
			graph_id /= 2;
			//std::cout << std::endl;
		}
	}
	graph->construct_graph_id();
	return graph;
}

void Graph::fill(bool value)
{
	//mat.fill(n);
	FORix(num_vertices)
		memset(data[ix], value, sizeof(bool)*num_vertices);
	_num_edges = -1;
}

void Graph::fill_diagonal(bool value)
{
	FORix(num_vertices)
		//mat(ix,ix) = 0;
		data[ix][ix] = value;
}

void Graph::print(string str_prefix) const
{
	print2(str_prefix);
}

void Graph::print2(string str_prefix) const
{
	std::cout << get_string(str_prefix);
}

std::string Graph::get_string(string str_prefix) const
{
	std::ostringstream oss_graph;
	oss_graph << str_prefix << std::endl;
	for(int row=0; row<num_vertices; row++) {
		for(int col=0; col<num_vertices; col++) {
			if(col<=row) {
				oss_graph << "- ";
			} else {
				//std::cout << mat(row, col) << " ";
				//oss_graph << (data[row][col] ? "1" : "0") << " ";
				oss_graph << data[row][col]  << " ";
			}
		}
		oss_graph << std::endl;
	}
	oss_graph << std::endl;
	return oss_graph.str();
}

Graph* Graph::clone() const
{
	Graph* graph_new = new Graph(num_vertices);
	FORix(num_vertices) {
		memcpy(graph_new->data[ix], data[ix], num_vertices*sizeof(bool));
	}
	graph_new->graph_id = graph_id;
	graph_new->_num_edges = _num_edges;
	return graph_new;
}

bool Graph::is_edge(int ix, int jx) const
{
	//return (mat(ix,jx) == 1);
	return data[ix][jx] || data[jx][ix];
}

// why all this pain
// check if graph id is all ones
// if graph id is not present, then either
// (a) compute the graph id and check graph_id ==  1 << sz_upper_triangle
// (b) iterate thro' the upper triangle and check for all ones
bool Graph::is_k_n(long long certificate) const
{
	if(certificate == -1) {
		//construct_graph_id(); this is not a constant function
		// certificate = graph_id;
		return false;
	}
	int sz_upper_triangle = num_vertices * (num_vertices - 1) / 2;
	int graph_id_k_n = pow(2, sz_upper_triangle) - 1; // perf: "1 << sz_upper_triangle"
	return (graph_id_k_n == certificate);
}

bool Graph::is_tree(bool b_connected=false) const
{
	b_connected |= connected();
	int num_edges = 0;
	for(int row=0; row<num_vertices; row++) {
		for(int col=row+1; col<num_vertices; col++) {
			num_edges += (is_edge(row,col)) ? 1 : 0; // is this slow?
		}
	}
	return num_edges == (num_vertices-1);
}

bool Graph::connected() const
{
	// Matrix<bool, 1, Dynamic> reached;
	// reached.resize(1, num_vertices);
	bool *reached = new bool[num_vertices];
	FORix(num_vertices) reached[ix] = false;
	
	reached[0] = true;
	update_reachability(0, reached);

	// reduce pattern
	bool result = true;
	FORix(num_vertices) {
		if(!reached[ix]) {
			result = false;
			break;
		}
	}
	delete[](reached);
	return result;
}

void Graph::update_reachability(int vertex, bool *reached) const
{
	FORix(num_vertices) {
		//if(mat(vertex,ix)==1 && !reached[ix]) {
		if(data[vertex][ix] && !reached[ix]) {
			reached[ix] = true;
			update_reachability(ix, reached);
		}
	}
}

int Graph::number_of_edges()
{
	if(_num_edges != -1)
		return _num_edges;

	_num_edges = 0;
	FORix(num_vertices) {
		for(int jx=ix+1; jx<num_vertices; jx++) {
			//num_edges += mat(ix, jx);
			_num_edges += data[ix][jx] ? 1 : 0;
		}
	}
	return _num_edges;
}


// let num_vertices = 6
// [
//  0.  1.  2.  3.  4.  5.  
//  0.  *                        
//  1.  #   *                     
//  2.  #   #   *   X                 
//  3.  #   #   #   *
//  4.  #   #   #   #   *           
//  5.  #   #   #   #   #   *         
//  ]
// here:
// # - don't care since they are part of the lower triangle
// * - diagonal entries
// X - marks the spot for which we have to find the position from LSB
//
// graph id is a binary string formed from the uppoer triangle
// with the entry (0, 0) being the MSB and entry (n-2, n-1) being the LSB.
// Now add we need to add the edge (2,4) i.e., row=2, col=4.
// First, we find the position from the MSB as follows:
// the "absolute" position in the column major order for (row, col) is
// position_absolute = row*num_vertices + col.
// Since we are going to discard the lower triangular entries,
// we have to subtract (row+1)*(row+2)/2 from the absolute position.
// => position_from_msb = position_absolute - (row+1)*(row+2)/2
//                      = row*num_vertices + col - (row+1)*(row+2)/2
// Now, finding the position from LSB is obtained by subtracting
// position_from_msb from position of the MSB from LSB. The latter
// term is nothing but the number of bits in the graph_id for
// num_vertices sized graph, which is num_vertices*(num_vertices-1)/2
//
// Now adding an edge is obtained by performing logical OR of the
// binary with zeros in all positions except position_from_lsb with
// the graph_id.
void Graph::add_edge(int row, int col)
{
	set_edge(row, col);
	int position_from_msb = row*num_vertices + col - (row+1)*(row+2)/2;

	long long diff = 1;
	// repeating
	// int n_upper_triangular = num_vertices*(num_vertices-1)/2;
	// int n_upper_triangular = Graph::num_upper_triangular(num_vertices);
	int n_upper_triangular = Graph::NUM_UPPER_TRIANGULAR[num_vertices];
	// why is this a for loop?
	FORix(n_upper_triangular - position_from_msb - 1) {
		diff <<= 1;
 	}
	graph_id |= diff;
	_num_edges++;
}

void Graph::validate_row_col(int row, int col, std::string requestor) const
{
	if(row>=0 || row<num_vertices ||
	   col>=0 || col<num_vertices)
		return;
	
	std::ostringstream oss_err;
	oss_err <<
		"graph/" << requestor << ": " <<
		"(" << row << "," << col << ") / " <<
		"(" << num_vertices << "," << num_vertices << ")";
	throw std::runtime_error(oss_err.str());
}

// Now removing an edge is obtained by performing logical AND of the
// binary with ones in all positions except position_from_lsb with
// the graph_id.
void Graph::remove_edge(int row, int col)
{
	validate_row_col(row, col, "remove-edge");
	// mat(row, col) = 0;
	// mat(col, row) = 0;
	data[row][col] = false; data[col][row] = false;
	int position =
		row*num_vertices + col - (row+1)*(row+2)/2;

	long long diff = 1;
	// repeating
	// int n_upper_triangular = num_vertices*(num_vertices-1)/2;
	// int n_upper_triangular = Graph::num_upper_triangular(num_vertices);
	int n_upper_triangular = Graph::NUM_UPPER_TRIANGULAR[num_vertices];
	// why is this a for loop?
	FORix(n_upper_triangular - position - 1) {
		diff <<= 1;
 	}
	diff = ~diff;
	graph_id &= diff;
	_num_edges--;

	// for string
	// graph_id.replace(position,1,"0");
}

void Graph::set_edge(int row, int col)
{
	validate_row_col(row, col);
	// mat(row, col) = 1;
	// mat(col, row) = 1;
	data[row][col] = true; data[col][row] = true;
}

void Graph::reset_edge(int row, int col)
{
	validate_row_col(row, col);
	data[row][col] = false;
	data[col][row] = false;
}

void Graph::reset_vertex(int ix_vertex)
{
	// reset the ix_vertex-th row
	for(int col=ix_vertex+1; col<num_vertices; col++)
		reset_edge(ix_vertex, col);
	// reset the ix_vertex-th col
	for(int row=0; row<ix_vertex; row++)
		reset_edge(row, ix_vertex);
	
	_num_edges = -1;
	number_of_edges();
	construct_graph_id();
}

void Graph::copy_edges_from(Graph const* src, int ix_vertex)
{
	// copy the ix_vertex-th row
	for(int col=ix_vertex+1; col<num_vertices; col++) {
		if(src->is_edge(ix_vertex, col))
			set_edge(ix_vertex, col);
		else
			reset_edge(ix_vertex, col);
	}
	
	// reset the ix_vertex-th col
	for(int row=0; row<ix_vertex; row++) {
		if(src->is_edge(row, ix_vertex))
			set_edge(row, ix_vertex);
		else
			reset_edge(row, ix_vertex);
	}
	
	_num_edges=-1;
	number_of_edges();
	construct_graph_id();
}

void Graph::copy_edges_from(Graph const* src, std::vector<int> vertices)
{
	int parent_src, child_src;
	for(int ix_parent=0; ix_parent<vertices.size(); ix_parent++) {
		parent_src = vertices[ix_parent];
		for(int ix_child=ix_parent+1; ix_child<vertices.size(); ix_child++) {
			child_src = vertices[ix_child];
			if(src->is_edge(parent_src, child_src))
				set_edge(ix_parent, ix_child);
			else
				reset_edge(ix_parent, ix_child);
		}
	}
	
	_num_edges=-1;
	number_of_edges();
	construct_graph_id();
}

void Graph::copy(Graph* graph_other) const
{
	if(graph_other->num_vertices != num_vertices)
		throw "fuck you";
	for(int row=0; row<num_vertices; row++) {
		for(int col=row+1; col<num_vertices; col++) {
			if(is_edge(row, col))
				graph_other->set_edge(row, col);
			else
				graph_other->reset_edge(row, col);
		}
	}
}

// void
// Graph::copy(ga::Graph *dst, ga::Block *block) const
// {
// 	if(block->size() != dst->num_vertices)
// 		throw "graph dst != block";
// 	for(int vertex : *block) {
// 		if(vertex >= num_vertices)
// 			throw "block has more vertices";
// 	}
// 
// 	int ix_row_dst=0, ix_col_dst;
// 	for(int row : *block) {
// 		ix_col_dst = 0;
// 		for(int col : *block) {
// 			if(is_edge(row, col))
// 				dst->set_edge(ix_row_dst, ix_col_dst);
// 			ix_col_dst++;
// 		}
// 		ix_row_dst++;
// 	}
// 	dst->_num_edges = -1;
// 	dst->number_of_edges();
// 	dst->construct_graph_id();
// }

void Graph::copy(Graph* dst, VectorFixed* vertices) const
{
	if(vertices->size() > dst->num_vertices)
		throw "graph-dst has less vertices";
	for(int ix_vertex : *vertices) {
		if(ix_vertex >= num_vertices)
			throw "graph-src has less vertices";
	}
	
	int ix_dst=0, ix_dst2;
	for(int ix_vertex : *vertices) {
		ix_dst2=0;
		for(int ix_vertex2 : *vertices) {
			//if(at(ix_vertex, ix_vertex2)==1) {
			if(is_edge(ix_vertex, ix_vertex2)) {
				dst->set_edge(ix_dst, ix_dst2);
			}
			++ix_dst2;
		}
		++ix_dst;
	}
	dst->_num_edges = -1;
	dst->number_of_edges();
	dst->construct_graph_id();
}

std::vector<int> Graph::get_degrees() const
{
	std::vector<int> degrees(num_vertices, 0);
	int i_degree;
	for(int ix_row=0; ix_row<num_vertices; ix_row++) {
		for(int ix_col=ix_row+1; ix_col<num_vertices; ix_col++) {
			if(is_edge(ix_row, ix_col)) {
				degrees[ix_row] += 1;
				degrees[ix_col] += 1;
			}
		}
	}
	return degrees;
}

std::vector<int> Graph::get_cut_vertices() const
{
	bool *visited    = (bool*) calloc(num_vertices, sizeof(bool));
	int *discovered  = (int*)  calloc(num_vertices, sizeof(int));
	int *lows        = (int*)  calloc(num_vertices, sizeof(int));
	int *parents     = (int*)  calloc(num_vertices, sizeof(int));
	bool *art_points = (bool*) calloc(num_vertices, sizeof(bool));
	FORix(num_vertices) {
		discovered[ix] = std::numeric_limits<int>::min();
		lows[ix] = std::numeric_limits<int>::min();
		parents[ix] = -1;
	}
	int time_cut_vertex = 0;

	for(int vertex=0; vertex < num_vertices; vertex++) {
		if(visited[vertex] == false)
			get_cut_vertices_helper(vertex,
									visited, discovered, lows, parents,
									art_points, time_cut_vertex);
	}

	std::vector<int> cut_vertices;
	for(int vertex=0; vertex<num_vertices; vertex++) {
		if(art_points[vertex])
			cut_vertices.push_back(vertex);
	}
	std::cout << "graph:get-cut-vertices: " << cut_vertices.size() << std::endl;

	free(art_points);
	free(parents);
	free(lows);
	free(discovered);
	free(visited);
	return cut_vertices;
}

void Graph::get_cut_vertices_helper(int vertex,
									bool *visited, int *discovered,
									int *lows, int *parents,
									bool *art_points, int& time_cut_vertex) const
{
	int children = 0;
	visited[vertex] = true;

	discovered[vertex] = time_cut_vertex;
	lows[vertex] = time_cut_vertex;
	time_cut_vertex = time_cut_vertex + 1;

	for(int nbr=0; nbr<num_vertices; nbr++) {
		if(!is_edge(vertex, nbr))
			continue;

		if(visited[nbr] == false) {
			parents[nbr] = vertex;
			children++;

			get_cut_vertices_helper(nbr,
								  visited, discovered, lows, parents,
								  art_points, time_cut_vertex);

			lows[vertex] = min(lows[vertex], lows[nbr]);

			if((parents[vertex] == -1) && (children > 1))
				art_points[vertex] = true;

			if((parents[vertex] != -1) && (lows[nbr] >= discovered[vertex]))
				art_points[vertex] = true;
		} else {
			if(nbr != parents[vertex])
				lows[vertex] = min(lows[vertex], discovered[nbr]);
		}
	}
}


int Graph::get_max_degree(std::vector<int> degrees) const {
	if(degrees.size()==0)
		degrees = get_degrees();
	//for(auto deg : degrees) std::cout << deg << ","; NEWLINE;
	return *(std::max_element(degrees.begin(), degrees.end()));
}

Graph::~Graph() {
	FORix(num_vertices)
		free(data[ix]);
	free(data);
}
