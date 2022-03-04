#ifndef FREQUENCY_COUNTER_HH
#define FREQUENCY_COUNTER_HH

#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>

namespace ga {
template <class T>
class FrequencyCounter {
public:
	// FrequencyCounter();
	// ~FrequencyCounter();
	void update(T key);
	bool present(T key);
	long long get(T key);
	void clear();
	void print();
	std::string get_string();
	std::pair<std::vector<T>, long long> max();
	typename std::map<T, long long>::iterator begin();
	typename std::map<T, long long>::iterator end();
private:
	std::map <T, long long> data;
};

template <class T>
typename std::map<T, long long>::iterator FrequencyCounter<T>::begin()
{
	return data.begin();
}

template <class T>
typename std::map<T, long long>::iterator FrequencyCounter<T>::end()
{
	return data.end();
}

	
template <class T>
void FrequencyCounter<T>::update(T key) {
	if(!present(key))
		data[key] = 0;
	data[key] = data[key] + 1;
}

template <class T>
bool FrequencyCounter<T>::present(T key) {
	return (data.find(key) != data.end());
}

template <class T>
long long FrequencyCounter<T>::get(T key) {
	if(present(key))
		return data[key];
	else
		return 0L;
}

template <class T>
std::pair<std::vector<T>, long long>
FrequencyCounter<T>::max() {
	T t_max;
	std::vector<T> t_max_all;
	long long max_freq = 0;

	if(data.empty())
		return std::make_pair(t_max_all, max_freq);
	
	for(auto it: data) {
		if(it.second > max_freq) {
			max_freq = it.second;
			t_max = it.first;
		}
	}

	for(auto it : data) {
		if(it.second == max_freq)
			t_max_all.push_back(it.first);
	}
	return std::make_pair(t_max_all, max_freq);
}

template <class T>
void FrequencyCounter<T>::clear() {
	data.clear();
}

template <class T>
void FrequencyCounter<T>::print() {
	std::cout << "{";
	for(auto it: data) {
		std::cout << "[" << it.first << " x" << it.second  << "]";
	}
	std::cout << "}" << std::endl;
}

template <class T>
std::string FrequencyCounter<T>::get_string() {
	std::ostringstream oss_fc;
	oss_fc << "{";
		for(auto it: data) {
			//oss_fc << "[" << it.first << " x" << it.second  << "]";
		}
	oss_fc << "}" << std::endl;
	return oss_fc.str();
}
	
}

#endif /* FREQUENCY_COUNTER_HH */


