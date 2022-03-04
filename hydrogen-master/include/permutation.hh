#ifndef __PERMUTATION_HH
#define __PERMUTATION_HH

#include <cstdlib>
#include <string>

#include "generic_object.hh"

namespace ga {
#define perm_for FORix(number_of_vertices)
	
	class Permutation : public GenericObject {
	public:

		// for pooling
		static size_t id_g;
		friend class PermutationPool;
		virtual void init_element() override {reset();}
		//virtual std::string class_name() override { return ; }
		virtual void reset_element() override {reset();}
		
		static size_t count;
		long* mappings;

		size_t size() const;

		void reset();
		void add_cycle(std::string str);
		void add_mapping(size_t from, size_t to);
		void add_mapping(std::string from, std::string to);

		void print(std::string prefix="perm");
		void print2(std::string prefix="perm");

		Permutation* multiply(Permutation& beta);
		Permutation* operator*(Permutation& beta);
		void multiply(Permutation& beta, Permutation& out);
		Permutation* inverse() const;
		void inverse(Permutation& out) const;

		size_t operator[](size_t ix) const;
		size_t at(size_t ix) const;

		void copy_to(Permutation *other) const;
		Permutation* clone();

		// class methods
		static Permutation* identity(size_t n);
		
	private:
		Permutation(size_t n, std::string str);
		Permutation(size_t n);
		~Permutation();

		void initialize(const size_t n);
		void update_id();
		void validate_bounds(size_t ix);
		long number_of_vertices;
	};
}

#endif
