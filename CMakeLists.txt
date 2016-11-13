project(Cestra)
cmake_minimum_required(VERSION 3.0)

if(MSVC)
    set(CMAKE_INSTALL_PREFIX "C:/Cestra" CACHE PATH "Install path prefix" FORCE)
endif()

include(${CMAKE_SOURCE_DIR}/cmake/Platform.cmake)

add_subdirectory(src)
