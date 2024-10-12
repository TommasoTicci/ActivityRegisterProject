cmake_minimum_required(VERSION 3.5)

add_subdirectory(Lib/googletest)
set(gtest_SOURCE_DIR, Lib/googletest/)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(TEST_SOURCE_FILES runAllTests.cpp ActivityFixture.cpp ActivityTest.cpp ActivityRegisterTest.cpp
        ActivityRegisterFixture.cpp)
add_executable(runActivityTest ${TEST_SOURCE_FILES})
target_link_libraries(runActivityTest gtest gtest_main Qt::Core Qt::Gui Qt::Widgets core)