#ifndef UNIQUE_CERTIFICATES_HH
#define UNIQUE_CERTIFICATES_HH

#include "menu_option.hh"
#include "certificate.hh"
#include "frequency_counter.hh"

namespace ga {
class UniqueCertificates : public MenuOption {
public:
	UniqueCertificates();
	~UniqueCertificates();

	void print_args_input_info() override;
	void get_args_input(char split_char) override;
	void init() override;
	void init_pools();
	void init_for_new_n(int n) override;
	void process() override;
	void process_graph(const Graph* graph) override;
	void post_processing() override;
	void print_args() override;
	
private:
	void validate_frequencies(std::map<int, FrequencyCounter<long>> freq_counter_by_num_vertices);

	std::map<int, FrequencyCounter<long>> freq_counter_by_num_vertices;
	std::map<int, std::map<long long, int>> automorphs_by_n_cert;

	void save_freq_counter();
	void save_automorphs();

	std::string fi_db = "data/db_unique_certificates.csv";
	std::string fi_db_automorphs = "data/db_num_automorphs.csv";
};
}

#endif /* UNIQUE_CERTIFICATES_HH */
