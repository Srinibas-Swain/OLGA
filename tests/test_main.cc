#include <limits.h>
#include "gtest/gtest.h"

#include "block.hh"
#include "permutation.hh"
#include "partition.hh"
#include "graph.hh"

int Block1::id_g = 0;
int Block1::count = 0;
int Permutation::id_g = 0;
int Permutation::count = 0;
int Partition::id_g = 0;
int Partition::count = 0;
vector<long> Graph::NUM_UPPER_TRIANGULAR = vector<long>();

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
