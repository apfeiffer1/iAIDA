
# iAIDA/tests

macro (setup exeName testString)
	add_executable( ${exeName} ${exeName}.cpp)
    TARGET_LINK_LIBRARIES( ${exeName} iAIDA ${EXTERNAL_LIBS} )
	add_test ( ${exeName} ${exeName} )
	set_tests_properties ( ${exeName} PROPERTIES PASS_REGULAR_EXPRESSION ${testString} )
endmacro (setup)

# generic tests
setup( "testAIDAMem" "OK")
setup( "testHistoName" "OK")

if (GRACE_FOUND)
  setup ( "plot" "OK")
  setup ( "plottingHistograms" "OK")
endif (GRACE_FOUND)

message ("... examples set up")
