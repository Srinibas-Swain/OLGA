#include "menu_option_helper.hh"

#include <omp.h>

#include "certificate.hh"

using namespace ga;

MenuOptionHelper::MenuOptionHelper() {
	cert_gen = new Certificate();
}

MenuOptionHelper::~MenuOptionHelper() {
	delete(cert_gen);
}

long long MenuOptionHelper::generate_certificate(const Graph* graph)
{
	return cert_gen->generate_certificate2(graph);
}

std::tuple<long long, int> MenuOptionHelper::generate_cert_and_automorph(const Graph* graph)
{
	return cert_gen->generate_cert_and_automorph(graph);
}

bool MenuOptionHelper::lock(std::map<long long, bool> &certs_done, long long cert)
{
	bool should_process = false;
	#pragma omp critical(mo_helper_lock)
	{
		if(certs_done[cert]) {
			should_process = false;
		} else {
			certs_done[cert] = true;
			should_process = true;
		}
	}
	return should_process;
}

void MenuOptionHelper::init(int n)
{
	
}

void MenuOptionHelper::init_for_new_n(int n)
{
	
}
