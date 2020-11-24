project(liblpsolve LANGUAGES C CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lpsolve)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lpsolve/colamd)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lpsolve/shared)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lpsolve/bfp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lpsolve/bfp/bfp_LUSOL)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lpsolve/bfp/bfp_LUSOL/LUSOL)

set(FILENAMES
  "lpsolve/lp_MDO.c"
  "lpsolve/shared/commonlib.c"
  "lpsolve/colamd/colamd.c"
  "lpsolve/shared/mmio.c"
  "lpsolve/shared/myblas.c"
  "lpsolve/ini.c"
  "lpsolve/fortify.c"
  "lpsolve/lp_rlp.c"
  "lpsolve/lp_crash.c"
  "lpsolve/bfp/bfp_LUSOL/lp_LUSOL.c"
  "lpsolve/bfp/bfp_LUSOL/LUSOL/lusol.c"
  "lpsolve/lp_Hash.c"
  "lpsolve/lp_lib.c"
  "lpsolve/lp_wlp.c"
  "lpsolve/lp_matrix.c"
  "lpsolve/lp_mipbb.c"
  "lpsolve/lp_MPS.c"
  "lpsolve/lp_params.c"
  "lpsolve/lp_presolve.c"
  "lpsolve/lp_price.c"
  "lpsolve/lp_pricePSE.c"
  "lpsolve/lp_report.c"
  "lpsolve/lp_scale.c"
  "lpsolve/lp_simplex.c"
  "lpsolve/lp_SOS.c"
  "lpsolve/lp_utils.c"
  "lpsolve/yacc_read.c")

add_compile_options(-w -DYY_NEVER_INTERACTIVE -DPARSER_LP -DINVERSE_ACTIVE=INVERSE_LUSOL -DRoleIsExternalInvEngine)
add_library(liblpsolve STATIC ${FILENAMES})
if(UNIX)
  target_link_libraries(liblpsolve m dl)
endif()