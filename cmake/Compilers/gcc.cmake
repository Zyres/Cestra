include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)

if(NOT COMPILER_SUPPORTS_CXX11)
    message(FATAL_ERROR "${CMAKE_CXX_COMPILER} does not support C++11 feature")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
