add_library(DIFdataExample src/DIFdataExample.cc)
target_include_directories(DIFdataExample PUBLIC include)
target_link_libraries(DIFdataExample PUBLIC core)


add_executable(test_exe main_test_exe.cc)
target_link_libraries(test_exe PRIVATE standard PRIVATE DIFdataExample )
install(TARGETS test_exe RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
add_test(NAME test_exe COMMAND test_exe)

if (BUILD_ROOT_INTERFACE)
  add_executable(test_root_exe main_test_root_exe.cc)
  target_link_libraries(test_root_exe PRIVATE root PRIVATE DIFdataExample)
  install(TARGETS test_root_exe RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
  add_test(NAME test_root_exe COMMAND test_root_exe)
endif()
