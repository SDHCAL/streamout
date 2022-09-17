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

add_executable(DumpReaderDumpWriter DumpReaderDumpWriter.cc src/DIFdataExample.cc)
target_include_directories(DumpReaderDumpWriter PUBLIC include)
target_link_libraries(DumpReaderDumpWriter PRIVATE Streamout::Interface::Dump CLI11)
install(TARGETS DumpReaderDumpWriter RUNTIME DESTINATION bin/tests LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
add_test(NAME DumpReaderDumpWriter COMMAND DumpReaderDumpWriter)

if(BUILD_LCIO_INTERFACE AND NOT MSVC AND BUILD_RAWDATA_INTERFACE)
  add_executable(RawdataLCIO.test RawdataLCIO.test.cc)
  target_link_libraries(RawdataLCIO.test PRIVATE LCIO::LCIO CLI11 Streamout::Interface::LCIOWriter Streamout::Interface::RawdataReader)
  install(TARGETS RawdataLCIO.test RUNTIME DESTINATION bin/tests LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
  ExternalData_Add_Test(RawdataLCIO.run NAME RawdataLCIO.test COMMAND RawdataLCIO.test -f DATA{data/SMM_210616_040234_732873.dat} -s 20 -e 10)
  ExternalData_Add_Target(RawdataLCIO.run)
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

  add_executable(ROOT_IJK.test ROOT_IJK.test.cc)
  target_compile_features(ROOT_IJK.test PUBLIC ${ROOT_CXX_STANDARD})
  target_link_libraries(ROOT_IJK.test PUBLIC CLI11 doctest_with_main Streamout::Interface::ROOT::Event Streamout::Core)
  install(TARGETS ROOT_IJK.test  RUNTIME DESTINATION bin/tests)
endif()

add_subdirectory(GGstreamout)
