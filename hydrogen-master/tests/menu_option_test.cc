#include "gtest/gtest.h"
#include "menu_option.hh"

#include <iostream>

class MenuOptionTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test
		// (right before the destructor).
	}
};

TEST_F(MenuOptionTest, TerminationTest) {
	MenuOption* mo = new MenuOption();
	//ASSERT_EQ(false, mo);
	delete(mo);
}

