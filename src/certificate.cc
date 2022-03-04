#include "certificate.hh"

#include "block.hh"
#include "partition_pool.hh"
#include "permutation_pool.hh"
#include "utils.hh"
#include "vertices.hh"

#include <iostream>

using namespace std;
using namespace ga;

Certificate::Certificate()
{
	
}

void log_b_s_t_u(int N, const Partition* B, const Blocks* S, const ga::Block* T, const ga::Block* U)
{
	cout << "\n\n**************\nrefine :: N: " << N << endl;
	B->print("refine:B");
	S->print("refine:S");
	cout << "refine :: ";
	T->print("T"); cout << " ";
	U->print("U"); cout << " ";
	cout << " subset?: " << T->proper_subset_of(U);
	cout << endl;
}	
void log_b_s(Partition* B, Blocks* S)
{
	cout << endl << "refine :: |B| != 1" << endl;
	B->print("refine :: B");
	S->print("refine :: S");
	cout << endl;
}
void log_refined(const Partition* A, const Partition* B)
{
	cout << "\n\n\n";
	cout << "=============================================================" << endl;
	cout << "original partition: "; A->print("A: ");
	cout << "refined equitable partition" << endl;
	B->print("B:");
	cout << "=============================================================" << endl;
}

Partition* Certificate::refine(const Partition *A) {
	// DEBUG2 A->print("refine:A");
	Partition* B = A->clone();
	// DEBUG2 B->print("refine:B ");
	
	Blocks* S = new Blocks();
	// S->append(B);
	B->append_to(S);
	
	// DEBUG2 S->print("refine:S ");
	S->reverse();

	// DEBUG2 S->print("refine:S ");

	//U = vertex list, a vector whose elements are 0...n
	int n = graph->num_vertices;
	// pool-me-once-shame-on-you
	// ga::Block* U = new ga::Block(0, n, 1);
	ga::Block* U = BlockPool::instances[n]->get();
	U->add_range(0, n, 1);

	int N = B->size_cached();
	ga::Block* T;
	int j;

	while(N > 0) {
		--N;
		T = S->pop();
		// DEBUG2 log_b_s_t_u(N, B, S, T, U);
		if (T->proper_subset_of(U)) {
			U->remove(T);
			j = 0;
			while(j < B->size_cached() && B->size_cached() < n) {
				if (B->size_cached() != 1) {
					// DEBUG2 log_b_s(B, S);
					split_update(B, S, U, T, j, N);
				}
				j++;
			}
		}
		// pool-me-once-shame-on-you
		// delete(T);
		T->free_element();
	}
	// DEBUG2 log_refined(A, B);

	// pool-me-once-shame-on-you
	delete(S);
	// delete(U);
	U->free_element();
	//delete(B);
	return B;
}
//abstract-it-out
void log_j_n_t(int j, int N, ga::Block* T)
{
	cout << "su: updating L ... j: " << j;
	cout << " N: "<< N << endl;
	T->print("su :: T");
	cout<< endl;
}

Partition* Certificate::create_and_update_L(const Graph* graph, ga::Block* block, ga::Block* T)
{
	// DEBUG2 block->print("su:create-update-L:block");
	// DEBUG2 NEWLINE;
	// Partition* L = new Partition(graph->num_vertices);
	Partition* L = PartitionPool::instances[graph->num_vertices]->get();
	// DEBUG2 L->print("su:create-update-L");
	int h;
	for(const auto u : *block) {
		// h = | T n N_G(u)|
		h = T->intersect(graph, u);
		// DEBUG2 cout << "su:u- " << u  << " su:h- " << h << std::endl;
		// L[h] << u
		L->at(h)->add_element(u);
		// DEBUG2 NEWLINE;
	}
	L->update_size();
	return L;
}

void Certificate::split_update(Partition *B, Blocks *S, ga::Block *U, ga::Block* T, int &j, int &N)
{
	// DEBUG2 std::cout << "entering su with j=" << j << std::endl;
	// DEBUG2 S->print("su:: S");
	
	L = create_and_update_L(graph, B->at(j), T);

	// DEBUG2 NEWLINE;
	// DEBUG2 log_j_n_t(j, N, T);

	// if L has one non-empty block, dont have to do anything
	// int m = L->non_empty_blocks();
	int m = L->size_cached();
	// DEBUG2 L->print("su:: L");
	// DEBUG2 cout << "su:: #non-empty blocks(m): " << m << endl;
	if (m<=1) {
		// pool-me-once-shame-on-you
		// delete(L);
		L->free_element();
		return;
	}

	// shift B by (m-1) to make some space in positions (j+1)..(|B|-1)
	
	B->shift(j,m-1);
	// DEBUG2 B->print("su:: B");
	// B->update_size();
	S->increase_by(m);
	// DEBUG2 S->print("su:: S");

	int k = 0;
	for(auto block : L->blocks) {
		if(block->size() == 0)
			continue;

		// block->print("su:assign"); NEWLINE;
		B->assign(j+k,  block, "B");
		// B->update_size();
		S->assign(N+m-1-k,  block, "S");
		U->append(block);
		k++;
		// DEBUG2 NEWLINE;
	}
	B->update_size();
	if(B->blocks.size() != graph->num_vertices)
		throw std::runtime_error("B is screwed!!");
	// DEBUG2 B->print("su:B");
	// DEBUG2 S->print("su:S");
	j = j+m-1;
	N = N+m;
	
	// delete(L);
	L->free_element();
}

int Certificate::refine_and_find_l(Partition* p, Partition* q)
{
	return 1;
}

int Certificate::number_of_vertices()
{
	return graph->num_vertices;
}

void Certificate::update_automorphism(Partition *q)
{
	// set pi2
	// remember, q == pi_1
	// for i in (0..n-1): pi_2[pi_1[i]] = Best[i]
	// pool-me-once-shame-on-you
	// Permutation *pi_2 = new Permutation(number_of_vertices());
	Permutation *pi_2 = PermutationPool::instances[number_of_vertices()]->get();
	FORix(graph->num_vertices) {
		// pool-me-once-shame-on-you
		pi_2->add_mapping(q->at(ix)->head(), mu2->at(ix)->head());
	}
	// enter2(pi_2, g)
	sch->print("update automorphism-b4::sch");
	sch->enter2(pi_2);
	sch->print("update automorphism-l8r::sch");
	// cleanup
	// delete pi_2
	// delete(pi_2);
}

void Certificate::handle_discrete_q(Partition *q, Partition::PartCompare result)
{
	// if not BestExist
	if (mu2 == NULL)
		// pool-me-once-shame-on-you
		mu2 = q->clone();
	else {
		if (result == Partition::BETTER) {
			// pool-me-once-shame-on-you
			// delete(mu2);
			mu2->free_element();
			
			// pool-me-once-shame-on-you
			mu2 = q->clone();
			// DEBUG1 mu2->print("better");
		}
		else if (result == Partition::EQUAL) {
			num_automorphs++;
			update_automorphism(q);
		}
	}
	// DEBUG2 std::cout << "handle_discrete_q :: fin" << std::endl;
}

void Certificate::initialize_c_d(ga::Block *c, ga::Block *d, Partition *q, int l)
{
	
}

Partition* Certificate::initialize_r(Partition *q, int l)
{
	// pool-me-once-shame-on-you
	// Partition *r = new Partition();
	Partition *r = PartitionPool::instances[graph->num_vertices]->get();
	
	// refactor
	// DEBUG2 q->print("init-r:q");
	// DEBUG2 std::cout << "init-r:l " << l << std::endl;
	for(int ix=0; ix<l; ix++) {
		r->set(ix, q->at(ix)->clone());
	}
	for(int ix=(l+1); ix<q->size(); ix++)
		r->set(ix+1, q->at(ix)->clone());
	// refactor
	// pool-me-once-shame-on-you
	// r->set(l,   new ga::Block(graph->num_vertices)); // hack
	// r->set(l,   BlockPool::instance.get()); // hack
	// pool-me-once-shame-on-you
	// r->set(l+1, new ga::Block(graph->num_vertices)); // hack
	// r->set(l+1,   BlockPool::instance.get()); // hack
	// DEBUG2 r->print("cert/init-r: done");
	return r;

}

void Certificate::update_r(Partition *r, int l, int u, ga::Block *d)
{
	r->at(l)->reset();
	r->at(l+1)->reset();
	
	r->at(l)->set(u);
	//r->assign(l, u, "r");
	// d_except_u =  d->except(u);
	r->set(l+1, d->except(u), "r");
	// DEBUG2 r->print("canon2:update_r:r");
}

Permutation* Certificate::initialize_beta_prime(Partition *q, int l, Partition *r)
{
	/// perf-benchmark
	// Permutation *beta_prime = new Permutation(number_of_vertices());
	Permutation *beta_prime = PermutationPool::instances[number_of_vertices()]->get();
	/// perf-benchmark
	bool present[number_of_vertices()];
	FORix(number_of_vertices())
		present[ix] = false;

	/// perf-benchmark
	FORjx(l+1) {
		beta_prime->add_mapping(jx, r->at(jx)->head());
		present[jx] = true;
	}
	
	// todo: for each y not in {beta_prime[i]}[i=0..l]
	// j++; beta_prime[j] = y
	int j=l;
	/// perf-benchmark
	FORix(number_of_vertices()) {
		if(present[ix] == false) {
			beta_prime->add_mapping(++j, ix);
		}
	}
	if(beta_prime->size() != number_of_vertices()) {
		std::ostringstream oss_err;
		oss_err << "|beta prime| = " << beta_prime->size() <<
			", num-vertices: " << number_of_vertices();
		throw std::runtime_error(oss_err.str());
	}
	
	// DEBUG2 cout << "num vertices: " << number_of_vertices() << endl;
	// DEBUG2 beta_prime->print("beta prime");

	return beta_prime;
}

Partition* Certificate::process_element_of_c(int u, Partition *r, int l, Partition *q, ga::Block *d)
{
	// DEBUG2 cout << "canon2:process-c:u " << u << endl;
	update_r(r, l, u, d);
	mu2 = canon2(r);
	// DEBUG2 mu2->print("canon2:process-c:mu2");
	Permutation* beta_prime = initialize_beta_prime(q, l, r);
	// DEBUG2 sch->print("canon2:process-c::old_sch");
	Schreiersims::change_base(sch, beta_prime, &sch);
	// DEBUG2 sch->print("canon2:process-c:new_sch");
	// todo:
	// for each g in U_l of sch
	// c = c \ g(u)
	// delete beta_prime;
	return mu2;
}

Partition* Certificate::handle_result_not_worse(Partition *q, int l)
{
	ga::Block *c, *d;
	//initialize_c_d(c, d, q, l);
	c = q->at(l)->clone();
	d = q->at(l)->clone();
	// DEBUG2 std::cout << "l: " << l << std::endl;
	// DEBUG2 c->print("handle_!worse::c");
	// DEBUG2 NEWLINE;
	// DEBUG2 d->print("handle_!worse::d");
	// DEBUG2 NEWLINE;

	// initialize r
	Partition *r = initialize_r(q, l);
	r->num_non_empty_elements = q->num_non_empty_elements+1;
	int u;
	while( c->size() > 0) {
		u = c->head();
		// DEBUG2 cout << "handle_not_worse::u " << u << endl;
		mu2 = process_element_of_c(u, r, l, q, d);
		// DEBUG2 sch->print("handle_!worse::new_sch");
		// for each g in U_l
		// DEBUG2 c->print("handle_not_worse::c-b4-update");
		for(auto g : *sch->at(l)) {
			// c = c \ g(u)
			c->remove(g->at(u));
		}
		// DEBUG2 cout << "handle_not_worse::u " << u << endl;
		// DEBUG2 c->print("handle_not_worse::c-af-update");
		// DEBUG2 NEWLINE;
	}
	// pool-me-once-shame-on-you
	// delete(c); delete(d);
	c->free_element(); d->free_element();

	// pool-me-once-shame-on-you
	// delete(r);
	r->free_element();
	
	return mu2;
}

void Certificate::canon2_log_p_q_l(const Partition* p, const Partition* q, int l)
{
	// DEBUG2 p->print("canon2::p");
	// DEBUG2 q->print("canon2::q");
	// DEBUG2 cout << "canon2/l: " << l << endl;
}

Partition* Certificate::canon2(const Partition *p)
{
	// DEBUG2 p->print("\n\n\n => canon2:p");
	Partition *q = refine(p);
	int l = q->index_non_unit_block();

	canon2_log_p_q_l(p, q, l);
	
	Partition::PartCompare result = Partition::BETTER;
	// DEBUG2 cout << "canon2::mu2.null? " << (mu2 == NULL) << endl;
	if (mu2 != NULL) // if BestExist
		result = q->compare(graph, mu2);
	// DEBUG2 cout << "canon2::result: " << result << endl;

	// DEBUG2 cout << "q has n blocks? " << (q->size() == graph->num_vertices) << endl;
	// if Q has n blocks
	if (q->size() == graph->num_vertices) {
		handle_discrete_q(q, result);

		// pool-me-once-shame-on-you
		//delete(q);
		q->free_element();
		
		return mu2;
	}
	
	// DEBUG2 sch->print("canon2::sch");
	if (result != Partition::WORSE) {
		mu2 = handle_result_not_worse(q, l);
	}
	// pool-me-once-shame-on-you
	// delete(q);
	q->free_element();
	
	return mu2;
}

void Certificate::init_schreiersims(int num_vertices)
{
	// sch = new Schreiersims();
	// sch->initialize_elements(num_vertices);
	// sch->id = 0;
	// sch->set_base(Permutation::identity(num_vertices));
	num_automorphs = 0;
	sch = new Schreiersims(num_vertices, Permutation::identity(num_vertices));
	// DEBUG2 sch->print("init_schreiersims::sch");
}



long Certificate::generate_certificate2(const Graph *g)
{
	return std::get<0>(generate_cert_and_automorph(g));
}

std::tuple<long long, int> Certificate::generate_cert_and_automorph(const Graph* g)
{
	// DEBUG2 g->print("\n\n\n\n\n\n\n\n\n\n\n\n\n\ncert:gen\n\n\n");
	int n = g->num_vertices;

	// init: schreiersims, mu, graph
	init_schreiersims(n);
	mu2 = NULL;
	add_graph(g);
	
	Partition *p = Partition::create_default(n);
	Partition *mu = canon2(p);
	// DEBUG2 mu->print("mu");
	// DEBUG2 NEWLINE;
	// delete(p);
	p->free_element();
	
	int k = 0;
	long C = 0;
	for(int jx=n-1; jx>=1; jx--) {
		for(int ix=jx-1; ix>=0; ix--) {
			//if (graph->at(mu->at(ix)->head(), mu->at(jx)->head())==1)
			if (graph->is_edge(mu->at(ix)->head(), mu->at(jx)->head()))
				C += pow(2,k);
			++k;
		}
	}
	// DEBUG2 cout << "certificate:: " << C << std::endl;
	
	// delete(mu2);
	mu2->free_element();
	
	delete(sch);
	return std::make_tuple(C, num_automorphs);
}

Partition* Certificate::canon1(Partition *p, Partition *mu)
{
	
	// DEBUG2 p->print("canon1::p");
	Partition *q = refine(p);
	
	// DEBUG2 q->print("canon1::q");
	int l = q->index_non_unit_block();
	// DEBUG2 std::cout << "canon1::l - " << l;
	// DEBUG2 NEWLINE;
	Partition::PartCompare result = Partition::BETTER;

	// DEBUG2 cout << "canon1::mu.null? " << (mu == NULL);
	// DEBUG2 NEWLINE;
	if (mu != NULL) // if BestExist
		result = q->compare(graph, mu);
	// DEBUG2 cout << "canon1::result: " << result;
	// DEBUG2 NEWLINE;

	// DEBUG2 cout << "canon1::q->size - " << q->size();
	// DEBUG2 cout << " n: " << graph->num_vertices;
	// DEBUG2 NEWLINE;
	
	if (q->size() == graph->num_vertices) {
		if (mu == NULL) // if not BestExist
			mu = q->clone();
		else if (result == Partition::BETTER) {
			// delete(mu);
			mu->free_element();
			mu = q->clone();
			// DEBUG1 mu->print("better");
		}
		// delete(q);
		q->free_element();
		
		return mu;
	}

	if (result != Partition::WORSE) {
		ga::Block *c = q->at(l)->clone();
		ga::Block *d = q->at(l)->clone();
		// DEBUG2 c->print("canon1::c");
		// DEBUG2 NEWLINE;
		// DEBUG2 d->print("canon1::d");
		// DEBUG2 NEWLINE;

		// initialize r
		// Partition *r = new Partition();
		Partition *r = PartitionPool::instances[graph->num_vertices]->get();
		r->reserve(q->size()+1);
		for(int ix=0; ix<l; ix++) {
			r->set(ix, q->at(ix)->clone());
		}
		for(int ix=(l+1); ix<q->size(); ix++)
			r->set(ix+1, q->at(ix)->clone());
		r->assign(l,   -1); // hack
		r->assign(l+1, -1); // hack

		int u;
		// ga::Block *d_except_u;
		while( c->size() > 0) {
			u = c->pop_front();
			// DEBUG2 cout << "canon1:u " << u;
			// DEBUG2 NEWLINE;
			r->assign(l, u, "");
			// DEBUG2 NEWLINE;
			// d_except_u =  d->except(u);
			r->set(l+1, d->except(u), "");
			// DEBUG2 NEWLINE;
			// DEBUG2 r->print("canon1::r");
			mu = canon1(r, mu);
			// delete(d_except_u);
			//delete(r->at(l)); delete(r->at(l+1));
		}
		
		// delete(r);
		r->free_element();
		
		// pool-me-once-shame-on-you
		// delete(c); delete(d);
		c->free_element(); d->free_element();
	}
	
	// delete(q);
	q->free_element();
	
	return mu;
}

// long Certificate::generate_certificate(Graph *g)
// {
// 	add_graph(g);
// 	int n = g->num_vertices;
// 
// 	Partition *p = initialize_p(n);
// 	Partition *mu = canon1(p);
// 	// mu->print_clean();
// 	// DEBUG mu->print("mu");
// 	// DEBUG NEWLINE;
// 	
// 	// delete(p);
// 	p->free_element();
// 	
// 	int k = 0;
// 	long long C = 0;
// 	for(int jx=n-1; jx>=1; jx--) {
// 		for(int ix=jx-1; ix>=0; ix--) {
// 			// if (graph->at(mu->at(ix)->head(), mu->at(jx)->head())==1)
// 			if (graph->is_edge(mu->at(ix)->head(), mu->at(jx)->head()))
// 				C += pow(2,k);
// 			++k;
// 		}
// 	}
// 	
// 	// delete(mu);
// 	mu->free_element();
// 	
// 	return C;
// }

Partition* Certificate::initialize_p(int n)
{
	// DEBUG2 std::cout << "initialize_p";
	// DEBUG2 NEWLINE;
	
	// Partition *p = new Partition();
	Partition *p = PartitionPool::instances[graph->num_vertices]->get();
	
	// ga::Block *block1 = new ga::Block();
	ga::Block *block1 = BlockPool::instances[n]->get();
	// DEBUG2 cout << "graph size: " << graph->num_vertices << endl;
	block1->add_range(0, n);
	p->add_block(block1);
	// delete(block1);
	block1->free_element();
	// DEBUG2 p->print("initialize_p::p");
	return p;
}

void Certificate::add_graph(const Graph *g)
{
	// DEBUG2 std::cout << "adding graph" << std::endl;
	graph = g;
}

Certificate::~Certificate()
{
}
