
# iAIDA/examples

macro (setup exeName testString)
	add_executable( ${exeName} ${exeName}.cpp)
	TARGET_LINK_LIBRARIES( ${exeName} iAIDA ${EXTERNAL_LIBS})

	add_test ( ${exeName} ${exeName} )
	set_tests_properties ( ${exeName} PROPERTIES PASS_REGULAR_EXPRESSION ${testString} )
endmacro (setup)

# generic examples
file(GLOB testFiles exa*.cpp)
foreach(_test ${testFiles})
	get_filename_component( Basename ${_test} NAME_WE )
	setup(${Basename} "That's it")
endforeach()
setup( convertTupleToCSV "That's it")

set_property(TEST exaTupleRead  APPEND PROPERTY DEPENDS exaTuple)
set_property(TEST exaTreeReader APPEND PROPERTY DEPENDS exaTree)
set_property(TEST convertTupleToCSV APPEND PROPERTY DEPENDS exaTuple)

# tests with root
if (ROOT_FOUND)

  setup( storeRoot "That's it")
  setup( readRoot "That's it")
  set_property(TEST readRoot APPEND PROPERTY DEPENDS storeRoot)

  setup( storeRootTup "That's it")
  setup( readRootTup "That's it")
  set_property(TEST readRootTup APPEND PROPERTY DEPENDS storeRootTup)

endif (ROOT_FOUND)

# tests for hbook
if (CERNLIB_FOUND)

  setup( storeHbk "That's it")
  setup( readHbk "That's it")
  set_property(TEST readHbk APPEND PROPERTY DEPENDS storeHbk)

  setup( storeHbkTup "That's it")
  setup( readHbkTup "That's it")
  set_property(TEST readHbkTup APPEND PROPERTY DEPENDS storeHbkTup)

  setup( storeHbkCWN "That's it")
  setup( readHbkCWN "That's it")
  set_property(TEST readHbkCWN APPEND PROPERTY DEPENDS storeHbkCWN)

endif (CERNLIB_FOUND)

message ("... examples set up")
