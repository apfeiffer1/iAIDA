
# iAIDA/examples

macro (setup exeName testString)
	add_executable( ${exeName} ${exeName}.cpp)
	TARGET_LINK_LIBRARIES( ${exeName} iAIDA ${EXTERNAL_LIBS})

	add_test ( ${exeName} ${exeName} )
	set_tests_properties ( ${exeName} PROPERTIES PASS_REGULAR_EXPRESSION ${testString} )
endmacro (setup)

file(GLOB testFiles *.cpp)
foreach(_test ${testFiles})
	get_filename_component( Basename ${_test} NAME_WE )
	setup(${Basename} "That's it")
	# message ("set up test for " ${Basename})
endforeach()

message ("... examples set up")

set_property(TEST exaTupleRead  APPEND PROPERTY DEPENDS exaTuple)
set_property(TEST exaTreeReader APPEND PROPERTY DEPENDS exaTree)
set_property(TEST convertTupleToCSV APPEND PROPERTY DEPENDS exaTuple)

set_property(TEST readRoot APPEND PROPERTY DEPENDS storeRoot)

foreach(_what Hbk HbkCWN HbkTup Root RootTup )
	set_property(TEST read${_what} APPEND PROPERTY DEPENDS store${_what})
endforeach()

