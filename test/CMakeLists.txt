include_directories(googletest/googletest/include)
link_directories(googletest/build/googlemock/gtest)

file(GLOB FILENAMES *.cpp)

add_executable(run_tests ${FILENAMES})
target_link_libraries(run_tests gtest)
