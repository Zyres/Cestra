if(IS_64BIT)
    set(MYSQL_LIBS ${CMAKE_SOURCE_DIR}/dep/mysql-connector/win64/lib/mysqlcppconn.lib CACHE INTERNAL "MySQL libs." FORCE)
else()
    set(MYSQL_LIBS ${CMAKE_SOURCE_DIR}/dep/mysql-connector/win32/lib/mysqlcppconn.lib CACHE INTERNAL "MySQL libs." FORCE)
endif()

set(BOOST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dep/boost CACHE INTERNAL "Boost dir." FORCE)

if(IS_64BIT)
    set(MYSQL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dep/mysql-connector/win64/include CACHE INTERNAL "MySQL dir." FORCE)
else()
    set(MYSQL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dep/mysql-connector/win32/include CACHE INTERNAL "MySQL dir." FORCE)
endif()

if(IS_64BIT)
   set(DEP_DLLS ${CMAKE_SOURCE_DIR}/dep/mysql-connector/win64/lib)
else()
   set(DEP_DLLS ${CMAKE_SOURCE_DIR}/dep/mysql-connector/win32/lib)
endif()

set(INSTALL_DEPS ${DEP_DLLS}/mysqlcppconn.dll)

if(MSVC)
    include(${CMAKE_SOURCE_DIR}/cmake/Compilers/msvc.cmake)
else()
    message(FATAL_ERROR "Compiler not supported")
endif()
