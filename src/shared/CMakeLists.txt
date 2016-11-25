project(shared CXX)

set(sources
    Common.hpp
    Configuration.cpp
    Configuration.hpp
    )

include_directories(
    ${MYSQL_INCLUDE_DIR}
)
	
add_library(${PROJECT_NAME} STATIC ${sources})

target_link_libraries(${PROJECT_NAME} ${MYSQL_LIBS})
