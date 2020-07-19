# standard
add_library(standard
            standard/src/textDump.cc
           )
target_include_directories(standard PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/standard/include")
target_link_libraries(standard PUBLIC core)
install(TARGETS standard RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)

if(BUILD_ROOT_INTERFACE)
  add_library(root
              "${CMAKE_CURRENT_SOURCE_DIR}/root/src/ROOTtreeDest.cc"
             )
  target_include_directories(root PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/root/include" PUBLIC ${ROOT_INCLUDE_DIRS})
  target_link_libraries(root PUBLIC core PUBLIC ${ROOT_LIBRARIES} )
  install(TARGETS root RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
endif()
