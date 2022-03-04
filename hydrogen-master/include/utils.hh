#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <vector>
#include "constants.hh"
#include <chrono>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

namespace ga {
#define NEWLINE		std::cout << std::endl;
#define FORix(lim)	for(int ix=0;ix<(lim);ix++)
#define FORjx(lim)	for(int jx=0;jx<(lim);jx++)
#define FORkx(lim)	for(int kx=0;kx<(lim);kx++)
#define FORlx(lim)	for(int lx=0;lx<(lim);lx++)

// #define DEBUG  if(DEBUGGER == 0)
// #define DEBUG1 if(DEBUGGER >= 1)
// #define DEBUG2 if(DEBUGGER >= 2)
// #define DEBUG3 if(DEBUGGER >= 3)
// #define DEBUG  if(debug_level() == 0)
// #define DEBUG1 if(debug_level() == 1)
// #define DEBUG2 if(debug_level() == 2)
// #define DEBUG3 if(debug_level() == 3)


std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
long long binary_string_2_long(std::string str);

void newline(int n=1);
void print(const std::string &s, int times=1);

template<typename TimeT = std::chrono::milliseconds>
struct measure
{
    template<typename F, typename ...Args>
    static typename TimeT::rep execution(F func, Args&&... args)
    {
        auto start = std::chrono::system_clock::now();
        func(std::forward<Args>(args)...);
        auto duration = std::chrono::duration_cast< TimeT> 
			(std::chrono::system_clock::now() - start);
        return duration.count();
    }
};
// int main() {
//     std::cout << measure<>::execution(functor(dummy)) << std::endl;
// }

long long getmsofday();
}

std::string get_env(const char* name);
int get_int_env(const char* name, int default_value=0);
bool is_env_present(const char *name);

int get_parallel();
bool is_parallel();
int debug_level();

int get_number_of_threads();

bool is_option_present();
int get_option();

int get_start_index();
int get_stop_index();

int num_digits(long long number);

///-- CONVERTERS

#endif /* UTILS_H */
