add_library(kitty INTERFACE)
target_include_directories(kitty INTERFACE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(kitty INTERFACE liblpsolve)
