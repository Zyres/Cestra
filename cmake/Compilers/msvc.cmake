if(MSVC_VERSION VERSION_LESS 19.0.24210) #2015 3
    message(FATAL_ERROR "This Visual studio version is not supported")
endif()

message(STATUS "Applying settings for ${CMAKE_CXX_COMPILER_ID}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
