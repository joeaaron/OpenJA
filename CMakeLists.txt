cmake_minimum_required(VERSION 2.8.11)
project(OpenJA)
include(CMakeDependentOption)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
# The version number.
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/include/config.h.in"
  "${PROJECT_SOURCE_DIR}/include/config.h"
  )
# include external library header files
include_directories("${PROJECT_SOURCE_DIR}/include")
# link external library directories
link_directories("${PROJECT_SOURCE_DIR}/lib")

# select library or target to build 
option(BUILD_CV			"Build Computer Vision" 	ON)
option (BUILD_EXAMPLES  "Build Test Demo" 	ON)
option (BUILD_CPLUSPLUS  "Build C PlusPlus" 	ON)
cmake_dependent_option(CV_CALIB_EXPORT	"build shared calib library"	OFF
					   "BUILD_CV" ON)	

cmake_dependent_option(CPLUSPLUS_FILE_EXPORT	"build shared file library"	OFF
					   "BUILD_CPLUSPLUS" ON)	
					   
if(BUILD_CV)
	add_subdirectory(modules/cv)
endif(BUILD_CV)

if(BUILD_EXAMPLES)
	add_subdirectory(samples)
endif(BUILD_EXAMPLES)

if(BUILD_CPLUSPLUS)
	add_subdirectory(modules/cplusplus)
endif(BUILD_CPLUSPLUS)

