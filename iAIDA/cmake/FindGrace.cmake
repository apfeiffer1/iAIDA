
# from https://github.com/dxli/Voronoi/blob/master/FindGrace.cmake - thanks ! :)

FIND_PATH(GRACE_INCLUDE_DIRS grace_np.h /usr/include/ /usr/local/include /usr/local/grace/include)

find_library(GRACE_LIBRARIES libgrace_np grace_np PATH /usr/lib /usr/local/lib /usr/local/grace/lib)
# FIND_PATH(_GRACE_LIB_PATH libgrace_np.a PATH /usr/lib /usr/local/lib /usr/local/grace/lib)
# set(GRACE_LIBRARIES ${_GRACE_LIB_PATH}/libgrace_np.a)

IF (GRACE_INCLUDE_DIRS AND GRACE_LIBRARIES)
   	SET(GRACE_FOUND TRUE)
	add_definitions(-DHAVE_GRACE=TRUE)
	
	INCLUDE_DIRECTORIES( ${GRACE_INCLUDE_DIRS} )
	SET(optLibs ${optLibs} ${GRACE_LIBRARIES})
ENDIF (GRACE_INCLUDE_DIRS AND GRACE_LIBRARIES)


IF (GRACE_FOUND)
   IF (NOT Grace_FIND_QUIETLY)
      MESSAGE(STATUS "Found Grace: ${GRACE_LIBRARIES}")
   ENDIF (NOT Grace_FIND_QUIETLY)
ELSE (GRACE_FOUND)
   IF (Grace_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find Grace")
   ENDIF (Grace_FIND_REQUIRED)
ENDIF (GRACE_FOUND)
