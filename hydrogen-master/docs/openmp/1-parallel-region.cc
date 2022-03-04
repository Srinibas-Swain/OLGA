

#include <array>
#include <vector>
#include <iostream>
# include <omp.h>

using namespace std;


int main()
{

	int nthreads, tid;

	/* Fork a team of threads with each thread having a private tid variable */
#pragma omp parallel private(tid)
	{

		/* Obtain and print thread id */
		tid = omp_get_thread_num();
		cout << "Hello World from thread = " << tid << endl;

		/* Only master thread does this */
		if (tid == 0) 
			{
				nthreads = omp_get_num_threads();
				cout << "Number of threads = " <<  nthreads << endl;
			}

	}  /* All threads join master thread and terminate */

}











