#ifndef PERMUTATION_POOL_HH
#define PERMUTATION_POOL_HH

#include <cstdlib>
#include "v_pool.hh"
#include "permutation.hh"

namespace ga {
	class PermutationPool : public VPool {
	public:
		size_type MIN_SIZE;
		//static PermutationPool instance;
		static PermutationPool **instances;

		PermutationPool(size_type _permutation_size=0);
		virtual void init_pool(size_type _permutation_size, size_type in_elements);
		virtual Permutation* get();

	protected:
		virtual Permutation* create_element() override;
		
	private:
		size_type permutation_size;
		void validate_element(Permutation* permutation,
							  size_type n);
		void validate_created_elements();
	};

}

#endif /* PERMUTATION_POOL_HH */

