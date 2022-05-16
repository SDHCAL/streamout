add_library(Interfaces INTERFACE)
add_library(Streamout::Interfaces ALIAS Interfaces)

add_subdirectory(Dump)
add_subdirectory(RawDataReader)

if(BUILD_ROOT_INTERFACE)
  add_subdirectory(ROOT)
endif()

if(BUILD_LCIO_INTERFACE AND NOT MSVC)
  add_subdirectory(LCIO)
endif()
