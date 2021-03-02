cmake_minimum_required(VERSION 3.10)
set(GTEST_PROJECT TestControlEngine)

enable_testing()

if (NOT BUILD_TEST_DEPS)
    message("Gtest Cmake")
    find_package(GTest REQUIRED)
endif ()

file(GLOB UTIL_SRC "../Source/Library/Utils/**.cpp")

file(GLOB DC_TST "./*.cpp" /
        "./Library/Utils/*.cpp")

list(APPEND DC_SRC ${DC_TST})
list(FILTER DC_SRC EXCLUDE REGEX ".*ControlEngine.cpp$")
list(FILTER DC_SRC EXCLUDE REGEX ".*TestClient.cpp$")
list(FILTER DC_SRC EXCLUDE REGEX ".*GTestMain.cpp$")


add_executable(${GTEST_PROJECT} "GTestMain.cpp" ${DC_SRC})
target_link_libraries(${GTEST_PROJECT} gtest gtest_main)

add_executable("TestClient" "./Library/Clients/TestClient.cpp" ${UTIL_SRC})

add_test(${GTEST_PROJECT} ${GTEST_PROJECT})
