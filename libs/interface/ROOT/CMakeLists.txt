list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
if(NOT $ENV{ROOTSYS} STREQUAL "")
  set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" "$ENV{ROOTSYS}")
  set(ROOT_DIR "$ENV{ROOTSYS}")
endif()

find_package(ROOT REQUIRED COMPONENTS Core Tree)
include("${ROOT_USE_FILE}")
string(REPLACE "-L " "-L" ROOT_EXE_LINKER_FLAGS "${ROOT_EXE_LINKER_FLAGS}")
# cmake-lint: disable=E1120
separate_arguments(ROOT_EXE_LINKER_FLAGS)
# pass
include_directories(${ROOT_INCLUDE_DIRS})

missive(WARN "Build with ROOT interface")
get_property(ROOT_CXX_STANDARD TARGET ROOT::Core PROPERTY INTERFACE_COMPILE_FEATURES)

root_generate_dictionary(G__Hit "${CMAKE_CURRENT_SOURCE_DIR}/include/Hit.h" LINKDEF "${CMAKE_CURRENT_SOURCE_DIR}/include/HitLinkDef.h")
add_library(Hit OBJECT "src/Hit.cc" G__Hit.cxx)
target_compile_features(Hit PUBLIC ${ROOT_CXX_STANDARD})
target_include_directories(Hit PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/streamout>)
target_link_libraries(Hit PUBLIC ROOT::Core)
set_target_properties(Hit PROPERTIES PUBLIC_HEADER "include/Hit.h;include/HitLinkDef.h")
add_library(HitStatic STATIC $<TARGET_OBJECTS:Hit>)
add_library(HitShared SHARED $<TARGET_OBJECTS:Hit>)
add_library(Streamout::Interface::ROOT::Hit ALIAS HitStatic)

root_generate_dictionary(G__DIF "${CMAKE_CURRENT_SOURCE_DIR}/include/DIF.h" LINKDEF "${CMAKE_CURRENT_SOURCE_DIR}/include/DIFLinkDef.h")
add_library(DIF OBJECT "src/DIF.cc" G__DIF.cxx)
target_compile_features(DIF PUBLIC ${ROOT_CXX_STANDARD})
target_include_directories(DIF PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/streamout>)
target_link_libraries(DIF PUBLIC ROOT::Core HitStatic)
set_target_properties(DIF PROPERTIES PUBLIC_HEADER "include/DIF.h;include/DIFLinkDef.h")
add_library(DIFStatic STATIC $<TARGET_OBJECTS:DIF>)
add_library(DIFShared SHARED $<TARGET_OBJECTS:DIF>)
add_library(Streamout::Interface::ROOT::DIF ALIAS DIFStatic)

root_generate_dictionary(G__Event "${CMAKE_CURRENT_SOURCE_DIR}/include/Event.h" LINKDEF "${CMAKE_CURRENT_SOURCE_DIR}/include/EventLinkDef.h")
add_library(Event OBJECT "src/Event.cc" G__Event.cxx)
target_compile_features(Event PUBLIC ${ROOT_CXX_STANDARD})
target_include_directories(Event PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/streamout>)
target_link_libraries(Event PUBLIC DIFStatic)
set_target_properties(Event PROPERTIES PUBLIC_HEADER "include/Event.h;include/EventLinkDef.h")
add_library(EventStatic STATIC $<TARGET_OBJECTS:Event>)
add_library(EventShared SHARED $<TARGET_OBJECTS:Event>)
add_library(Streamout::Interface::ROOT::Event ALIAS EventStatic)

add_library(ROOT src/ROOTWriter.cc)
target_compile_features(ROOT PUBLIC ${ROOT_CXX_STANDARD})
target_include_directories(ROOT PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/streamout>)
target_link_libraries(ROOT PUBLIC Streamout::Core ROOT::Tree EventStatic DIFStatic HitStatic)
set_target_properties(ROOT PROPERTIES PUBLIC_HEADER "include/ROOTWriter.h")
add_library(Streamout::Interface::ROOT ALIAS ROOT)
target_link_libraries(Interfaces INTERFACE Streamout::Interface::ROOT)

install(TARGETS ROOT EventStatic EventShared DIFStatic DIFShared HitStatic HitShared
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/streamout/interfaces/ROOTWriter"
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/streamout/interfaces/ROOTWriter")
