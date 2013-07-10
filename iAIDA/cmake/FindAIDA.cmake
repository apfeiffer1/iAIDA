
# - Finds AIDA installation

# This module sets up AIDA information 
# It defines:
# AIDA_FOUND          If AIDA is found
# AIDA_INCLUDE_DIR    PATH to the include directory
# AIDA is a header only project, so not libs and the like is defined

set ( AIDA_CHECK_PATH $ENV{AIDA_DIR} )

# check if the user provides an explicit installation, if so, check that it is valid
if (EXISTS ${AIDA_CHECK_PATH} )
   if (EXISTS ${AIDA_CHECK_PATH}/AIDA/AIDA.h)  # check if there really is an AIDA/AIDA.h in the path:
      set(AIDA_PATH ${AIDA_CHECK_PATH})
   endif (EXISTS ${AIDA_CHECK_PATH}/AIDA/AIDA.h)
endif (EXISTS ${AIDA_CHECK_PATH})

# check if we got a valid path
if (${AIDA_PATH})

else()

   find_path(AIDA_PATH AIDA/AIDA.h PATHS $ENV{AIDA_DIR})
   
   if (AIDA_PATH)
     set(AIDA_FOUND TRUE)
     
     unset (AIDA_LIBRARIES)  # no libs, include only project
     set( AIDA_INCLUDE_DIR ${AIDA_PATH})
   
     if(NOT AIDA_FIND_QUIETLY)
       message(STATUS "Found AIDA - installed in: " ${AIDA_INCLUDE_DIR})
     endif()
   
   else()    
     set(AIDA_FOUND FALSE)
   endif()

endif (${AIDA_PATH})
