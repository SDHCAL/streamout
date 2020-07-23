cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

# project name
project(streamout VERSION "0.1.0" LANGUAGES CXX)

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

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_ROOT_INTERFACE "Flag to toggle building ROOT output" OFF)
option(BUILD_TEST "Build the test" ON)

## Version of the data
set(DATA_FORMAT_VERSION "13" CACHE STRING "Version of the data format")


if (BUILD_ROOT_INTERFACE)
  message(STATUS "Build with ROOT")
  list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
  find_package(ROOT REQUIRED COMPONENTS Tree)
  ## Is mainly the most important PROPERTY
  get_property(ROOT_CXX_STANDARD TARGET ROOT::Core PROPERTY INTERFACE_COMPILE_FEATURES)
    if(${ROOT_CXX_STANDARD} STREQUAL "cxx_std_14") 
      set( CMAKE_CXX_STANDARD 17 )
    elseif(${ROOT_CXX_STANDARD} STREQUAL "cxx_std_17") 
      set( CMAKE_CXX_STANDARD 17 )
    elseif(${ROOT_CXX_STANDARD} STREQUAL "cxx_std_20")  
      set( CMAKE_CXX_STANDARD 20 )
  endif()
endif()

add_subdirectory(core)

add_subdirectory(interface)

if(BUILD_TEST)
  include(CTest)
  add_subdirectory(tests)
endif()
