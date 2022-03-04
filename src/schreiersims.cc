#include "schreiersims.hh"

#include <iostream>
#include <string>

#include "permutation_pool.hh"

using namespace ga;

/// perf-benchmark
Schreiersims::Permutations* Schreiersims::at(int ix)
{
	/// perf-benchmark, isnt the elements.size fixed in advance?
	/// based on num_vertices
	// if(ix >= elements.size()) {
	// 	ss_err << "ss:at(" << ix << ") / " << elements.size();
	// 	throw std::runtime_error(ss_err.str());
	// }
	if(ix >= num_elements) {
		std::ostringstream ss_err;
		ss_err << "ss:at(" << ix << ") / " << size();
		throw std::runtime_error(ss_err.str());
	}
	return &elements[ix];
}

long long Schreiersims::size() const
{
	return num_elements;
}

Permutation* Schreiersims::find(Schreiersims::Permutations permutations, int ix, int value)
{
	/// perf-benchmark
	for(auto permutation : permutations) {
		if(permutation->at(ix) == value)
			return permutation;
	}
	return NULL;
}

// dst = h^{-1} * g
// pi2 = h->inverse();
// pi3 = pi2->multiply(*g);
// pi3->copy_to(g);
void Schreiersims::test2_helper(Permutation *h, Permutation *g, Permutation* dst)
{
	// DEBUG3 h->print("ss:test2-hl: h");
	// DEBUG3 g->print("ss:test2-hl: g");
	int n = h->size();
	// compute inverse of h
	FORix(n) {
		scratch_buffer[h->mappings[ix]] = ix;
	}
	// compute inv(h) x g
	FORix(n) {
		dst->add_mapping(ix, scratch_buffer[g->mappings[ix]]);
	}
	// DEBUG3 dst->print("ss:test2-h: dst");
}

int Schreiersims::test2(Permutation *g, Permutation *beta, std::string str_level)
{
	int n = g->size(), x;
	Permutation *h; //, *pi2, *pi3;
	// DEBUG3 g->print2(str_level + "test2::g");
	// DEBUG3 beta->print2(str_level + "test2::beta");
	FORix(n) {
		x = g->at(beta->at(ix));
		// DEBUG3 cout << str_level + "test2: g["<< ix << "] = " << x;
		// DEBUG3 NEWLINE;
		h = find(*at(ix), beta->at(ix), x);
		if (h==NULL)
			return ix;
		else {
			test2_helper(h, g, g);
			// perf-optimize
			// these 3 operations
			// pi2 = h->inverse();
			// DEBUG3 pi2->print2("test2::pi2");

			// // refactor-use-*
			// pi3 = pi2->multiply(*g);
			// DEBUG3 pi3->print2("test2::pi3");

			// pi3->copy_to(g);
			// DEBUG3 g->print2("test2::g");

			// pool-me-once-shame-on-you
			// delete(pi3);
			// pool-me-once-shame-on-you
			// delete(pi2);
		}
	}
	return n;
}

void Schreiersims::enter(Permutation *g)
{
	//beta = new Permutation(g->size());
	beta = PermutationPool::instances[g->size()]->get();
	enter2(beta);
}

void Schreiersims::enter2(Permutation *g, std::string str_level)
{
	int n = g->size();
	int ix = test2(g, beta, str_level);
	// DEBUG2 std::cout << str_level << "enter2 :: ix: " << ix;
	// DEBUG2 NEWLINE;
	if (ix == n) {
		// DEBUG2 std::cout << str_level << "ix == n";
		// DEBUG2 NEWLINE;
		// pool-me-once-shame-on-you
		// delete(g);
		g->free_element();
		// DEBUG2 std::cout << str_level << "deleted g" << std::endl;
		return;
	}
	else {
		append(ix, g, str_level);
		// DEBUG2 std::cout << str_level << "add of [" << ix << "]: " <<
		//	at(ix) << std::endl;
		for(auto p: *at(ix)) {
			// DEBUG2 std::cout << " @ " << p << ": ";
			// DEBUG2 p->print2(str_level + "enter2:after_test2");
		}
	}
	Permutation *f, *h;
	/// perf-benchmark -- double for!!!
	FORjx(ix+1) {
		// looping using iterators leads to wrong memory address
		// being returned by at(jx)
		// for (auto h : *at(jx)) {
		// instead of using vector iterator, going for for-loop
		int nk = at(jx)->size();
		// can safely skip the first element,
		// since its going to be always identity permutation
		// in which case nothing much happens
		// since every U_i has identity permutation, so
		// test2 will always return n
		for(int kx=1; kx < nk; kx++) {
			h = (*at(jx))[kx];
			// DEBUG2 std::cout << str_level << "add of [" << jx << "]: " <<
			//	at(ix) << std::endl;
			// DEBUG2 std::cout << str_level <<  "(" << jx << "/" << ix <<
			//	", " << kx << "/" << at(jx)->size() << ")" << std::endl;
			// DEBUG2 std::cout << str_level << "h #" << h->id <<
			//	"# @" << h << std::endl;
			// DEBUG2 h->print2(str_level + "enter2::h");
			f = g->multiply(*h);
			// DEBUG2 f->print2(str_level + "enter2::f");
			enter2(f, str_level + " - ");
			//delete(f);
		}
	}
}

Schreiersims* Schreiersims::change_base(Schreiersims* g_bar, Permutation *beta_prime, Schreiersims **dst)
{
	// DEBUG2 g_bar->print("change_base::g_bar");
	int n = beta_prime->size();
	Schreiersims *h = new Schreiersims(n, beta_prime);
	// h->id = g_bar->id+1;
	/// perf-benchmark -- double for!!!
	FORjx(n) {
		// DEBUG2 cout << "change_base::jx: " << jx << endl;
		for(auto g : *g_bar->at(jx)) {
			// DEBUG2 g->print("change_base::g");
			h->enter2(g->clone());
		}
	}
	// pool-me-once-shame-on-you
	delete(g_bar);
	*dst = h;
	// DEBUG2 (*dst)->print("change_base::dst");
	// DEBUG2 cout << endl;
	return h;
}

Schreiersims::Schreiersims()
{
	update_id();
	throw std::runtime_error("not implemented");
}

Schreiersims::Schreiersims(int n, Permutation* _beta)
{
	update_id();
	initialize_elements(n);
	set_base(_beta);
	scratch_buffer = (int *) calloc(n, sizeof(int));
}

void Schreiersims::update_id()
{
	Schreiersims::id_g += 1;
	id = Schreiersims::id_g;
}

void Schreiersims::initialize_elements(const int n)
{
	// Permutations *whoa;
	/// perf-benchmark
	num_elements = n;
	elements = new Permutations[n];
	FORix(num_elements) {
		// whoa = at(ix);
		// tmp = Permutations(1);
		//whoa = &tmp;
		elements[ix] = Permutations();
		elements[ix].reserve(n);
		/// perf-benchmark
		// whoa->push_back(Permutation::identity(n));

		elements[ix].push_back(Permutation::identity(n));
		// elements.at(ix) = whoa;
	}
}

void Schreiersims::set_base(Permutation* _beta)
{
	beta = _beta;
}


Schreiersims::~Schreiersims()
{
	// DEBUG2 std::cout << "~sch#" << id << std::endl;
	// std::cout << "|elements| = " << elements.size() << std::endl;
	// for(auto permutations : elements) {
	Permutations permutations;
	ss_for {
		permutations = *at(ix);
		// DEBUG2 std::cout << "sch[" << permutations.size() << "]:" << ix << std::endl;
		Schreiersims::stats_size[permutations.size()] += 1;
		Schreiersims::stats_capacity[permutations.capacity()] += 1;
		for(auto permutation : permutations) {
			if(permutation != nullptr) {
				// DEBUG2 std::cout << "~sch #perm" << permutation->id << std::endl;
				//delete(permutation);
				permutation->free_element();
			}
		}
		// ++ix;
	}
	// DEBUG2 beta->print("~Schreiersims");
	if(beta != nullptr) {
		// delete(beta);
		beta->free_element();
	}
	if(scratch_buffer != nullptr) {
		free(scratch_buffer);
	}
	// free(elements);
	// delete(elements);
	delete[](elements);
}

void Schreiersims::append(Permutations permutations, Permutation *permutation)
{
	/// perf-benchmark
	permutations.push_back(permutation);
	/// perf-benchmark
	// DEBUG3 permutations.at(permutations.size()-1)->print(std::to_string(permutations.size()));
}

void Schreiersims::append(int ix, Permutation *permutation, std::string str_level)
{
	/// perf-benchmark
	Permutations *whoa = at(ix);
	whoa->push_back(permutation);
	// DEBUG2 std::cout << str_level << "at[" << ix << "] << @ " <<
	//	permutation << " :";
	// DEBUG2 permutation->print2("");
	// elements.at(ix) = whoa;
}

void Schreiersims::generate(Permutations generators)
{
	generators.at(0);
	int n = generators.at(0)->size();
	std::cout << "n : " << n; NEWLINE;
	Permutations* whoa;
	// resize(n);
	FORix(n) {
		whoa = at(ix);
		// whoa->push_back(new Permutation(n));
		whoa->push_back(PermutationPool::instances[n]->get());
		// elements.at(ix) = whoa;
	}
	// print();
	for (auto generator : generators) {
		enter(generator);
	}
	print();
}

// void Schreiersims::assign(int ix, Permutations permutations)
// {
// 	/// perf-benchmark
// 	// resize(ix+1);
// 	// elements.assign(ix, permutations);
// 	// elements[ix] = &permutations;
// }

void Schreiersims::print(string str_prefix)
{
	// DEBUG2 std::cout << str_prefix << " #" << id << "[" << size() << "]" << endl;
	// ss_for {
	// 	DEBUG2 std::cout << "U_" << ix << "[" << at(ix)->size() << "]: ";
	// 	for(auto permutation : elements[ix])
	// 		DEBUG2 permutation->print2("");
	// }
	// DEBUG2 beta->print("beta");
}

/// perf-benchmark
// void Schreiersims::resize(int size)
// {
// 	elements.resize(size);
// }
