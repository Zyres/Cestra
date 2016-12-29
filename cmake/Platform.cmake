if(UNIX)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "[xX]64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "[xX]86_64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "[aA][mM][dD]64" )
        set(IS_64BIT TRUE)
    else()
        set(IS_64BIT FALSE)
    endif()
else()
    if(CMAKE_GENERATOR MATCHES Win64*)
        set(IS_64BIT TRUE)
    else()
        set(IS_64BIT FALSE)
    endif()
endif()

if(IS_64BIT)
    message(STATUS "Detected 64 bit system")
endif()

set(CMAKE_MODULE_PATH 
    ${CMAKE_MODULE_PATH}
    ${CMAKE_SOURCE_DIR}/cmake/Modules
)

find_package(Boost)

add_definitions(-DBOOST_ALL_NO_LIB)
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

include_directories(${Boost_INCLUDE_DIR})

if(WIN32)
    include(${CMAKE_SOURCE_DIR}/cmake/Platform/Windows.cmake)
elseif(UNIX)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Linux") 
        include(${CMAKE_SOURCE_DIR}/cmake/Platform/Linux.cmake) 
    else()
        message(FATAL_ERROR "Platform not supported.") 
    endif()
else()
    message(FATAL_ERROR "Platform not supported.")
endif()
