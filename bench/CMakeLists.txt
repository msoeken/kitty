file(GLOB FILENAMES *.cpp)

# No debugging and full optimization for benchmark test
set(CMAKE_CXX_FLAGS "-O3 -DNODEBUG")

set(BENCHMARK_ENABLE_TESTING "OFF")
set(CMAKE_BUILD_TYPE "Release")
add_subdirectory(benchmark)

foreach(FILENAME ${FILENAMES})
  get_filename_component(BASENAME ${FILENAME} NAME_WE)

  set(TARGET_NAME bench_${BASENAME})

  add_executable(${TARGET_NAME} ${FILENAME})
  target_link_libraries(${TARGET_NAME} benchmark)
endforeach()

