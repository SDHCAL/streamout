CMAKE_MINIMUM_REQUIRED(VERSION 3.1 FATAL_ERROR)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

# project name
PROJECT(streamout)

# project version
SET( ${PROJECT_NAME}_VERSION_MAJOR 0 )
SET( ${PROJECT_NAME}_VERSION_MINOR 1 )
SET( ${PROJECT_NAME}_VERSION_PATCH 0 )

include_directories(core/include)

set (core_sources
  core/src/DIFSlowControl.cc
  core/src/DIFUnpacker.cc
  core/src/SDHCAL_buffer.cc
  )

add_subdirectory(interface)

message ("RECEIVES " ${interface_include})
include_directories(${interface_include})

add_library( ${PROJECT_NAME} ${core_sources} ${interface_sources})
install (TARGETS ${PROJECT_NAME} DESTINATION lib)
