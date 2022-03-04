#ifndef SCHREIERSIMS_H
#define SCHREIERSIMS_H

#include <array>
#include <string>
#include <vector>
#include "permutation.hh"

#include "utils.hh"

namespace ga {
using namespace std;

#define ss_for for(int ix=0; ix<num_elements; ix++)
class Schreiersims {
public:
	typedef std::vector<Permutation*> Permutations;
	Permutations* elements;
	Permutation *beta;

	Schreiersims();
	Schreiersims(int n, Permutation* _beta);
	~Schreiersims();
	
	Permutation* find(Schreiersims::Permutations permutations, int ix, int value);
	Permutations* at(int ix);
	void append(Permutations permutations, Permutation* permutation);
	void append(int ix, Permutation* permutation, std::string str_level="");
	// void assign(int ix, Permutations permutations);
	// void resize(int size);
	long long size() const;
	void print(string str_prefix="");
	void initialize_elements(int n);
	void set_base(Permutation* _beta);
	
	int test2(Permutation *g, Permutation *beta, std::string str_level="");
	void enter2(Permutation *g, std::string str_level="");
	void enter(Permutation *g);
	void generate(Permutations generators);
	long long id;

	// static methods
	static Schreiersims* change_base(Schreiersims* g_bar, Permutation* beta_prime, Schreiersims** dst);
	static long long id_g;
	static vector<int> stats_size;
	static vector<int> stats_capacity;
private:
	void test2_helper(Permutation*, Permutation*, Permutation*);
	void update_id();
	int* scratch_buffer;
	long long num_elements;
};
}

#endif /* SCHREIERSIMS_H */
