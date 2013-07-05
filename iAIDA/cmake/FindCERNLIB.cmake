# - Finds CERNLIB installation
# This module sets up CERNLIB information 
# It defines:
# CERNLIB_FOUND          If CERNLIB is found
# CERNLIB_INCLUDE_DIR    PATH to the include directory
# CERNLIB_LIBRARIES      CERNLIB libraries
# CERNLIB_LIBRARY_DIR    PATH to the library directory 


find_program(CERNLIB_CONFIG_EXECUTABLE cernlib)

if(NOT CERNLIB_CONFIG_EXECUTABLE)
  set(CERNLIB_FOUND FALSE)
else()    
  set(CERNLIB_FOUND TRUE)

  execute_process(
    COMMAND ${CERNLIB_CONFIG_EXECUTABLE} kernlib
    OUTPUT_VARIABLE CERNLIB_LIBRARIES
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  # Make variables changeable to the advanced user
  mark_as_advanced(CERNLIB_CONFIG_EXECUTABLE)

  if(NOT CERNLIB_FIND_QUIETLY)
    message(STATUS "Found CERNLIB ")
  endif()

  SET(optLibs ${optLibs} ${CERNLIB_LIBRARIES})

endif()

