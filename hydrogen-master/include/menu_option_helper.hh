#ifndef MENU_OPTION_HELPER_HH
#define MENU_OPTION_HELPER_HH

#include <map>

#include "certificate.hh"

namespace ga {
	class MenuOptionHelper {
	public:
		MenuOptionHelper();
		~MenuOptionHelper();

		virtual void init(int n);
		long long generate_certificate(const Graph* graph);
		std::tuple<long long, int> generate_cert_and_automorph(const Graph* graph);
		bool lock(std::map<long long, bool> &certs_done, long long certificate);
		virtual void init_for_new_n(int n);
		
	private:
		Certificate *cert_gen;
	};
}

#endif /* MENU_OPTION_HELPER_HH */

