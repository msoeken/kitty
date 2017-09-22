add_subdirectory(googletest/googletest)

include_directories(googletest/googletest/include)

file(GLOB FILENAMES *.cpp)

add_executable(run_tests ${FILENAMES})
target_link_libraries(run_tests gtest)
