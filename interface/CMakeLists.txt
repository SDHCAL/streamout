# standard


set (local_include ${CMAKE_CURRENT_SOURCE_DIR}/standard/include)
set (local_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/standard/src/DIFdataExample.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/standard/src/textDump.cc
  )



set ( interface_sources ${local_sources} PARENT_SCOPE)
set ( interface_include ${local_include} PARENT_SCOPE)
