#include "certificate_cache.hh"

#include <iostream>

using namespace ga;

CertificateCache::CertificateCache() {
	
}

CertificateCache::~CertificateCache() {
	
}

bool CertificateCache::present(long long graph_id)
{
	return (certs_by_graph_id.find(graph_id) != certs_by_graph_id.end());
}

void CertificateCache::save(long long graph_id, long long cert)
{
	if( !(present(graph_id)) && (cert != -1) ) {
		certs_by_graph_id[graph_id] = cert;
	}
}

void CertificateCache::print()
{
	for(auto it: certs_by_graph_id) {
		std::cout << it.first << ": " << it.second << std::endl;
	}
}

long long CertificateCache::get(long long graph_id)
{
	return certs_by_graph_id[graph_id];
}

long long CertificateCache::rget(long long cert)
{
	for(auto it : certs_by_graph_id) {
		if(it.second == cert)
			return it.first;
	}
	return -1;
}

