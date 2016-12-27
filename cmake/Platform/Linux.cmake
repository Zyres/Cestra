if(IS_64BIT)
    set(MYSQL_LIBS ${CMAKE_SOURCE_DIR}/dep/lib/x64/libmysql.lib CACHE INTERNAL "MySQL libs." FORCE)
else()
    set(MYSQL_LIBS ${CMAKE_SOURCE_DIR}/dep/lib/x86/libmysql.lib CACHE INTERNAL "MySQL libs." FORCE)
endif()

set(MYSQL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dep/mysql CACHE INTERNAL "MySQL dir." FORCE)

if(CMAKE_COMPILER_IS_GNUCXX)
    include(${CMAKE_SOURCE_DIR}/cmake/Compilers/gcc.cmake)
else()
    message(FATAL_ERROR "Compiler not supported")
endif()
