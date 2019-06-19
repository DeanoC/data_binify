#include "al2o3_platform/platform.h"
#include "al2o3_catch2/catch2.hpp"
#include "data_binify/binify.h"

TEST_CASE( "Alloc and Free", "[Binify]" )
{
	Binify_ContextHandle handle = Binify_Create("");
	REQUIRE(handle);
	Binify_Destroy(handle);
}

TEST_CASE( "Empty Parse", "[Binify]" ) {
	Binify_ContextHandle handle = Binify_Create("");
	REQUIRE(handle);
	size_t size = Binify_BinarySize(handle);
	REQUIRE(size == 0);
	Binify_Destroy(handle);
}