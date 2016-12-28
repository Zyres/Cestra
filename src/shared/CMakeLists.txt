project(shared CXX)

set(sources
    Common.hpp
    Configuration.cpp
    Configuration.hpp
    FileLog.cpp
    FileLog.hpp
    )

include_directories(
    ${MYSQL_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
)
	
add_library(${PROJECT_NAME} STATIC ${sources})

link_directories(${Boost_LIBRARY_DIR})
target_link_libraries(${PROJECT_NAME} ${MYSQL_LIBS} ${Boost_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY})
