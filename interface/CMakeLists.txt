# standard


set (local_include ${CMAKE_CURRENT_SOURCE_DIR}/standard/include)
set (local_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/standard/src/DIFdataExample.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/standard/src/textDump.cc
  )


if (BUILD_ROOT_INTERFACE)
  set (root_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/root/src/ROOTtreeDest.cc
    )
  set (root_include ${CMAKE_CURRENT_SOURCE_DIR}/root/include)
endif()

set ( interface_sources ${local_sources} ${root_sources} PARENT_SCOPE)
set ( interface_include ${local_include} ${root_include} PARENT_SCOPE)

add_executable (test_exe standard/src/main_test_exe.cc)
target_include_directories(test_exe PUBLIC ${local_include})
target_link_libraries (test_exe ${PROJECT_NAME})

if (BUILD_ROOT_INTERFACE)
  add_executable (test_root_exe root/src/main_test_root_exe.cc)
  target_include_directories(test_root_exe PUBLIC ${local_include} ${root_include})
  target_link_libraries (test_root_exe ${PROJECT_NAME} ${ROOT_LIBRARIES})
endif()

