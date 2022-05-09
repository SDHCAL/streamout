add_subdirectory(standard)

if(BUILD_ROOT_INTERFACE)
  add_subdirectory(root)
endif()

if(BUILD_LCIO_INTERFACE AND NOT MSVC)
  add_subdirectory(LCIO)
endif()
