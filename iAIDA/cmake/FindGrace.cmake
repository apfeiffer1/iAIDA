
# from https://github.com/dxli/Voronoi/blob/master/FindGrace.cmake - thanks ! :)

FIND_PATH(GRACE_INCLUDE_DIR grace_np.h /usr/include/ /usr/local/include)

FIND_LIBRARY(GRACE_LIBRARY NAMES grace_np PATH /usr/lib /usr/local/lib) 

IF (GRACE_INCLUDE_DIR AND GRACE_LIBRARY)
   SET(GRACE_FOUND TRUE)
INCLUDE_DIRECTORIES( ${GRACE_INCLUDE_DIR} )
SET(optionalLibs ${optionalLibs} ${GRACE_LIBRARY})
ENDIF (GRACE_INCLUDE_DIR AND GRACE_LIBRARY)


IF (GRACE_FOUND)
   IF (NOT Grace_FIND_QUIETLY)
      MESSAGE(STATUS "Found Grace: ${GRACE_LIBRARY}")
   ENDIF (NOT Grace_FIND_QUIETLY)
ELSE (GRACE_FOUND)
   IF (Grace_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find Grace")
   ENDIF (Grace_FIND_REQUIRED)
ENDIF (GRACE_FOUND)