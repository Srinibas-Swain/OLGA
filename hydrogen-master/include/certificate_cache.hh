#ifndef CERTIFICATE_CACHE_HH
#define CERTIFICATE_CACHE_HH

#include <map>

namespace ga {
class CertificateCache {
public:
	CertificateCache();
	~CertificateCache();

	// these should be private
	bool present(long long graph_id);
	void save(long long graph_id, long long cert);
	long long get(long long graph_id);
	long long rget(long long cert);
	void print();
private:
	std::map <long long, long> certs_by_graph_id;
};
}

#endif /* CERTIFICATE_CACHE_HH */


