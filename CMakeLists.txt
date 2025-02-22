cmake_minimum_required(VERSION 3.12)
project(GameOfLife)

find_package(raylib REQUIRED)

# Adding our source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.c")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_LIST_DIR}/src/*.h")

# Declaring our executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${HEADERS})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
