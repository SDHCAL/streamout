add_library(core
            src/DIFSlowControl.cc
            src/DIFUnpacker.cc
            src/SDHCAL_buffer.cc
            src/SDHCAL_buffer_LoopCounter.cc
            src/SDHCAL_RawBuffer_Navigator.cc
            src/DIFPtr.cc)
target_include_directories(core PUBLIC include)
target_compile_definitions(core PRIVATE "-DDATA_FORMAT_VERSION=${DATA_FORMAT_VERSION}")
install(TARGETS core RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
