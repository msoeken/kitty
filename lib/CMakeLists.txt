if (NOT TARGET liblpsolve)
  add_subdirectory(lpsolve)
  target_include_directories(liblpsolve SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lpsolve)
endif()
