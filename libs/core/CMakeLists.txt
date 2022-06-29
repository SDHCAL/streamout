include(Fmt)
include(Spdlog)
add_library(Core
            src/Bits.cc
            src/Formatters.cc
            src/DIFSlowControl.cc
            src/DIFUnpacker.cc
            src/Buffer.cc
            src/BufferLooperCounter.cc
            src/RawBufferNavigator.cc
)

target_include_directories(Core PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/streamout>)
target_link_libraries(Core PUBLIC spdlog::spdlog fmt::fmt)
target_compile_definitions(Core PRIVATE "-DDATA_FORMAT_VERSION=${DATA_FORMAT_VERSION}")
set_target_properties(Core PROPERTIES PUBLIC_HEADER
    "include/Bits.h;include/Formatters.h;include/DIFSlowControl.h;include/DIFUnpacker.h;include/Buffer.h;include/BufferLooperCounter.h;include/RawBufferNavigator.h"
)
add_library(Streamout::Core ALIAS Core)

install(TARGETS Core spdlog EXPORT streamoutTargets INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/streamout/" PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/streamout/")
