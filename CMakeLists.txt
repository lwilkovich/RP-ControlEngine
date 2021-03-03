cmake_minimum_required(VERSION 3.10)
project(ControlEngine)

option(BUILD_TEST_DEPS "" ON)

file(GLOB CE_SRC "Source/*.cpp" /
    "Source/**/*.cpp"
    "Source/**/**/*.cpp"
    "Source/**/*.hpp"
    "Source/*.hpp")

set(CE_PROJECT_NAME ControlEngine)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -g3")

add_executable(${CE_PROJECT_NAME} ${CE_SRC})
set_target_properties(${CE_PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(${CE_PROJECT_NAME} PRIVATE "-DDEBUG_BUILD")

if (MSVC)
    target_compile_options(${CE_PROJECT_NAME} PRIVATE /W4 /WX)
else ()
    target_compile_options(${CE_PROJECT_NAME} PRIVATE -Wall -Wno-long-long -pedantic -Werror)
endif ()

if (BUILD_TEST_DEPS)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    configure_file(CMakeLists.txt.gtest
            googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
    add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
            ${CMAKE_BINARY_DIR}/googletest-build)
endif ()

add_subdirectory(Test)