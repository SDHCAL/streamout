add_library(Core
            src/Bits.cc
            src/DIFSlowControl.cc
            src/DIFUnpacker.cc
						src/Buffer.cc
            src/SDHCAL_buffer_LoopCounter.cc
            src/SDHCAL_RawBuffer_Navigator.cc
            src/DIFPtr.cc)
target_include_directories(Core PUBLIC include)
target_compile_definitions(Core PRIVATE "-DDATA_FORMAT_VERSION=${DATA_FORMAT_VERSION}")
add_library(Streamout::Core ALIAS Core)
install(TARGETS Core RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
