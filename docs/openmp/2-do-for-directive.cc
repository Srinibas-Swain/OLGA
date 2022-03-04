
#include <iostream>

#include <omp.h>
#define N 10
#define CHUNKSIZE 2

using namespace std;

/*
  SCHEDULE
  Describes how iterations of the loop are divided among the threads in the team. The default schedule is implementation dependent. For a discussion on how one type of scheduling may be more optimal than others, see http://openmp.org/forum/viewtopic.php?f=3&t=83.

    STATIC: Loop iterations are divided into pieces of size chunk and then statically assigned to threads. If chunk is not specified, the iterations are evenly (if possible) divided contiguously among the threads.
  
    DYNAMIC: Loop iterations are divided into pieces of size chunk, and dynamically scheduled among the threads; when a thread finishes one chunk, it is dynamically assigned another. The default chunk size is 1.

    GUIDED: Iterations are dynamically assigned to threads in blocks as threads request them until no blocks remain to be assigned. Similar to DYNAMIC except that the block size decreases each time a parcel of work is given to a thread. The size of the initial block is proportional to:

      number_of_iterations / number_of_threads

    Subsequent blocks are proportional to

      number_of_iterations_remaining / number_of_threads

    The chunk parameter defines the minimum block size. The default chunk size is 1.

    RUNTIME: The scheduling decision is deferred until runtime by the environment variable OMP_SCHEDULE. It is illegal to specify a chunk size for this clause.

    AUTO: The scheduling decision is delegated to the compiler and/or runtime system.
 */

int main(int argc, char *argv[]) {

	int i, chunk;
	float a[N], b[N], c[N];

	/* Some initializations */
	for (i=0; i < N; i++)
		a[i] = b[i] = i * 1.0;
	chunk = CHUNKSIZE;
	int tid;

#pragma omp parallel shared(a,b,c,chunk) private(i, tid)
	{

#pragma omp for schedule(guided,chunk) nowait
		for (i=0; i < N; i++) {
			tid = omp_get_thread_num();
			cout << " #" << tid << "# ";
			c[i] = a[i] + b[i];
		}

	}   /* end of parallel region */
	for(int ix=0; ix<N; ix++) {
		cout << "c[" << ix << "] = " << c[ix] << endl;
	}
	return 0;
}
