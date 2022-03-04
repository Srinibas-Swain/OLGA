#ifndef CERTIFICATE_CACHE_TWO_HH
#define CERTIFICATE_CACHE_TWO_HH

#include <map>
#include <unordered_map>

#include "graph.hh"

namespace ga {
class CertificateCache2 {
public:
	CertificateCache2();
	~CertificateCache2();

	// these should be private
	bool present(const Graph* graph);
	bool present(int num_vertices, long long graph_id);
	
	void save(const Graph* graph, long long cert);
	void save(int num_vertices, long long graph_id, long long cert);

	long long get(const Graph* graph);
	long long get(int num_vertices, long long graph_id);
	
	//long long rget(const Graph* graph, long long cert);
	long long rget(int num_vertices, long long cert);

	void print();
private:
	std::map <int, std::unordered_map<long long, long long> > certs_by_graph_id;
};
}

#endif /* CERTIFICATE_CACHE_TWO_HH */


