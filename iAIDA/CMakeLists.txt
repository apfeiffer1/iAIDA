
# top level CMakeLists.txt file for iAIDA
# 
# to be used in a non-source build area like this:
#
# mkdir build && cd build
# cmake  ..
# 
# possible other options for the cmake command:
# 		-DCMAKE_INSTALL_PREFIX=/path/to/install  	# to install in a specific location
#       -DCMAKE_BUILD_TYPE=DEBUG      				# to switch on some debug messages
#       -DIAIDA_INSTALL_EXAMPLES=ON   				# to install the binaries of the examples/tests into <installPrefix>/libexec/iAIDA/
#       -DIAIDA_DO_NOT_USE_ROOT=ON					# do not use optional ROOT  package, even if found on system
#       -DIAIDA_DO_NOT_USE_Grace=ON					# do not use optional Grace package, even if found on system
#
# Copyright (C) Andreas Pfeiffer (apfeiffer1@gmail.com) 2013

cmake_minimum_required (VERSION 2.6)

project (iAIDA CXX)

include(GNUInstallDirs)

# iAIDA top level CMakeLists.txt 

# enable and set up tests and examples
ENABLE_TESTING()

# set up the required packages
find_package ( Boost 1.41 COMPONENTS serialization iostreams filesystem system random REQUIRED )
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
  message("... AIDA installation not found ... using local headers")

  # add the header files to the installation, if AIDA is not already installed:
  install(FILES    ${aidaHeaders} DESTINATION include/AIDA)

else() # add include dirs from central installation:
   include_directories( ${AIDA_INCLUDE_DIR}  )
endif ( NOT AIDA_FOUND )


# and check for optional ones. These add their own params to the overall include dir and the optLibs variables:
# but first check if the user has forbidden the use of any of these (e.g. for licensing reasons)
foreach(_pkg ROOT Grace)
  if ( NOT IAIDA_DO_NOT_USE_${_pkg} )
    configure_file(${PROJECT_SOURCE_DIR}/cmake/Find${_pkg}.cmake ${PROJECT_BINARY_DIR}/Modules/Find${_pkg}.cmake COPYONLY)
    include(${PROJECT_BINARY_DIR}/Modules/Find${_pkg}.cmake)
  else ()
    message( "... user request to ignore use of optional package "+${_pkg} )
	if ( CMAKE_BUILD_TYPE MATCHES DEBUG ) 
	   message( "... user request to ignore use of optional package "+${_pkg} )
	endif ( CMAKE_BUILD_TYPE MATCHES DEBUG ) 
  endif ( NOT IAIDA_DO_NOT_USE_${_pkg} )
endforeach()  # check optional packages


# set up aida-config
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/bin/)
configure_file(${PROJECT_SOURCE_DIR}/aida-config.in ${PROJECT_BINARY_DIR}/bin/aida-config)

# set up common include directories and libs from externals and optional packages
include_directories( ${PROJECT_BINARY_DIR} ${extIncDirs}  )
set(EXTERNAL_LIBS ${extLibs} ${optLibs} )

IF(CMAKE_BUILD_TYPE MATCHES DEBUG) 
    message("... build type is DEBUG") 
ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG) 

# add the subdirs where we need to run make:
foreach(_dir src tests examples utilities)
    add_subdirectory( ${_dir} )
endforeach()

# installation
install(PROGRAMS ${PROJECT_BINARY_DIR}/bin/aida-config DESTINATION bin)
install(DIRECTORY include src examples tests DESTINATION ${CMAKE_INSTALL_DOCDIR} )
