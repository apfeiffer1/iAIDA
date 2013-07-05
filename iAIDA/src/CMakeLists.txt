
## iAIDA/src 

include_directories( ${PROJECT_BINARY_DIR} ${extIncDirs}  )

# build the lib here
file(GLOB srcFilesAIDA "*.cpp")

if ( ROOT_FOUND )
    file	(GLOB srcFilesROOT "AIDA_ROOT/*.cpp")
	message ( "... added files for root ")
endif ( ROOT_FOUND )

if ( GRACE_FOUND )
   file	(GLOB srcFilesPlot "AIDAGracePlotter/*.cpp")
	message ( "... added files for Grace plotter ")
endif ( GRACE_FOUND )

if ( CERNLIB_FOUND )
   file	(GLOB srcFilesHbook "AIDAHBook/*.cpp")
	message ( "... added files for HBook ")
endif ( CERNLIB_FOUND )

set( srcFiles ${srcFilesAIDA} ${srcFilesROOT} ${srcFilesPlot} ${srcFilesHbook} )
add_library(iAIDA SHARED ${srcFiles})
target_link_libraries( iAIDA ${EXTERNAL_LIBS})

INSTALL(TARGETS iAIDA
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
