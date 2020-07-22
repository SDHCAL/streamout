if(BUILD_DOCUMENTATION)
  find_package(Doxygen)
  if(NOT DOXYGEN_FOUND)
    message(STATUS "Doxygen is needed to build the documentation.")
    return()
  endif()

  set( doxyfile_in          ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
  SET( doxyfile             ${PROJECT_BINARY_DIR}/Doxyfile)
  SET( doxy_html_index_file ${CMAKE_CURRENT_BINARY_DIR}/html/index.html)
  SET( doxy_output_root     ${CMAKE_CURRENT_BINARY_DIR}) # Pasted into Doxyfile.in
  SET( doxy_input           ${PROJECT_SOURCE_DIR}/source) # Pasted into Doxyfile.in
 # SET( doxy_extra_files     ${CMAKE_CURRENT_SOURCE_DIR}/mainpage.dox) # Pasted into Doxyfile.in

  configure_file( ${doxyfile_in} ${doxyfile} @ONLY )

  add_custom_command(
    OUTPUT ${doxy_html_index_file}
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    # The following should be ${doxyfile} only but it
    # will break the dependency.
    # The optimal solution would be creating a
    # custom_command for ${doxyfile} generation
    # but I still have to figure out how...
    MAIN_DEPENDENCY ${doxyfile} ${doxyfile_in}
    DEPENDS core
    COMMENT "Generating HTML documentation"
  )

  add_custom_target( doc ALL DEPENDS ${doxy_html_index_file} )

  install( DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html" DESTINATION "${CMAKE_INSTALL_PREFIX}/docs" )
endif()
