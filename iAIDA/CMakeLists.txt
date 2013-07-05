
cmake_minimum_required (VERSION 2.6)

project (iAIDA CXX)

foreach(_pkg ROOT)
  configure_file(${PROJECT_SOURCE_DIR}/cmake/Find${_pkg}.cmake ${PROJECT_BINARY_DIR}/Modules/Find${_pkg}.cmake COPYONLY)
  include(${PROJECT_BINARY_DIR}/Modules/Find${_pkg}.cmake)
endforeach()

# enable and set up tests and examples
ENABLE_TESTING()

# set up the required packages
find_package ( Boost 1.46 COMPONENTS serialization iostreams REQUIRED )
find_package ( EXPAT REQUIRED )
find_package ( ZLIB  REQUIRED )

# set up external includes and libs
set(extIncDirs ${Boost_INCLUDE_DIR} "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/src" )
set(extLibs    ${Boost_LIBRARIES} ${EXPAT_LIBRARIES} ${ZLIB_LIBRARIES} )

# copy AIDA includes if no AIDA_DIR is specified
file(GLOB aidaHeaders "${PROJECT_SOURCE_DIR}/ext/AIDA/*.h")
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/AIDA/)
foreach(_hdr ${aidaHeaders} )
	get_filename_component( Basename ${_hdr} NAME_WE )
   configure_file(${_hdr} ${PROJECT_BINARY_DIR}/AIDA/${Basename} COPYONLY)
endforeach()

# and check for optional ones :
foreach(_pkg ROOT Grace)
  configure_file(${PROJECT_SOURCE_DIR}/cmake/Find${_pkg}.cmake ${PROJECT_BINARY_DIR}/Modules/Find${_pkg}.cmake COPYONLY)
  include(${PROJECT_BINARY_DIR}/Modules/Find${_pkg}.cmake)
endforeach()

# set up the include and libs for the optional packages
set(optIncDirs ${ROOT_INCLUDE_DIR} ${Grace_INCLUDE_DIR} )
set(optLibs    ${ROOT_LIBRARIES} ${Grace_LIBRARIES} )

# set up aida-config
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/bin/)
configure_file(${PROJECT_SOURCE_DIR}/aida-config.in ${PROJECT_BINARY_DIR}/bin/aida-config)

# set up common include directories and libs from externals and optional packages
include_directories( ${PROJECT_BINARY_DIR} ${extIncDirs} ${optIncDirs} )
set(EXTERNAL_LIBS ${extLibs} ${optLibs} )

# add the subdirs where we need to run make:
foreach(_dir src tests examples)
    add_subdirectory( ${_dir} )
endforeach()

# installation
install(PROGRAMS ${PROJECT_BINARY_DIR}/bin/aida-config DESTINATION bin)
install(FILES    ${aidaHeaders} DESTINATION include/AIDA)
