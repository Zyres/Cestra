project(game CXX)

set(sources
    Main.cpp
    Main.hpp
	Master.cpp
	Master.hpp
    stdafx.cpp
    stdafx.hpp
    )

include_directories(
   ${CMAKE_SOURCE_DIR}/src/shared
)
add_executable(${PROJECT_NAME} ${sources})

set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS TRUE)

target_link_libraries(${PROJECT_NAME} shared)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)
