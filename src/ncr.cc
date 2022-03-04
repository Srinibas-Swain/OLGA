#include "ncr.hh"

#include <iostream>
#include <stdexcept>

#include "utils.hh"

using namespace ga;

Ncr::Ncr() {
	data = NULL;
}

Ncr::~Ncr() {
	clear();
}

long long Ncr::get(int n, int r) const
{
	validate_n_r(n, r);
	return data[n][r];
}

void Ncr::validate_n_r(int n, int r) const
{
	if(n>=0 || n<=N ||
	   r>=0 || r<=N)
		return;
	std::ostringstream oss_err;
	oss_err << "ncr- n:" << n << ", r:" << r << "N: " << N;
	throw std::runtime_error(oss_err.str());
}

void Ncr::print()
{
	for(int n=0; n<=N; n++) {
		std::cout << "n=" << n << ": ";
		for(int r=0; r<=n; r++) {
			std::cout << data[n][r] << "   ";
		}
		NEWLINE;
	}
}

void Ncr::init(int _n)
{
	//return;
	N = _n;
	std::cout << "ncr/init: " << N << std::endl;

	clear();
	init_data();

	data[0][0] = 1;
	for(int n=1; n<=N; n++) {
		for(int r=0; r<=n; r++) {
			// nCr = (n-1)C(r-1) + (n-1)Cr
			data[n][r] = 0;
			if(r!=0)
				data[n][r] += data[n-1][r-1];
			data[n][r] += data[n-1][r];
		}
	}
}

void Ncr::init_data()
{
	data = (long long**) malloc((N+1) * sizeof(long long*));
	FORix(N+1) {
		data[ix] = (long long*) calloc(N+1, sizeof(long long));
	}
}

void Ncr::clear()
{
	if(data != NULL) {
		if(N<1) {
			std::ostringstream oss_err;
			oss_err << "ncr: N " << N << " < 1";
			throw std::runtime_error(oss_err.str());
		}
		FORix(N+1)
			free(data[ix]);
		free(data);
	}
}


