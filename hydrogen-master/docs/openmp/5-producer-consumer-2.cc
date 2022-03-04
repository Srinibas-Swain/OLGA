
#include <omp.h>
#include <iostream>
#include <math.h>

#define STRIDE 8
#define N 10000000

int main()
{
	float* ssumt = new float[N*STRIDE];
	double ssum=0;
	
	#pragma omp parallel
	{
		#pragma omp single
		for (int i = 0; i < N; i++) {
			#pragma omp task
			{
				const int idx = omp_get_thread_num();
				ssumt[idx*STRIDE] += sin(i*0.001);
			}
		}
		#pragma omp taskwait

		const int idx = omp_get_thread_num();
		#pragma omp atomic
		ssum += ssumt[idx*STRIDE];
	}
	std::cout << "sum: " << ssum << std::endl;
	
	delete[] ssumt;
}
