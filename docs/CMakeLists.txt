include(Documentations)
set(DOXYGEN_RECURSIVE YES)
set(DOXYGEN_JAVADOC_BLOCK YES)
set(DOXYGEN_JAVADOC_AUTOBRIEF YES)
set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_SOURCE_BROWSER YES)
set(DOXYGEN_INLINE_SOURCES YES)
set(DOXYGEN_SEARCHENGINE YES)
set(DOXYGEN_SERVER_BASED_SEARCH NO)
set(DOXYGEN_DISABLE_INDEX NO)
set(DOXYGEN_GENERATE_TREEVIEW YES)
set(DOXYGEN_INTERACTIVE_SVG YES)
set(DOXYGEN_DOR_IMAGE_FORMAT svg)
set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
set(DOXYGEN_TAB_SIZE 2)
set(DOXYGEN_SOURCE_TOOLTIPS YES)
set(DOXYGEN_HTML_DYNAMIC_MENUS YES)
set(DOXYGEN_CLASS_DIAGRAMS YES)
set(DOXYGEN_UML_LOOK YES)
set(DOXYGEN_CALL_GRAPH YES)
set(DOXYGEN_CALLER_GRAPH YES)
set(DOXYGEN_FULL_PATH_NAMES YES)
set(DOXYGEN_INLINE_SOURCES YES)
set(DOXYGEN_ENABLE_PREPROCESSING YES)
set(DOXYGEN_MACRO_EXPANSION YES)
set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
## HTML OPTIONS ##
set(DOXYGEN_GENERATE_HTML YES)
## LATEX OPTIONS ##
set(DOXYGEN_GENERATE_LATEX YES)
set(DOXYGEN_COMPACT_LATEX YES)
set(DOXYGEN_LATEX_BATCHMODE NO)
set(DOXYGEN_IMAGE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/imgs")
set(DOXYGEN_STRIP_FROM_PATH "/home/runner/work/streamout/streamout;${PROJECT_SOURCE_DIR}")
set(DOXYGEN_STRIP_FROM_INC_PATH "/home/runner/work/streamout/streamout;${PROJECT_SOURCE_DIR}")
doxyfile_docs("${PROJECT_SOURCE_DIR}/libs/" "${PROJECT_SOURCE_DIR}/apps/" "${PROJECT_BINARY_DIR}/tests/")
