#include "al2o3_platform/platform.h"
#include "al2o3_catch2/catch2.hpp"
#include "data_binify/binify.h"

TEST_CASE( "Alloc and Free", "[Binify_C]" )
{
	BINIFY_HANDLE const * handle = BINIFY_Alloc();
	REQUIRE(handle);
	BINIFY_Free(handle);
}

TEST_CASE( "Empty Parse", "[Binify_C]" )
{
	BINIFY_HANDLE const * handle = BINIFY_Alloc();
	int result = BINIFY_Parse(handle, "");
	REQUIRE(result);
	size_t size = BINIFY_BinarySize(handle);
	REQUIRE(size == 0);
}