CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT (test_xasm)

set (CMAKE_CXX_FLAGS "-std=c++11")

SET (TEST_SRC
  main.cpp
  )

include_directories(../src)
include_directories(../../3rd/liter)

# locate gtest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(main ${TEST_SRC})
target_link_libraries(main ${GTEST_LIBRARIES} pthread)
