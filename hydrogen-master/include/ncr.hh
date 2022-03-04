#ifndef NCR_HH
#define NCR_HH

namespace ga {
	class Ncr {
	public:
		Ncr();
		~Ncr();

		void init(int n);
		long long get(int n, int r) const;
		void print();
	  
	private:
		void validate_n_r(int n, int r) const;
		void init_data();
		void clear();
		
		long long **data;
		int N;
	};
}

#endif /* NCR_HH */

