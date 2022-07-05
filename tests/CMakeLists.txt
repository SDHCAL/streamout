include(ExternalData)
include(CLI11)

add_executable(Filesystem.test Filesystem.test.cc)
target_link_libraries(Filesystem.test PRIVATE Streamout::Core doctest_with_main)
install(TARGETS Filesystem.test RUNTIME DESTINATION bin/tests)

add_executable(Exception.test Exception.test.cc)
target_link_libraries(Exception.test PRIVATE Streamout::Core doctest_with_main)
install(TARGETS Exception.test RUNTIME DESTINATION bin/tests)

if(BUILD_RAWDATA_INTERFACE)
  # cmake-lint: disable=C0103
  set(ExternalData_URL_TEMPLATES
    "https://github.com/SDHCAL/streamout_test_files/blob/main/SMM_210616_040234_732873.dat?raw=true")
  # pass
  add_executable(RawdataReader.test RawdataReader.test.cc)
  target_link_libraries(RawdataReader.test PRIVATE Streamout::Interface::RawdataReader Streamout::Interface::Dump Streamout::Core CLI11)
  install(TARGETS RawdataReader.test RUNTIME DESTINATION bin/tests LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
  ExternalData_Add_Test(RawdataReader.run NAME RawdataReader.test COMMAND RawdataReader.test -f DATA{data/SMM_210616_040234_732873.dat} -s 20 -e 10)
  ExternalData_Add_Target(RawdataReader.run)
endif()

add_executable(Formatters.test Formatters.test.cc)
target_link_libraries(Formatters.test PRIVATE Streamout::Core)
install(TARGETS Formatters.test RUNTIME DESTINATION bin/tests LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
add_test(NAME Formatters.test COMMAND Formatters.test)

add_executable(Buffer.test Buffer.test.cc)
target_include_directories(Buffer.test PUBLIC include)
target_link_libraries(Buffer.test PUBLIC Streamout::Core doctest_with_main)
install(TARGETS Buffer.test RUNTIME DESTINATION bin/tests LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
add_test(NAME Buffer.test COMMAND Buffer.test)

add_library(DIFdataExample src/DIFdataExample.cc)
target_include_directories(DIFdataExample PUBLIC include)
target_link_libraries(DIFdataExample PUBLIC Streamout::Core)

add_executable(test_exe main_test_exe.cc)
target_link_libraries(test_exe PRIVATE Streamout::Interface::Dump DIFdataExample CLI11)
install(TARGETS test_exe RUNTIME DESTINATION bin/tests LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
add_test(NAME test_exe COMMAND test_exe)

if(BUILD_LCIO_INTERFACE AND NOT MSVC)
  add_executable(test_lcio_exe main_test_lcio_exe.cc)
  target_link_libraries(test_lcio_exe PRIVATE LCIO::LCIO DIFdataExample CLI11)
  install(TARGETS test_lcio_exe RUNTIME DESTINATION bin/tests LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
  add_test(NAME test_lcio_exe COMMAND test_lcio_exe)
endif()

if(BUILD_ROOT_INTERFACE AND BUILD_RAWDATA_INTERFACE)
  add_executable(RawdataROOT.test RawdataROOT.test.cc)
  target_link_libraries(RawdataROOT.test PRIVATE Streamout::Interface::ROOTWriter Streamout::Interface::RawdataReader CLI11)
  install(TARGETS RawdataROOT.test RUNTIME DESTINATION bin/tests LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
  ExternalData_Add_Test(RawdataROOT.run NAME RawdataROOT.test COMMAND RawdataROOT.test -f DATA{data/SMM_210616_040234_732873.dat} -s 20 -e 10)
  ExternalData_Add_Target(RawdataROOT.run)

  add_executable(ROOTchecker.test ROOTchecker.test.cc)
  target_compile_features(ROOTchecker.test PUBLIC ${ROOT_CXX_STANDARD})
  target_link_libraries(ROOTchecker.test PUBLIC CLI11 doctest_with_main Streamout::Interface::ROOT::Event Streamout::Core)
  install(TARGETS ROOTchecker.test  RUNTIME DESTINATION bin/tests)
endif()
