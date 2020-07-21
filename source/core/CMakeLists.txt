add_library(core 
            src/DIFSlowControl.cpp 
            src/SDHCAL_buffer.cpp
            src/SDHCAL_buffer_LoopCounter.cpp
            src/SDHCAL_RawBuffer_Navigator.cpp
            src/DIFPtr.cpp
           )
target_include_directories(core PUBLIC include)
target_compile_definitions(core PRIVATE "-DDATA_FORMAT_VERSION=${DATA_FORMAT_VERSION}")
install(TARGETS core RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
