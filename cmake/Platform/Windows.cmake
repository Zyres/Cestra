if(IS_64BIT)
    set(MYSQL_LIBS ${CMAKE_SOURCE_DIR}/dep/lib/x64/libmysql.lib CACHE INTERNAL "MySQL libs." FORCE)
else()
    set(MYSQL_LIBS ${CMAKE_SOURCE_DIR}/dep/lib/x86/libmysql.lib CACHE INTERNAL "MySQL libs." FORCE)
endif()

set(MYSQL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dep/mysql CACHE INTERNAL "MySQL dir." FORCE)

if(IS_64BIT)
   set(DEP_DLLS ${CMAKE_SOURCE_DIR}/dep/dll/x64)
else()
   set(DEP_DLLS ${CMAKE_SOURCE_DIR}/dep/dll/x86)
endif()

set(INSTALL_DEPS ${DEP_DLLS}/libmysql.dll)

if(MSVC)
    include(${CMAKE_SOURCE_DIR}/cmake/Compilers/msvc.cmake)
else()
    message(FATAL_ERROR "Compiler not supported")
endif()
