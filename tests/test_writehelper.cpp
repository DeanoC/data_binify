#include "al2o3_platform/platform.h"
#include "al2o3_catch2/catch2.hpp"
#include "data_binify/writehelper.hpp"

TEST_CASE( "Write Helper create/destroy", "[Binify WriteHelper]" ) {
	Binify::WriteHelper helper;
	REQUIRE(tinystl::string(helper.c_str()).empty());
}

TEST_CASE( "Write Helper set variable", "[Binify WriteHelper]" ) {
	{
		Binify::WriteHelper helper;
		helper.SetVariable("Bob1", 10);
		REQUIRE(tinystl::string("VAR_Bob1 = 10\n") == helper.c_str());
	}
	{
		Binify::WriteHelper helper;
		helper.SetVariable("122Bob1", 0);
		REQUIRE(tinystl::string("VAR_122Bob1 = 0\n") == helper.c_str());
	}
	{
		Binify::WriteHelper helper;
		helper.SetVariable("Bob1", 10, true);
		REQUIRE(tinystl::string("*VAR_Bob1* 10\n") == helper.c_str());
	}
	{
		Binify::WriteHelper helper;
		helper.SetVariable("Bob1", 10, false, "comment");
		REQUIRE(tinystl::string("VAR_Bob1 = 10 // comment\n") == helper.c_str());
	}
}

TEST_CASE( "Write Helper set const", "[Binify WriteHelper]" ) {
	{
		Binify::WriteHelper helper;
		helper.SetConstant("Bob1", 10);
		REQUIRE(tinystl::string("*CONST_Bob1* 10\n") == helper.c_str());
	}
}