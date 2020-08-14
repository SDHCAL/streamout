cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

set(GITHUB_ORGANIZATION "SDHCAL")
set(PROJECT_NAME "streamout")

project(${PROJECT_NAME} VERSION "0.1.0" DESCRIPTION "Standalone SDHCAL raw data processor" HOMEPAGE_URL "https://github.com/${GITHUB_ORGANIZATION}/${PROJECT_NAME}" LANGUAGES CXX)

configure_file("${CMAKE_SOURCE_DIR}/docs/README.md.in" "${CMAKE_SOURCE_DIR}/README.md" @ONLY)

# Download CMakeCM
set(CMAKECM_BASE_URL "https://raw.githubusercontent.com/flagarde/CMakeCM/master" CACHE STRING "URL of the CMakeCM package")
set(CMAKECM_SHA256 "a5739917401b3bddbd8a32e280008ed98aa1311cf1f217ac78490c800ed2ff9a" CACHE STRING "SHA256 of CMakeCM")
set(CMAKECM_INSTALL_DIR "${CMAKE_BINARY_DIR}/cmake/CMakeCM" CACHE STRING "Path where to install CMakeCM")
set(CMAKECM_MODULES_DIR "${CMAKECM_INSTALL_DIR}/Modules")
file(DOWNLOAD "${CMAKECM_BASE_URL}/CMakeCM.cmake" "${CMAKECM_INSTALL_DIR}/CMakeCM.cmake" INACTIVITY_TIMEOUT 5 TIMEOUT 1 EXPECTED_HASH SHA256=${CMAKECM_SHA256})
include("${CMAKECM_INSTALL_DIR}/CMakeCM.cmake")

# Install Common utilities from CMakeCM
include(Common)

default_install_prefix("${CMAKE_SOURCE_DIR}/bin")
cxx_14()

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_INSTALL_PREFIX}/lib/cmake")

option(BUILD_ROOT_INTERFACE "Flag to toggle building ROOT output" OFF)
option(BUILD_LCIO_INTERFACE "Flag to toggle building LCIO output" ON)
option(BUILD_TESTS "Build the test" ON)
option(BUILD_APPS "Build the apps" ON)
option(BUILD_DOCS "Build the docs" ON)


## Version of the data
set(DATA_FORMAT_VERSION "13" CACHE STRING "Version of the data format")


if (BUILD_ROOT_INTERFACE)
  message(STATUS "Build with ROOT")
  list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
  find_package(ROOT REQUIRED COMPONENTS Tree)
  ## Is mainly the most important PROPERTY
  get_property(ROOT_CXX_STANDARD TARGET ROOT::Core PROPERTY INTERFACE_COMPILE_FEATURES)
    if(${ROOT_CXX_STANDARD} STREQUAL "cxx_std_14")
      set( CMAKE_CXX_STANDARD 14 )
    elseif(${ROOT_CXX_STANDARD} STREQUAL "cxx_std_17")
      set( CMAKE_CXX_STANDARD 17 )
    elseif(${ROOT_CXX_STANDARD} STREQUAL "cxx_std_20")
      set( CMAKE_CXX_STANDARD 20 )
  endif()
endif()

add_subdirectory(libs)

if(BUILD_APPS)
  add_subdirectory(apps)
endif()

if(BUILD_DOCS)
  add_subdirectory(docs)
endif()

if(BUILD_TESTS)
  include(CTest)
  add_subdirectory(tests)
endif()
