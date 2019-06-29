#if building standalone and not specified build tests
option(BUILD_TESTS "Whether to build tests" ON)
include(FetchContent)
FetchContent_Declare( al2o3 GIT_REPOSITORY https://github.com/DeanoC/al2o3 GIT_TAG master )
FetchContent_GetProperties(al2o3)
if(NOT al2o3_POPULATED)
	FetchContent_Populate(al2o3)
	add_subdirectory(${al2o3_SOURCE_DIR} ${al2o3_BINARY_DIR})
endif()

SET_MIN_VERSIONS()
set(LibName data_binify)
project(${LibName})

set(Interface
		binify.h
		writehelper.hpp
		)

set(Src
		ast.h
		binify.h
		binify.cpp
		writehelper.cpp

		FlexLexer.h
		parser.cpp
		parser.hpp
		scanner.cpp
		scanner.hpp
		scanner.h
)

set(Deps
		al2o3_platform
		al2o3_cmath
		al2o3_cadt
		al2o3_vfile
		al2o3_tinystl
		fmt
		)
ADD_LIB(${LibName} "${Interface}" "${Src}" "${Deps}")

set( Tests
		runner.cpp
		test_binify.cpp
		test_writehelper.cpp
		)
set( TestDeps
		utils_simple_logmanager
		al2o3_catch2 )
ADD_LIB_TESTS(${LibName} "${Interface}" "${Tests}" "${TestDeps}")
