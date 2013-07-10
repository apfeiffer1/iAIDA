
cmake_minimum_required (VERSION 2.6)

project (iAIDA CXX)

foreach(_pkg ROOT)
  configure_file(${PROJECT_SOURCE_DIR}/cmake/Find${_pkg}.cmake ${PROJECT_BINARY_DIR}/Modules/Find${_pkg}.cmake COPYONLY)
  include(${PROJECT_BINARY_DIR}/Modules/Find${_pkg}.cmake)
endforeach()

# iAIDA top level CMakeLists.txt 

# enable and set up tests and examples
ENABLE_TESTING()

# set up the required packages
find_package ( Boost 1.46 COMPONENTS serialization iostreams REQUIRED )
find_package ( EXPAT REQUIRED )
find_package ( ZLIB  REQUIRED )

# set up external includes and libs
set(extIncDirs ${Boost_INCLUDE_DIR} "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/src" )
set(extLibs    ${Boost_LIBRARIES} ${EXPAT_LIBRARIES} ${ZLIB_LIBRARIES} )

# we need the AIDA package
configure_file(${PROJECT_SOURCE_DIR}/cmake/FindAIDA.cmake ${PROJECT_BINARY_DIR}/Modules/FindAIDA.cmake COPYONLY)
include(${PROJECT_BINARY_DIR}/Modules/FindAIDA.cmake)

# ... copy the local AIDA includes if no AIDA is found
if ( NOT AIDA_FOUND )
  file(GLOB aidaHeaders "${PROJECT_SOURCE_DIR}/ext/AIDA/*.h")
  file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/AIDA/)
  foreach(_hdr ${aidaHeaders} )
  	get_filename_component( Basename ${_hdr} NAME_WE )
     	configure_file(${_hdr} ${PROJECT_BINARY_DIR}/AIDA/${Basename}.h COPYONLY)
  endforeach()
  message("... AIDA installation not found ... copied headers locally")
else() # add include dirs from central installation:
   include_directories( ${AIDA_INCLUDE_DIR}  )
endif ( NOT AIDA_FOUND )


# and check for optional ones. These add their own params to the overall include dir and the optLibs variables:
foreach(_pkg ROOT Grace)
  configure_file(${PROJECT_SOURCE_DIR}/cmake/Find${_pkg}.cmake ${PROJECT_BINARY_DIR}/Modules/Find${_pkg}.cmake COPYONLY)
  include(${PROJECT_BINARY_DIR}/Modules/Find${_pkg}.cmake)
endforeach()

# set up aida-config
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/bin/)
configure_file(${PROJECT_SOURCE_DIR}/aida-config.in ${PROJECT_BINARY_DIR}/bin/aida-config)

# set up common include directories and libs from externals and optional packages
include_directories( ${PROJECT_BINARY_DIR} ${extIncDirs}  )
set(EXTERNAL_LIBS ${extLibs} ${optLibs} )

# add the subdirs where we need to run make:
foreach(_dir src tests examples)
    add_subdirectory( ${_dir} )
endforeach()

# installation
install(PROGRAMS ${PROJECT_BINARY_DIR}/bin/aida-config DESTINATION bin)
install(FILES    ${aidaHeaders} DESTINATION include/AIDA)
