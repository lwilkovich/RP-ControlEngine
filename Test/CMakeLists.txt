cmake_minimum_required(VERSION 3.10)
set(GTEST_PROJECT TestControlEngine)

enable_testing()

if (NOT BUILD_TEST_DEPS)
    message("Gtest Cmake")
    find_package(GTest REQUIRED)
endif ()

file(GLOB UTIL_SRC "../source/util/*.cpp")

file(GLOB CE_SRC "Source/*.cpp" /
    "../Source/**/*.cpp"
    "../Source/**/**/*.cpp"
    "../Source/**/*.hpp"
    "../Source/*.hpp"
    ${UTIL_SRC})

file(GLOB DC_TST "./*.cpp" /
        "./Library/Utils/*.cpp")

list(APPEND DC_SRC ${DC_TST})
list(FILTER DC_SRC EXCLUDE REGEX ".*ControlEngine.cpp$")

set(HEADERS)
add_executable(${GTEST_PROJECT} ${DC_SRC})

target_link_libraries(${GTEST_PROJECT} gtest gtest_main)
target_link_libraries(${GTEST_PROJECT} ${DEP_DC_LIBS})

add_test(${GTEST_PROJECT} ${GTEST_PROJECT})