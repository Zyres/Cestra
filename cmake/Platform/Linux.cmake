set(MYSQL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dep/mysql CACHE INTERNAL "MySQL dir." FORCE)

if(CMAKE_COMPILER_IS_GNUCXX)
    include(${CMAKE_SOURCE_DIR}/cmake/Compilers/gcc.cmake)
else()
    message(FATAL_ERROR "Compiler not supported")
endif()
