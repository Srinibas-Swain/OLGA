#include "gtest/gtest.h"
#include "certificate_cache.hh"

#include <iostream>

class CertificateCacheTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(CertificateCacheTest, BasicTest) {
	CertificateCache* cc = new CertificateCache();
	long long graph_id=55, cert=24;
	cc->save(graph_id, cert);
	ASSERT_EQ(true, cc->present(graph_id));
	ASSERT_EQ(false, cc->present(graph_id+1));
	delete(cc);
}


