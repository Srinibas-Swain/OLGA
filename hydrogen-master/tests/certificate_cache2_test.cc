#include "gtest/gtest.h"
#include "certificate_cache2.hh"

#include <iostream>

class CertificateCache2Test : public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(CertificateCache2Test, BasicTest) {
	CertificateCache2* cc = new CertificateCache2();
	long long graph_id=55, cert=24;
	cc->save(5, graph_id, cert);
	ASSERT_EQ(true, cc->present(5, graph_id));
	ASSERT_EQ(false, cc->present(5, graph_id+1));
	delete(cc);
}


