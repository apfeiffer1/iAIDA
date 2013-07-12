
## iAIDA/src 

include_directories( ${PROJECT_BINARY_DIR} ${extIncDirs}  )

# build the lib here
file(GLOB srcFilesAIDA "*.cpp")

if ( ROOT_FOUND )
    file	(GLOB srcFilesROOT "AIDA_ROOT/*.cpp")
	IF(CMAKE_BUILD_TYPE MATCHES DEBUG) 
		message ( "... added files for root ")
	ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG) 
endif ( ROOT_FOUND )

if ( GRACE_FOUND )
   file	(GLOB srcFilesPlot "AIDAGracePlotter/*.cpp")
	IF(CMAKE_BUILD_TYPE MATCHES DEBUG) 
		message ( "... added files for Grace plotter ")
	ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG) 
endif ( GRACE_FOUND )

if ( CERNLIB_FOUND )
   file	(GLOB srcFilesHbook "AIDA_HBook/*.cpp")
	IF(CMAKE_BUILD_TYPE MATCHES DEBUG) 
		message ( "... added files for HBook ")
	ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG) 
endif ( CERNLIB_FOUND )

set( srcFiles ${srcFilesAIDA} ${srcFilesROOT} ${srcFilesPlot} ${srcFilesHbook} )
add_library(iAIDA SHARED ${srcFiles})
target_link_libraries( iAIDA ${EXTERNAL_LIBS} ${EXTERNAL_LIBS} -ldl)

INSTALL(TARGETS iAIDA
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
