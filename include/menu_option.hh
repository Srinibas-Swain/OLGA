#ifndef MENU_OPTION_HH
#define MENU_OPTION_HH

#include <chrono>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "certificate.hh"
#include "certificate_cache2.hh"
#include "graph.hh"
#include "graph_generator.hh"
#include "graph_seed.hh"
#include "menu_option_helper.hh"
#include "ncr.hh"
#include "types.hh"
#include "utils.hh"

namespace ga {
using ns = std::chrono::nanoseconds;
// using get_time = chrono::steady_clock;
using get_time = std::chrono::system_clock;

/**
   things needed
   1. define the title of the option
   2. helper functions
     1. get_args_input(split_char) and gives out the tokens in input_tokens
     2. print_args_input_info() prints a std welcome msg for this option using the title
 **/

class MenuOption {
public:
	MenuOption();
	virtual ~MenuOption();
	virtual void print_args_input_info();
	virtual void get_args_input(char split_char=' ');
	virtual void init(); // before processing all

	virtual void process();
	virtual void process_graphs(GraphGenerator::Direction dir = GraphGenerator::Direction::from_kn); // prefer menuoption's version 
	virtual void process_graph(const Graph* graph); // override mostly

	virtual void init_for_new_n(int n); // b4 staring new n
	virtual void post_processing(); // after processing all
	virtual void print_args();
	long long get_certificate(const Graph* graph) const;
	long long compute_graph_id_and_certify(const Graph* graph);
	long long generate_certificate(const Graph* graph);
	std::tuple<long long, int> generate_cert_and_automorph(const Graph* graph);

	void print_cert_cache() const;

	std::string title;
	//std::map<long long, bool> certs_done;
	CertificateCache2 *cert_cache;

	MenuOptionHelper** mo_helpers;
	int ix_start, ix_end;

	friend class MenuOptionHelper;
	
private:
protected:
	std::vector<std::string> input_tokens;
	Certificate* certificate;

	Ncr* ncr;

	virtual void init_objects();
	void save_certificates(std::string f_name=nullptr);
	void load_certificates(std::string f_name=nullptr);
	int num_digits_cert_max();
	void init_pools();
	// std::tuple<long long, int> get_cert_automorph(const Graph* graph) const;
};
}

#endif /* MENU_OPTION_HH */


