include(ExternalData)
include(CLI11)

if(BUILD_RAWDATA_INTERFACE)
  # cmake-lint: disable=C0103
  set(ExternalData_URL_TEMPLATES
    "https://github.com/SDHCAL/streamout_test_files/blob/main/SMM_210616_040234_732873.dat?raw=true")
  # pass
  add_executable(RawdataReader.test RawdataReader.test.cc)
  target_link_libraries(RawdataReader.test PRIVATE Streamout::Interface::RawdataReader Streamout::Interface::Dump Streamout::Core CLI11)
  install(TARGETS RawdataReader.test RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
  ExternalData_Add_Test(RawdataReader.run NAME RawdataReader.test COMMAND RawdataReader.test -f DATA{data/SMM_210616_040234_732873.dat} -s 20 -v warn )
  ExternalData_Add_Target(RawdataReader.run)
endif()

add_executable(Formatters.test Formatters.test.cc)
target_link_libraries(Formatters.test PRIVATE Streamout::Core)
install(TARGETS Formatters.test RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
add_test(NAME Formatters.test COMMAND Formatters.test)

add_executable(Buffer.test Buffer.test.cc)
target_include_directories(Buffer.test PUBLIC include)
target_link_libraries(Buffer.test PUBLIC Streamout::Core doctest_with_main)
install(TARGETS Buffer.test RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
add_test(NAME Buffer.test COMMAND Buffer.test)

add_library(DIFdataExample src/DIFdataExample.cc)
target_include_directories(DIFdataExample PUBLIC include)
target_link_libraries(DIFdataExample PUBLIC Streamout::Core)

add_executable(test_exe main_test_exe.cc)
target_link_libraries(test_exe PRIVATE Streamout::Interface::Dump DIFdataExample CLI11)
install(TARGETS test_exe RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
add_test(NAME test_exe COMMAND test_exe)

if(BUILD_ROOT_INTERFACE)
  add_executable(test_root_exe main_test_root_exe.cc)
  target_link_libraries(test_root_exe PRIVATE Streamout::Interface::ROOT DIFdataExample CLI11)
  install(TARGETS test_root_exe RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
  add_test(NAME test_root_exe COMMAND test_root_exe)
endif()

if(BUILD_LCIO_INTERFACE AND NOT MSVC)
  add_executable(test_lcio_exe main_test_lcio_exe.cc)
  target_link_libraries(test_lcio_exe PRIVATE LCIO::LCIO DIFdataExample CLI11)
  install(TARGETS test_lcio_exe RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
  add_test(NAME test_lcio_exe COMMAND test_lcio_exe)
endif()
