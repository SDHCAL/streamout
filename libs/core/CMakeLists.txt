include(Fmt)
include(Spdlog)

# Generate a version file for streamout
include(Semver)

semverfile(INPUT "${CMAKE_CURRENT_SOURCE_DIR}/include/AppVersion.h.in" OUTPUT "${CMAKE_BINARY_DIR}/generated/AppVersion.h")
add_library(AppVersion INTERFACE)
target_include_directories(AppVersion INTERFACE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/generated/> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/streamout>)
set_target_properties(AppVersion PROPERTIES PUBLIC_HEADER "${CMAKE_BINARY_DIR}/generated/AppVersion.h")

add_library(Core
            src/Version.cc
            src/Bits.cc
            src/Formatters.cc
            src/DIFSlowControl.cc
            src/BufferLooperCounter.cc
            src/RawBufferNavigator.cc
            src/Filesystem.cc
)

target_include_directories(Core PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/streamout>)
target_link_libraries(Core PUBLIC AppVersion spdlog::spdlog fmt::fmt semver::semver)
set_target_properties(Core PROPERTIES PUBLIC_HEADER
    "include/Bits.h;include/Formatters.h;include/DIFSlowControl.h;include/Buffer.h;include/BufferLooperCounter.h;include/RawBufferNavigator.h"
)
add_library(Streamout::Core ALIAS Core)

install(TARGETS Core AppVersion spdlog EXPORT streamoutTargets INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/streamout/" PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/streamout/")
