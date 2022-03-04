#include <iostream>
#include <vector>
#include <omp.h>

#include "permutation.hh"
#include "permutation_pool.hh"
#include "utils.hh"

using namespace ga;

Permutation::Permutation(size_t n)
{
	class_name = "permutation";
	initialize(n);
}

Permutation::~Permutation()
{
	// DEBUG2 std::cout << "~perm#" << id << "#";
	// DEBUG2 NEWLINE;
	class_name = "permutation";
	Permutation::count -= 1;
	delete[](mappings);
	//Permutation::id_g  -= 1;
}

Permutation::Permutation(size_t n, std::string str)
{
	initialize(n);
	// std::cout << str.length(); NEWLINE;
	std::string tmp;
	bool inside=false;
	for(auto chr : str) {
		if (chr == '(') {
			tmp = ""; inside = true;
		}
		else if (chr == ')') {
			add_cycle(tmp);
		}
		else if (inside)
			tmp += chr;
		else if (!inside)
			continue;
	}
}

// create an identity permutation
void Permutation::initialize(const size_t n)
{
	number_of_vertices = n;
	update_id();
	/// perf-benchmark
	mappings = new long[n];
	perm_for {
		mappings[ix] = ix;
	}
}

void Permutation::update_id()
{
	id = Permutation::id_g;
	Permutation::count += 1;
	Permutation::id_g  += 1;
}

size_t Permutation::size() const
{
	return number_of_vertices;
}

void Permutation::add_cycle(std::string str)
{
	std::vector<std::string> splits = ga::split(str, ',');
	FORix(splits.size()-1) {
		add_mapping(splits[ix], splits[ix+1]);
	}
	add_mapping(splits[splits.size()-1], splits[0]);
}

void Permutation::reset()
{
	perm_for {
		mappings[ix] = ix;
	}
}

void Permutation::add_mapping(std::string from, std::string to)
{
	add_mapping(std::stoi(from), std::stoi(to));
}

void Permutation::validate_bounds(size_t ix)
{
	if(ix<number_of_vertices)
		return;

	std::ostringstream oss_err;
	oss_err << "perm bounds: " << ix << " / " << number_of_vertices;
	throw std::runtime_error(oss_err.str());
}


void Permutation::add_mapping(size_t from, size_t to)
{
	validate_bounds(from);
	validate_bounds(to);
	mappings[from] = to;
}

/// perf-benchmark
// void Permutation::resize(size_t n)
// {
// 	if(mappings.size() <= n)
// 		mappings.resize(n+1);
// }

void Permutation::print(std::string prefix)
{
	std::cout << prefix << " #" << id << "# : ";
	perm_for {
		std::cout << ix << " => " << mappings[ix] << ", ";
	}
	NEWLINE;
}

void Permutation::print2(std::string prefix)
{
	std::vector<bool> P(number_of_vertices, true);
	size_t j;
	std::cout << prefix << "#" << id << " : ";
	perm_for {
		if(P[ix]) {
			std::cout << "(" << ix;
			P.at(ix)= false;
			j = ix;
			while(P[mappings[j]]) {
				j = mappings[j];
				std::cout << "," << j;
				P.at(j) = false;
			}
			std::cout << ")";
		}
	}
	NEWLINE;
}

Permutation* Permutation::multiply(Permutation &beta)
{
	// pool-me-once-shame-on-you
	// Permutation* out = new Permutation(beta.size());
	Permutation* out = PermutationPool::instances[beta.size()]->get();
	perm_for {
		out->add_mapping(ix, mappings[beta[ix]]);
	}
	return out;
}

Permutation* Permutation::operator*(Permutation &beta)
{
	return multiply(beta);
}

void Permutation::multiply(Permutation& beta, Permutation& out)
{
	Permutation* tmp = PermutationPool::instances[out.size()]->get();
	perm_for {
		tmp->add_mapping(ix, mappings[beta[ix]]);
	}
	perm_for {
		out.add_mapping(ix, tmp->at(ix));
	}
	delete(tmp);
}

Permutation* Permutation::inverse() const
{
	// pool-me-once-shame-on-you
	Permutation* out = PermutationPool::instances[number_of_vertices]->get();
	perm_for {
		out->add_mapping(mappings[ix], ix);
	}
	return out;
}
void Permutation::inverse(Permutation& out) const
{
	perm_for {
		out.add_mapping(mappings[ix], ix);
	}
}

size_t Permutation::at(size_t ix) const
{
	return mappings[ix];
}

size_t Permutation::operator[](size_t ix) const
{
	return mappings[ix];
}

void Permutation::copy_to(Permutation *other) const
{
	perm_for {
		other->add_mapping(ix, mappings[ix]);
	}
}

Permutation* Permutation::clone()
{
	Permutation* new1 = PermutationPool::instances[size()]->get();
	copy_to(new1);
	return new1;
}


// pool-me-once-shame-on-you
Permutation* Permutation::identity(size_t n)
{
	return PermutationPool::instances[n]->get();
}
