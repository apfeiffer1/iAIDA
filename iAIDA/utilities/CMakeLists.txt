
# iAIDA/utilities

macro (setup exeName testString)
	add_executable( ${exeName} ${exeName}.cpp)
	TARGET_LINK_LIBRARIES( ${exeName} iAIDA ${EXTERNAL_LIBS})

	add_test ( ${exeName} ${exeName} )
	set_tests_properties ( ${exeName} PROPERTIES PASS_REGULAR_EXPRESSION ${testString} )
endmacro (setup)

# generic utilities

set ( utilFiles 	aida_convertTupleToCSV 
				aida_convertToXML
				aida_showFile
)

foreach (util_ ${utilFiles} )
  setup( ${util_} "That's it")
endforeach()


set( allutilities ${utilFiles} )

IF(CMAKE_BUILD_TYPE MATCHES DEBUG) 
    message("... utilities set up") 
ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG) 

if ( ${IAIDA_INSTALL_utilities} MATCHES ON)
  foreach(util_ ${utilFiles} )	
    install(PROGRAMS ${PROJECT_BINARY_DIR}/utilities/${util_} DESTINATION ${CMAKE_INSTALL_LIBEXECDIR}/iAIDA )
  endforeach()
endif ( ${IAIDA_INSTALL_utilities} MATCHES ON)
