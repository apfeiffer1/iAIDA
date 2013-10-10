
# iAIDA/tests

macro (setup exeName testString)
	add_executable( ${exeName} ${exeName}.cpp)
    TARGET_LINK_LIBRARIES( ${exeName} iAIDA ${EXTERNAL_LIBS} )
	add_test ( ${exeName} ${exeName} )
	set_tests_properties ( ${exeName} PROPERTIES PASS_REGULAR_EXPRESSION ${testString} )
endmacro (setup)

# generic tests
set( tstFiles 	testAIDAMemoryTree
				testHistoName
				writeHistos
				writeTuple
				writeClouds
   )
foreach(exa_ ${tstFiles} )	
	setup( ${exa_} "OK")
endforeach()

set( allTests ${tstFiles} )

if (GRACE_FOUND)
  setup ( "plot" "OK")
  setup ( "plottingHistograms" "OK")

  set( allTests ${allTests} plot plottingHistogram )
endif (GRACE_FOUND)

IF(CMAKE_BUILD_TYPE MATCHES DEBUG) 
    message("... tests set up") 
ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG) 

if ( ${IAIDA_INSTALL_EXAMPLES} MATCHES ON)
  foreach(tst_ ${tstFiles} )	
    install(PROGRAMS ${PROJECT_BINARY_DIR}/tests/${tst_} DESTINATION ${CMAKE_INSTALL_LIBEXECDIR}/iAIDA )
  endforeach()
endif ( ${IAIDA_INSTALL_EXAMPLES} MATCHES ON)
