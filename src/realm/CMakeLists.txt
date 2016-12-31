project(realm CXX)

set(sources
    Main.cpp
    Main.hpp
	Master.cpp
	Master.hpp
    RealmConsole.cpp
    RealmConsole.hpp
    stdafx.cpp
    stdafx.hpp
    )

if(WIN32)
   set(sources ${sources} ${CMAKE_CURRENT_SOURCE_DIR}/resources.rc)
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/src/shared
    ${MYSQL_INCLUDE_DIR}
)

find_package(Boost COMPONENTS System Thread REQUIRED)

add_definitions(-DBOOST_ALL_NO_LIB)
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

include_directories(${Boost_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${sources})

set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS TRUE)

target_link_libraries(${PROJECT_NAME} shared ${Boost_LIBRARIES})
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)
