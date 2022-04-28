cmake_minimum_required(VERSION 3.15)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Download CMakeCM and include it as soon as possible
include(GetCMakeMM)
cmmm(VERSION "2.3" REPOSITORY "flagarde/CMakeMM" VERBOSITY VERBOSE)

cmmm_modules_list(PROVIDER github REPOSITORY "SDHCAL/CMakeModules" BRANCH main DESTINATION "Modules")

project(SDHCAL VERSION "0.1.0" DESCRIPTION "Standalone SDHCAL raw data processor" HOMEPAGE_URL "https://github.com/SDHCAL/streamout" LANGUAGES CXX)

#set(CMAKE_MACOSX_RPATH TRUE)
#set(CMAKE_SKIP_BUILD_RPATH FALSE)
#set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}" "${CMAKE_INSTALL_PREFIX}/lib")
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#link_directories("${CMAKE_INSTALL_PREFIX}/lib")

include(DefaultInstallPrefix)
default_install_prefix("${CMAKE_SOURCE_DIR}/binaries")

include(Standards)
cxx_14()

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_INSTALL_PREFIX}/lib/cmake")

option(BUILD_ROOT_INTERFACE "Flag to toggle building ROOT output" OFF)
option(BUILD_LCIO_INTERFACE "Flag to toggle building LCIO output" ON)
option(BUILD_TESTS "Build the test" ON)
option(BUILD_APPS "Build the apps" ON)
option(BUILD_DOCS "Build the docs" OFF)

## Version of the data
set(DATA_FORMAT_VERSION "13" CACHE STRING "Version of the data format")

if(BUILD_ROOT_INTERFACE)
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
  include(Doctest)
  add_subdirectory(tests)
endif()
