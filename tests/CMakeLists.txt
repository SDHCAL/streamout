include(CLI11_project)

add_library(DIFdataExample src/DIFdataExample.cc)
target_include_directories(DIFdataExample PUBLIC include)
target_link_libraries(DIFdataExample PUBLIC core)

add_executable(test_exe main_test_exe.cc)
target_link_libraries(test_exe PRIVATE standard PRIVATE DIFdataExample PRIVATE streamout::CLI11)
install(TARGETS test_exe RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
add_test(NAME test_exe COMMAND test_exe)

if (BUILD_ROOT_INTERFACE)
  add_executable(test_root_exe main_test_root_exe.cc)
  target_link_libraries(test_root_exe PRIVATE root PRIVATE DIFdataExample PRIVATE streamout::CLI11)
  install(TARGETS test_root_exe RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
  add_test(NAME test_root_exe COMMAND test_root_exe)
endif()

if (BUILD_LCIO_INTERFACE)
  add_executable(test_lcio_exe main_test_lcio_exe.cc)
  target_link_libraries(test_lcio_exe PRIVATE streamout::LCIO PRIVATE DIFdataExample PRIVATE streamout::CLI11)
  install(TARGETS test_lcio_exe RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
  add_test(NAME test_lcio_exe COMMAND test_lcio_exe)
endif()
