list(APPEND SRC_LIST MatrixCSR.cpp MatrixCOO.cpp MatrixELL.cpp MatrixDENSE.cpp MatrixSCOO.cpp Matrix.cpp)

add_library(tbsla SHARED ${SRC_LIST})
target_include_directories(tbsla
  PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/cpp/include>
  PUBLIC $<INSTALL_INTERFACE:include>
)
target_link_libraries(tbsla PUBLIC tbsla_utils)
install(TARGETS tbsla)

if(TBSLA_ENABLE_OMP)
  add_library(tbsla_omp SHARED ${SRC_LIST})
  target_include_directories(tbsla_omp
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/cpp/include>
    PUBLIC $<INSTALL_INTERFACE:include>
  )
  target_link_libraries(tbsla_omp PUBLIC OpenMP::OpenMP_CXX)
  target_link_libraries(tbsla_omp PUBLIC tbsla_utils)
  target_compile_definitions(tbsla_omp PUBLIC TBSLA_COMPILED_WITH_OMP)
  install(TARGETS tbsla_omp)
endif()

install(DIRECTORY ${PROJECT_SOURCE_DIR}/cpp/include/tbsla DESTINATION "include")

