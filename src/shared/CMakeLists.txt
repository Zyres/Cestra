project(shared CXX)

set(sources
    Common.hpp
    Configuration.cpp
    Configuration.hpp
    )

add_library(${PROJECT_NAME} STATIC ${sources})

target_link_libraries(${PROJECT_NAME})
