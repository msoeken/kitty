# No debugging and optimization for examples
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -DNDEBUG")

add_executable(npn_enumeration npn_enumeration.cpp)
add_executable(npn_random npn_random.cpp)
