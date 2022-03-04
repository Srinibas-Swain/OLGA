#include "certificate_cache2.hh"

#include <iostream>

using namespace ga;

CertificateCache2::CertificateCache2() {
	
}

CertificateCache2::~CertificateCache2() {
	
}

bool CertificateCache2::present(const Graph* graph)
{
	return present(graph->num_vertices, graph->graph_id);
}


// check if the fucker is present
bool CertificateCache2::present(int num_vertices, long long graph_id)
{
	// std::cout << " vert: " << (certs_by_graph_id.find(num_vertices) != certs_by_graph_id.end());
	// if(certs_by_graph_id.find(num_vertices) != certs_by_graph_id.end())
	// 	std::cout << " gid: " << (certs_by_graph_id[num_vertices].find(graph_id) != certs_by_graph_id[num_vertices].end()) << " | ";
	// std::cout << (certs_by_graph_id[num_vertices].find(graph_id) != certs_by_graph_id[num_vertices].end()) << " || ";
	return (certs_by_graph_id.find(num_vertices) != certs_by_graph_id.end()) &&
		(certs_by_graph_id[num_vertices].find(graph_id) != certs_by_graph_id[num_vertices].end());
}


void CertificateCache2::save(const Graph* graph, long long cert)
{
	save(graph->num_vertices, graph->graph_id, cert);
}

void CertificateCache2::save(int num_vertices, long long graph_id, long long cert)
{
	if( !(present(num_vertices, graph_id)) && (cert != -1) ) {
		certs_by_graph_id[num_vertices][graph_id] = cert;
	}
}

void CertificateCache2::print()
{
	
	for(auto it: certs_by_graph_id) {
		for(auto it2 : it.second)
			std::cout << "#" << it.first << "::" << it2.first << " -- " << it2.second << std::endl;
	}
}


long long CertificateCache2::get(const Graph* graph)
{
	return get(graph->num_vertices, graph->graph_id);
}

long long CertificateCache2::get(int num_vertices, long long graph_id)
{
	return certs_by_graph_id[num_vertices][graph_id];
}

long long CertificateCache2::rget(int num_vertices, long long cert)
{
	if((certs_by_graph_id.find(num_vertices) == certs_by_graph_id.end()))
		return -1;
	
	for(auto it : certs_by_graph_id[num_vertices]) {
		if(it.second == cert)
			return it.first;
	}
	return -1;
}

