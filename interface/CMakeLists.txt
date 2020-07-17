# standard


set (local_include ${CMAKE_CURRENT_SOURCE_DIR}/standard/include)
set (local_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/standard/src/DIFdataExample.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/standard/src/textDump.cc
  )



set ( interface_sources ${local_sources} PARENT_SCOPE)
set ( interface_include ${local_include} PARENT_SCOPE)

add_executable (test_exe standard/src/main_test_exe.cc)
target_include_directories(test_exe PUBLIC ${local_include})
target_link_libraries (test_exe ${PROJECT_NAME})
