#include <gtest/gtest.h>
#include <cstring>

using namespace std;

bool accept_name(const char* name) {
  const char *result = strstr(name, "eek");
  if(result == NULL) 
    return false;
  return true;
}

class MyStringTest : public testing::TestWithParam<const char*>
{
  public:
    virtual void SetUp() {}
    virtual void TearDown() {}
};

// change mee1k1 to meek to make it work
INSTANTIATE_TEST_CASE_P(InstantiationName, MyStringTest,
                        ::testing::Values("meek","geek")
          );

TEST_P(MyStringTest, acceptsEekWords) {
  ASSERT_TRUE(accept_name(GetParam()));
}

TEST_P(MyStringTest, rejectsNonEekWords) {
	ASSERT_FALSE(accept_name("none1feky"));
}
