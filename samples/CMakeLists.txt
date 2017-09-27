#######################################################
# Copyright:   ShanghaiCowaRobot Tech. Co., Ltd.
# Author:      Vision Team
# Version:     1.0
# Data:        2017.09.21
# url:         
########################################################
cmake_minimum_required (VERSION 2.6) 

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/SeqCapture")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/SeqCapture")

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})

AUX_SOURCE_DIRECTORY("${CMAKE_CURRENT_SOURCE_DIR}/SeqCapture" SeqCaptureSRC)

file(GLOB_RECURSE CURRENT_HEADERS *.h *.hpp)
source_group("Include" FILES ${CURRENT_HEADERS})

#Test Calib
add_executable(TestCalib testCalib.cpp )
target_link_libraries(TestCalib 
					 Calib                     #Use the lib
					 ${EXTRA_LIBS}
					)
set_target_properties(TestCalib 
				    PROPERTIES 
					ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/lib"
					LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/lib"
					RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/debug"
					ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/lib"
					LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/lib"
					RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/release"
)

#Use the Opencv module
target_link_libraries(TestCalib ${OpenCV_LIBS})
set_target_properties(TestCalib
					PROPERTIES
					ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
					LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
					RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

#Test File
add_executable(TestFile testFile.cpp )
target_link_libraries(TestFile 
					 File                     #Use the lib
					 ${EXTRA_LIBS}
					)
set_target_properties(TestFile 
				    PROPERTIES 
					ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/lib"
					LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/lib"
					RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/debug"
					ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/lib"
					LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/lib"
					RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/release"
)
