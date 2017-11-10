# No debugging and optimization for examples
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -DNDEBUG")

add_executable(isop isop.cpp)
add_executable(npn_enumeration npn_enumeration.cpp)
add_executable(npn_random npn_random.cpp)
add_executable(spectral_enumeration spectral_enumeration.cpp)
