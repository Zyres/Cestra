find_package(MySQL REQUIRED)

if(CMAKE_COMPILER_IS_GNUCXX)
    include(${CMAKE_SOURCE_DIR}/cmake/Compilers/gcc.cmake)
else()
    message(FATAL_ERROR "Compiler not supported")
endif()
