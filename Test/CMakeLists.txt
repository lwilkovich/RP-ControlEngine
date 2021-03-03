cmake_minimum_required(VERSION 3.10)
set(GTEST_PROJECT TestControlEngine)

enable_testing()

if (NOT BUILD_TEST_DEPS)
    message("Gtest Cmake")
    find_package(GTest REQUIRED)
endif ()

file(GLOB UTIL_SRC "../Source/Library/Utils/**.cpp")

file(GLOB CE_TST "./*.cpp" /
        "./Library/Utils/*.cpp")

list(APPEND CE_SRC ${CE_TST})
list(FILTER CE_SRC EXCLUDE REGEX ".*ControlEngine.cpp$")
list(FILTER CE_SRC EXCLUDE REGEX ".*TestClient.cpp$")
list(FILTER CE_SRC EXCLUDE REGEX ".*GTestMain.cpp$")


add_executable(${GTEST_PROJECT} "GTestMain.cpp" ${CE_SRC})
target_link_libraries(${GTEST_PROJECT} gtest gtest_main)

add_executable("TestClient" "./Library/Clients/TestClient.cpp" ${UTIL_SRC})

add_test(${GTEST_PROJECT} ${GTEST_PROJECT})
