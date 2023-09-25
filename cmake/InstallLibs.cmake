if (WIN32 AND NOT MSVC AND NOT NOT "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll")
  find_library(LIBGCC NAMES libgcc_s_seh-1)
  if (LIBGCC)
    message(STATUS "Found libgcc at ${LIBGCC}")
    configure_file("${LIBGCC}" "${CMAKE_CURRENT_BINARY_DIR}/libgcc_s_seh-1.dll" COPYONLY)
  else ()
    message(SEND_ERROR "Could not find libgcc!")
  endif ()

  find_library(LIBSTDCPP libstdc++-6)
  if (LIBSTDCPP)
    message(STATUS "Found libstdc++ at ${LIBSTDCPP}")
    configure_file("${LIBSTDCPP}" "${CMAKE_CURRENT_BINARY_DIR}/libstdc++-6.dll" COPYONLY)
  else ()
    message(SEND_ERROR "Could not find libstdc++!")
  endif ()

  find_library(LIBWINPTHREAD libwinpthread-1)
  if (LIBWINPTHREAD)
    message(STATUS "Found libwinpthread at ${LIBWINPTHREAD}")
    configure_file("${LIBWINPTHREAD}" "${CMAKE_CURRENT_BINARY_DIR}/libwinpthread-1.dll" COPYONLY)
  else ()
    message(SEND_ERROR "Could not find libwinpthread!")
  endif ()

  # install required libs
  function(install_libs PATH)
    install(FILES "${LIBGCC}" "${LIBSTDCPP}" "${LIBWINPTHREAD}" DESTINATION "${PATH}")
  endfunction()
  set(CMAKE_FIND_LIBRARY_SUFFIXES "")
elseif(WIN32 AND MSVC)
  set(CMAKE_INSTALL_DEBUG_LIBRARIES TRUE)
  set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
  set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION "bin/examples")
  include(InstallRequiredSystemLibraries)
  # install required libs
  function(install_libs PATH)
  endfunction()
else ()
  # install required libs
  function(install_libs PATH)
  endfunction()
endif()
