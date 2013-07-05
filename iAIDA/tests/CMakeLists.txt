
macro (setup exeName testString)
	add_executable( ${exeName} ${exeName}.cpp)
	TARGET_LINK_LIBRARIES( ${exeName} iAIDA ${EXTERNAL_LIBS})

	add_test ( ${exeName} ${exeName} )
	set_tests_properties ( ${exeName} PROPERTIES PASS_REGULAR_EXPRESSION ${testString} )
endmacro (setup)

file(GLOB testFiles *.cpp)
foreach(_test ${testFiles})
	get_filename_component( Basename ${_test} NAME_WE )
	setup(${Basename} "OK")
	# message ("set up test for " ${Basename})
endforeach()

message ("... examples set up")
