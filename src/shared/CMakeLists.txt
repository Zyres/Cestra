project(shared CXX)

set(sources
    Common.hpp
    Configuration.cpp
    Configuration.hpp
    Database.cpp
    Database.hpp
    FileLog.cpp
    FileLog.hpp
    SocketMgr.cpp
    SocketMgr.hpp
    )

include_directories(
    ${MYSQL_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
)
	
add_library(${PROJECT_NAME} STATIC ${sources})

link_directories(${Boost_LIBRARY_DIR})
target_link_libraries(${PROJECT_NAME} ${MYSQL_LIBS})
