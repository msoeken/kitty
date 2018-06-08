add_subdirectory(googletest/googletest)

include_directories(googletest/googletest/include)

file(GLOB FILENAMES *.cpp)

add_executable(run_tests ${FILENAMES})
target_link_libraries(run_tests gtest kitty)
target_compile_options(run_tests PRIVATE -W -Wall -Wextra -g -O0)

if(win32)
  target_compile_definitions(run_tests PRIVATE GTEST_HAS_TR1_TUPLE=0)
endif()

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-ftime-report" HAS_FTIME_REPORT)
if(HAS_FTIME_REPORT)
  add_executable(run_tests_report ${FILENAMES})
  target_link_libraries(run_tests_report gtest kitty)
  target_compile_options(run_tests_report PRIVATE -ftime-report)
endif()
