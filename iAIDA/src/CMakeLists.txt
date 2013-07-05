
## iAIDA/src 

# build the lib here
file(GLOB srcFilesAIDA "*.cpp")

if ( ROOT_FOUND )
    file	(GLOB srcFilesROOT "AIDA_ROOT/*.cpp")
	message ( "... added files for root ")
endif ( ROOT_FOUND )

if ( Grace_FOUND )
   file	(GLOB srcFilesPlot "AIDA_GracePlotter/*.cpp")
	message ( "... added files for Grace plotter ")
endif ( Grace_FOUND )

set( srcFiles ${srcFilesAIDA} ${srcFilesROOT} ${srcFilesPlot} )
add_library(iAIDA SHARED ${srcFiles})
target_link_libraries( iAIDA ${EXTERNAL_LIBS})

INSTALL(TARGETS iAIDA
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
