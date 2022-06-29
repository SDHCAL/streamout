add_subdirectory(core)

add_subdirectory(interface)

include(CMakePackageConfigHelpers)

configure_package_config_file("${PROJECT_SOURCE_DIR}/cmake/streamoutConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/streamoutConfig.cmake" INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/streamout")
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/streamoutVersion.cmake" COMPATIBILITY SameMajorVersion)

install(EXPORT streamoutTargets FILE streamoutTargets.cmake NAMESPACE streamout:: DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/streamout")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/streamoutConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/streamoutVersion.cmake" DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/streamout")
export(EXPORT streamoutTargets FILE "${PROJECT_BINARY_DIR}/cmake/streamoutConfig.cmake" NAMESPACE streamout::)
