# standard
add_library(standard src/textDump.cc)
target_include_directories(standard PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(standard PUBLIC core)
install(TARGETS standard RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
