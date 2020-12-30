cmake_minimum_required(VERSION 3.10)
project(ControlEngine)

file(GLOB CE_SRC "Source/*.cpp" /
    "Source/**/*.cpp"
    "Source/**/**/*.cpp"
    "Source/**/*.hpp"
    "Source/*.hpp")

set(CE_PROJECT_NAME ControlEngine)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

add_executable(${CE_PROJECT_NAME} ${CE_SRC})
set_target_properties(${CE_PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(${CE_PROJECT_NAME} PRIVATE "-DDEBUG_BUILD")

if (MSVC)
    target_compile_options(${CE_PROJECT_NAME} PRIVATE /W4 /WX)
else ()
    target_compile_options(${CE_PROJECT_NAME} PRIVATE -Wall -Wno-long-long -pedantic -Werror)
endif ()
