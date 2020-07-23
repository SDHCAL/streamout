cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

set(GITHUB_ORGANIZATION "SDHCAL")
set(PROJECT_NAME "streamout")

project(${PROJECT_NAME} VERSION "0.1.0" DESCRIPTION "Standalone SDHCAL raw data processor" HOMEPAGE_URL "https://github.com/${GITHUB_ORGANIZATION}/${PROJECT_NAME}" LANGUAGES CXX)

configure_file("${CMAKE_SOURCE_DIR}/docs/README.md.in" "${CMAKE_SOURCE_DIR}/README.md" @ONLY)

# Download CMakeCM
set(DOWNLOAD_CMAKECM_URL "https://raw.githubusercontent.com/flagarde/CMakeCM/master/DownloadCMakeCM.cmake" CACHE STRING "URL of the CMakeCM package")
set(DOWNLOAD_CMAKECM_SHA256 "f30eca2c1bc5ff6bc4b314b0abd9b1ea5999ed874e2a710fb0f5ccd5db7b3963" CACHE STRING "SHA256 of DownloadCMakeCM.cmake")
file(DOWNLOAD "${DOWNLOAD_CMAKECM_URL}"
              "${CMAKE_BINARY_DIR}/cmake/CMakeCM/DownloadCMakeCM.cmake"
              INACTIVITY_TIMEOUT 5
              TIMEOUT 1
              EXPECTED_HASH SHA256=${DOWNLOAD_CMAKECM_SHA256})
include(${CMAKE_BINARY_DIR}/cmake/CMakeCM/DownloadCMakeCM.cmake)

# Install Common utilities from CMakeCM
include(Common)

default_install_prefix("${CMAKE_SOURCE_DIR}/bin")
cxx_11()

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_INSTALL_PREFIX}/lib/cmake")

option(BUILD_ROOT_INTERFACE "Flag to toggle building ROOT output" OFF)
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
