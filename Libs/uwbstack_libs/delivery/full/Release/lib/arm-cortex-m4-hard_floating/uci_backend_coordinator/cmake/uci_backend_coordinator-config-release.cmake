#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "uci_backend_coordinator" for configuration "Release"
set_property(TARGET uci_backend_coordinator APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(uci_backend_coordinator PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/arm-cortex-m4-hard_floating/uci_backend_coordinator/libuci_backend_coordinator_full_arm-cortex-m4-hard_floating_rtos_R12.7.0-00288-gb8203d558863.a"
  )

list(APPEND _cmake_import_check_targets uci_backend_coordinator )
list(APPEND _cmake_import_check_files_for_uci_backend_coordinator "${_IMPORT_PREFIX}/lib/arm-cortex-m4-hard_floating/uci_backend_coordinator/libuci_backend_coordinator_full_arm-cortex-m4-hard_floating_rtos_R12.7.0-00288-gb8203d558863.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
