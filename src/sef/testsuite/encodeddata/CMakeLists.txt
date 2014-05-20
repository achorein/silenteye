PROJECT( testencodeddata ) # the name of your project

cmake_minimum_required(VERSION 2.6.0)

SET( QT_DONT_USE_QTGUI TRUE )
SET( QT_USE_QTTEST TRUE )

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project

INCLUDE(${QT_USE_FILE})

SET(testencodeddata_SRCS
  testencodeddata.cpp
)

SET(testencodeddata_MOC_HDRS
  testencodeddata.h
)

INCLUDE_DIRECTORIES( testencodeddata ${CMAKE_BINARY_DIR} )
include_directories( testencodeddata 	${sef_SOURCE_DIR} )

QT4_WRAP_CPP(testencodeddata_MOC_SRCS ${testencodeddata_MOC_HDRS})

link_libraries( testencodeddata  sef)
ADD_EXECUTABLE( testencodeddata ${testencodeddata_SRCS} ${testencodeddata_MOC_SRCS} )

TARGET_LINK_LIBRARIES(testencodeddata ${QT_LIBRARIES}) 

IF(WIN32)
	ADD_CUSTOM_COMMAND(TARGET testencodeddata POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libsef.dll .
	)
ENDIF(WIN32)