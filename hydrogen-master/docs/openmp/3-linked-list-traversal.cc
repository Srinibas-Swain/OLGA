

#include <queue>
#include <iostream>
#include <sstream>
#include <string>
#include <omp.h>
#include <unistd.h>

#define OMPTASK 

using namespace std;

int main()
{
	std::queue<float> queue;
	std::queue<float> present;
	//for(int ix=3; ix<20; ix++)
	//	queue.push(ix * 0.25);
	queue.push(2.3);
	
	int tid;
	float i,j;
	std::ostringstream oss1, oss2, oss3;
	#pragma omp parallel
	{			
		#pragma omp single nowait
		{
			while (!queue.empty()) {
				#pragma omp task shared(queue, present) private(tid, i, j, oss1, oss2, oss3)
				{
					tid = omp_get_thread_num();
					i = -1;
					#pragma omp critical
					{
						if(!queue.empty()) {
							i = queue.front();
							queue.pop();
							present.push(i);
						}
					}

					
					
					j = i*i;
					if(j<100 && j > 1) {
						#pragma omp critical
						{
							queue.push(j);
						}
					}
					
					if(i != -1) {
						oss1 << "thread #" << tid << "# : got " << i << " and pushed " << j << "\n";
						std::cout << oss1.str();
						sleep(i);
						oss2 << "thread #" << tid << "# : waking up after " << i << " seconds \n";
						std::cout << oss2.str();
					}
				}
			}
		}
	}
	if(queue.empty())
		std::cout << "queue empty" << std::endl;
	else
		std::cout << "queue not empty" << std::endl;

	return 0;
}





// 





