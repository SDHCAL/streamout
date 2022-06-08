add_library(Interfaces INTERFACE)
add_library(Streamout::Interfaces ALIAS Interfaces)

add_subdirectory(Dump)

if(BUILD_RAWDATA_INTERFACE)
  add_subdirectory(RawDataReader)
endif()

if(BUILD_ROOT_INTERFACE)
  add_subdirectory(ROOT)
endif()

if(BUILD_LCIO_INTERFACE AND NOT MSVC)
  add_subdirectory(LCIO)
endif()
