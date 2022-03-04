// #include <iostream>
// int main()
// {
//   MatrixXd m(2,2);
//     m(0,0) = 3;
//       m(1,0) = 2.5;
//         m(0,1) = -1;
//           m(1,1) = m(1,0) + m(0,1);
//         cout << m << endl;}

// inserting into a vector
#include <functional>
#include <iostream>
#include <vector>
#include <omp.h>
//#include <Eigen/Dense>

#include "certificate.hh"
#include "combination_generator.hh"
#include "block.hh"
#include "permutation.hh"
#include "blocks.hh"
#include "graph.hh"
#include "v_pool.hh"
#include "partition_pool.hh"
#include "permutation_pool.hh"
#include "unique_certificates.hh"
#include "find_connected_graphs.hh"
#include "chromatic_index.hh"
#include "chromatic_index_helper.hh"

#include <bitset>

#define BACKWARD_HAS_BFD 1
#include "backward.hpp"

#include "csv_iterator.hh"
#include "find_connected_components.hh"
#include "partition.hh"
#include "permutation.hh"
#include "vector_int_pool.hh"

//using Eigen::MatrixXd;
using namespace std;
using namespace ga;

void print(std::vector<int> myvector)
{
	for (std::vector<int>::iterator it=myvector.begin(); it<myvector.end(); it++)
		std::cout << ' ' << *it;
	std::cout << '\n';
}
typedef typename std::vector<int>::size_type size_type;

#include <queue>
#include <functional>
#include "generic_object.hh"

class KaajaPool;
	
// class Kaaja : public GenericObject {
// public:
// 	virtual void init_element() override { cout << "kaaja/init" << endl; }
// 	virtual void reset_element() override { cout << "kaaja/reset" << endl; }
// 	friend class KaajaPool;
// protected:
// 	static long id_g;
// 	Kaaja() {
// 	}
// 	Kaaja(int _id_inc) {
// 		id_g += _id_inc;
// 		id = id_g;
// 		std::cout << "yo kaaja #" << id << std::endl;
// 	}
// };	
	
// class KaajaPool : public VPool {
// public:
// 	Kaaja* get() {
// 		return (Kaaja*)VPool::get();
// 	}
// 	KaajaPool(int _id_inc) {
// 		id_inc = _id_inc;
// 	}
	
// protected:
// 	Kaaja* create_element() {return new Kaaja(id_inc); }
// private:
// 	int id_inc;
// };


using namespace ga;

//int VPool_Element::id_vpe = 0;
// long Kaaja::id_g = 0;
int ga::Block::id_g = 0;
int Partition::id_g = 0;

size_t Permutation::id_g = 0;
size_t Permutation::count = 0;
size_type Blocks::id_g = 0;
size_type Blocks::count = 0;
vector<long> Graph::NUM_UPPER_TRIANGULAR = vector<long>();

const size_type BlockPool::MIN_CAPACITY = 10;

long long Schreiersims::id_g = 0;
vector<int> Schreiersims::stats_size = vector<int>(100, 0);
vector<int> Schreiersims::stats_capacity = vector<int>(100, 0);

BlockPool**       BlockPool::instances       = (BlockPool**)       malloc(3 * sizeof(BlockPool*));
PartitionPool**   PartitionPool::instances   = (PartitionPool**)   malloc(3 * sizeof(PartitionPool*));
PermutationPool** PermutationPool::instances = (PermutationPool**) malloc(3 * sizeof(PermutationPool*));

int main ()
{
	// for handling exceptions and printing stacktraces
	std::vector<int> signals;
	signals.push_back(SIGSEGV);
	signals.push_back(SIGABRT);
	backward::SignalHandling sh(signals);
	std::cout << std::boolalpha << "sh.loaded() == " << sh.loaded() << std::endl;

	
	/*KaajaPool *kp = new KaajaPool(5);
	{
		Kaaja* kaaja = kp->get();
		std::cout << "using kaaja#" << kaaja->id << std::endl;
		kaaja->free_element();
		// std::cout << vpe._id << std::endl;
		// std::cout << "before: add vpe: " << &vpe << std::endl;
		// vpe._id = 55;
		// std::cout << "after: add vpe: " << &vpe << std::endl;
		// std::cout << vpe._id << std::endl;
		// std::cout << vpe.in_use << std::endl;
		// std::cout << vpe.pooled << std::endl;
		// vpe.free_element();
		// std::cout << "after: add vpe: " << &vpe << std::endl;
		// std::cout << "after vp->free(vpe)" << std::endl;
	}
	// vpe = new VPool_Element(20);
	// delete(vpe);
	std::cout << "deleting(kp)" << std::endl;
	delete(kp);
	std::cout << "delete(kp)" << std::endl;
	BlockPool::instance.clear();
	*/

	/** playing with vector insert
	std::vector<int> myvector (3,100);
	print(myvector);
	std::vector<int>::iterator it;
	
	it = myvector.begin();
	it = myvector.insert ( it , 200 );
	print(myvector);

	myvector.insert (it,2,300);
	print(myvector);

	// "it" no longer valid, get a new one:
	it = myvector.begin();

	std::vector<int> anothervector (2,400);
	myvector.insert (it+2,anothervector.begin(),anothervector.end());
	print(myvector);

	int myarray [] = { 501,502,503 };
	myvector.insert (myvector.begin(), myarray, myarray+3);

	std::cout << "myvector contains:";
	print(myvector);
	**/

	/** printing row of Eigen Matrix
	MatrixXd m(2,2);
	m(0,0) = 3;
	m(1,0) = 2.5;
	m(0,1) = -1;
	m(1,1) = m(1,0) + m(0,1);
	cout << m << endl;
	
	MatrixXd::RowXpr row1 = m.row(1);
	cout << row1 << endl;
	cout << row1(0) << ", " << row1(1) << endl;
	row1(0) = 300;
	cout << row1(0) << ", " << row1(1) << endl;
	cout << m << endl;
	**/

	/* have a bitmap and a wrapping iterator

	Bitmap* bm = new Bitmap(10);
	bm->print();
	bm->set(1);
	bm->set(3);
	bm->set(4);
	bm->set(7);
	bm->print();
	NEWLINE; NEWLINE;

	std::cout << "old: {";
	for(Bitmap::iterator i = bm->begin(); i != bm->end(); i++)
		std::cout << *i << ",";
	std::cout << "}" << std::endl;
	std::cout << "new: {";
	for(auto i : *bm)
		std::cout << i << ",";
	std::cout << "}" << std::endl;
	std::cout << "capacity: " << bm->capacity() << std::endl;
	std::cout << "size: " << bm->size() << std::endl;
	delete(bm);
	*/

	// int n=5;
	// CombinationIterator *cgi = new CombinationIterator(n,2);
	// while(cgi->next())
	// 	cgi->print();
	// delete(cgi);


	/**
	   csv-iterator
	std::ifstream file("plop.csv");

    for(CSVIterator loop(file);loop != CSVIterator();++loop)
		{
			std::cout << "4th Element(" << (*loop)[3] << ")\n";
		}
	**/

	/**
	Graph* graph = new Graph(9);
	// thi is for tests
	// graph->set_edge(0,1);	graph->set_edge(4,5); graph->set_edge(4,6);
	// graph->set_edge(1,2);	graph->set_edge(5,7);
	// 						graph->set_edge(7,8);
	// this is a bug
	// 0 0 1 0
	// 0 0 1 1
	// 1 1 0 1
	// 0 1 1 0
	graph->set_edge(0,2);
	graph->set_edge(1,2);
	graph->set_edge(1,3);
	graph->set_edge(2,3);

	std::vector<Graph*> components = FindConnectedComponents::for_graph(graph);
	std::cout << "# conn-components : " << components.size() << " "; NEWLINE;
	for(auto comp : components) {
		std::cout << comp->
	}
	for(auto comp : components)
		delete(comp);
	
	delete(graph);
	**/

	/**
	VectorIntPool *vi_pool = new VectorIntPool(5);
	FORix(vi_pool->size()) {
		std::cout << vi_pool->get(ix).capacity()  << ", ";
		vi_pool->get(ix).reserve(10);
		std::cout << vi_pool->get(ix).capacity()  << ", ";
	}
	NEWLINE;
	std::cout << "Del me"; NEWLINE;
	delete(vi_pool);
	**/
	

	/* of fig 7.6
	int num_vertices = 8;
	Graph* graph = new Graph(num_vertices);
	cout << "min capacities :" << endl;
	cout << "bpool: " << BlockPool::MIN_CAPACITY << endl;
	cout << "ppool: " << PartitionPool::MIN_CAPACITY << endl;

	cout << "min capacity() :" << endl;
	cout << "bpool: " << BlockPool::instance.min_capacity() << endl;
	cout << "ppool: " << PartitionPool::instance.min_capacity() << endl;

	// throw std::runtime_error("yo");
	
	BlockPool::instance.init_pool(num_vertices, 160);
	PartitionPool::instance.init_pool(graph->num_vertices, 10);

	// Partition* A1 = PartitionPool::instance.get(graph->num_vertices);
	// std::cout << "A: #" << A1->id << " in-use? " << A1->in_use << endl;
	// Partition* A2 = PartitionPool::instance.get(graph->num_vertices);
	// std::cout << "A: #" << A2->id << " in-use? " << A2->in_use << endl;

	
	std::cout << graph->num_vertices << std::endl;
	// outer cycle
	for(int ix=0; ix<7; ix++)	graph->set_edge(ix, ix+1);
	graph->set_edge(7, 0);
	// chords
	graph->set_edge(0, 3);
	graph->set_edge(1, 4);
	graph->set_edge(2, 6);
	graph->set_edge(5, 7);
	graph->print("fig 7.6\n"); NEWLINE;

	Partition* A = PartitionPool::instance.get(graph->num_vertices);
	Block1 *blk1, *blk2;
	blk1 = A->at(0);
	blk2 = A->at(1);
	
	// blk1 = new Block1(graph->num_vertices);
	// blk2 = new Block1(graph->num_vertices);
	// blk1 = BlockPool::instance.get(num_vertices);
	// blk2 = BlockPool::instance.get(num_vertices);
	
	blk1->set(0);
	for(int ix=1; ix<8; ix++) blk2->set(ix);
	// A->add_block(blk1);
	// A->add_block(blk2);
	A->print("pnd/A: ");

	Certificate* cert = new Certificate();
	cert->add_graph(graph);

	Partition* B = cert->refine(A);

	B->free_element();
	delete(cert);
	delete(graph);
	A->free_element();
	*/



	/*int in_vertices = 4;
	FindConnectedGraphs *fcg;
	Certificate *certificate = new Certificate();

	auto func_for_graph = [&] (Graph* g) {
		certificate->generate_certificate2(g);
		// std::cout << "cert: " << l_certificate << std::endl;
		//freq_counter_by_num_vertices[g->num_vertices].update(l_certificate);
	};
	
	BlockPool::instance.init_pool(in_vertices, 128);
	PartitionPool::instance.init_pool(in_vertices, 16);
	PermutationPool::instance.init_pool(in_vertices, 50);

	int ix_start=in_vertices, ix_end=in_vertices;
	Graph::init_upper_triangular(ix_end);
	fcg = new FindConnectedGraphs();

	// #pragma omp parallel
	{
		fcg->generate_parallel(ix_end, func_for_graph);
	}

	Graph* graph_fcc = new Graph(3);
	graph_fcc->set_edge(0, 1);
	graph_fcc->construct_graph_id();
	std::cout << "graph-id: " << graph_fcc->graph_id << " conn? " << graph_fcc->connected()  << std::endl;
	graph_fcc->print();
	std::vector<Graph*> conn_components = FindConnectedComponents::for_graph(graph_fcc);
	for(Graph* comp : conn_components) {
		comp->construct_graph_id();
		comp->print();
		std::cout << "#" << comp->graph_id << " n: " << comp->num_vertices << " conn? " << comp->connected() << std::endl;
	}

	delete(fcg);
	delete(certificate);
	*/

	int num_vertices = 8;
	
	// BlockPool::instance.init_pool(num_vertices, 128);
	// PartitionPool::instance.init_pool(num_vertices, 16);
	// PermutationPool::instance.init_pool(num_vertices, 50);
	Graph::init_upper_triangular(num_vertices+1);

	Graph* graph_ci = new Graph(num_vertices);
	FORix(num_vertices-1) graph_ci->set_edge(ix, ix+1);
	graph_ci->set_edge(num_vertices-1, 0);

	const int non_circ_edges = 7;
	int edges[2][non_circ_edges] = {  
		{1, 1, 2, 3, 3, 4, 4} , 
		{5, 6, 6, 7, 8, 6, 8}};
	FORix(non_circ_edges)
		graph_ci->set_edge(edges[0][ix]-1, edges[1][ix]-1);

	graph_ci->print2("");


	ChromaticIndex *ci = new ChromaticIndex();
	ci->ix_end = num_vertices;
	ci->init();
	ChromaticIndexHelper *cih = new ChromaticIndexHelper(ci);
	cih->init(num_vertices);

	int ci_graph = cih->find_exact_chromatic_index(graph_ci);
	std::cout << "ci: " << ci_graph; NEWLINE
	
	delete(ci); delete(cih);
	delete(graph_ci);
	
	return 0;
}
