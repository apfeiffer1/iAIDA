
# iAIDA/examples

macro (setup exeName testString)
	add_executable( ${exeName} ${exeName}.cpp)
	TARGET_LINK_LIBRARIES( ${exeName} iAIDA ${EXTERNAL_LIBS})

	add_test ( ${exeName} ${exeName} )
	set_tests_properties ( ${exeName} PROPERTIES PASS_REGULAR_EXPRESSION ${testString} )
endmacro (setup)

# generic examples

set ( exaFiles 	convertTupleToCSV 
				showAidaFile
				exaH1         
				exaH2         
				exaH3         
				exaTree       
				exaTreeReader 
				exaTuple      
				exaTupleRead  
)

foreach (exa_ ${exaFiles} )
  setup( ${exa_} "That's it")
endforeach()

# additional setup:
set_tests_properties ( exaTreeReader PROPERTIES PASS_REGULAR_EXPRESSION "All OK" )

set_property(TEST exaTupleRead      APPEND PROPERTY DEPENDS exaTuple)
set_property(TEST exaTreeReader     APPEND PROPERTY DEPENDS exaTree)
set_property(TEST convertTupleToCSV APPEND PROPERTY DEPENDS exaTuple)
set_property(TEST showAidaFile      APPEND PROPERTY DEPENDS exaTree)

set( allExamples ${exaFiles} )

# tests with root
if (ROOT_FOUND)

  set( exaFilesRoot 	storeRoot
						readRoot
     )
  foreach(exa_ ${exaFilesRoot} )	
	setup( ${exa_} "That's it")
  endforeach()

  set_property(TEST readRoot APPEND PROPERTY DEPENDS storeRoot)
  set_property(TEST readRootTup APPEND PROPERTY DEPENDS storeRootTup)

  set( allExamples ${allExamples} ${exaFilesRoot} )

endif (ROOT_FOUND)

# tests for hbook
if (CERNLIB_FOUND)

  set( exaFilesHbk 	storeHbk
					readHbk
					storeHbkTup
					readHbkTup
					storeHbkCWN
					readHbkCWN
     )
  foreach(exa_ ${exaFilesHbk} )	
	setup( ${exa_} "That's it")
  endforeach()

  set_property(TEST readHbk APPEND PROPERTY DEPENDS storeHbk)
  set_property(TEST readHbkTup APPEND PROPERTY DEPENDS storeHbkTup)
  set_property(TEST readHbkCWN APPEND PROPERTY DEPENDS storeHbkCWN)

  set( allExamples ${allExamples} ${exaFilesHbk} )

endif (CERNLIB_FOUND)

IF(CMAKE_BUILD_TYPE MATCHES DEBUG) 
    message("... examples set up") 
ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG) 

if ( ${IAIDA_INSTALL_EXAMPLES} MATCHES ON)
  foreach(exa_ ${exaFiles} )	
    install(PROGRAMS ${PROJECT_BINARY_DIR}/examples/${exa_} DESTINATION ${CMAKE_INSTALL_LIBEXECDIR}/iAIDA )
  endforeach()
endif ( ${IAIDA_INSTALL_EXAMPLES} MATCHES ON)
