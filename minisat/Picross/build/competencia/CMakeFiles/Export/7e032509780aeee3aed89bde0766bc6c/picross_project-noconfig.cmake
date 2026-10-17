#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "picross::stdutils" for configuration ""
set_property(TARGET picross::stdutils APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(picross::stdutils PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libstdutils.a"
  )

list(APPEND _cmake_import_check_targets picross::stdutils )
list(APPEND _cmake_import_check_files_for_picross::stdutils "${_IMPORT_PREFIX}/lib/libstdutils.a" )

# Import target "picross::picross" for configuration ""
set_property(TARGET picross::picross APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(picross::picross PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libpicross.a"
  )

list(APPEND _cmake_import_check_targets picross::picross )
list(APPEND _cmake_import_check_files_for_picross::picross "${_IMPORT_PREFIX}/lib/libpicross.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
