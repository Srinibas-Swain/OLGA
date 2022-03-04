#include "utils.hh"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

using namespace ga;

std::vector<std::string>& ga::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> ga::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
	ga::split(s, delim, elems);
    return elems;
}

void newline(int n)
{
	FORix(n)
		std::cout << std::endl;
}


long long ga::getmsofday()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (long long)tv.tv_sec*1000 + tv.tv_usec/1000;
}

long long ga::binary_string_2_long(std::string str)
{
	long long number = 0;
	for(auto chr : str) {
		number <<= 1;
		if(chr == '1')
			number |= 1;
	}
	return number;
}

void ga::print(const std::string &s, int times)
{
	FORix(times)
		std::cout << s;
	NEWLINE;
}

std::string get_env(const char* name)
{
	const char* env_var = getenv(name);
	return (env_var == NULL) ? "" : std::string(env_var);
}

int get_int_env(const char* name, int default_value)
{
	std::string _str_env = get_env(name);
	return ((_str_env.compare("")==0) ? default_value : std::stoi(_str_env));
}

bool is_env_present(const char *name)
{
	return !(get_env(name).compare("")==0);
}

bool is_option_present()
{
	return is_env_present("OPTION");
}

int get_parallel()
{
	return get_int_env("PARALLEL", 0);
}

bool is_parallel()
{
	return (get_parallel()==1) || get_number_of_threads() > 1;
}

int get_number_of_threads()
{
	// std::string _number_of_threads = get_env("THREADS");
	// return ((_number_of_threads.compare("")==0) ? 1 : std::stoi(_number_of_threads));
	return get_int_env("THREADS", 1);
}

int debug_level()
{
	// std::string _debug_level = get_env("DEBUG");
	// return ((_debug_level.compare("")==0) ? 0 : std::stoi(_debug_level));
	return get_int_env("DEBUG", 0);
}

int get_option()
{
	return get_int_env("OPTION", 1);
}

int get_start_index()
{
	return get_int_env("START", 3);
}

int get_stop_index()
{
	return get_int_env("STOP", 3);
}


int num_digits(long long number)
{
	int digits = 0;
	do { number /= 10; digits++; } while (number != 0);
	return digits;
}
