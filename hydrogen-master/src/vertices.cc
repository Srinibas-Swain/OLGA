#include "vertices.hh"

using namespace ga;

Vertices::Vertices(int n) {
	//elements.resize(n);
	for(int ix=0; ix<n; ix++) {
		elements.push_back(ix);
	}
}
