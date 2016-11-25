project(Cestra)
cmake_minimum_required(VERSION 3.0)

if(MSVC)
    set(CMAKE_INSTALL_PREFIX "C:/Cestra" CACHE PATH "Install path prefix" FORCE)
endif()

include(${CMAKE_SOURCE_DIR}/cmake/Platform.cmake)

include(${CMAKE_SOURCE_DIR}/cmake/Modules/FindBoost.cmake)

add_definitions(-DBOOST_ALL_NO_LIB)
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

include_directories(${Boost_INCLUDE_DIRS}) 

add_subdirectory(src)

if(WIN32)
    install(FILES ${INSTALL_DEPS} DESTINATION .)
endif()
