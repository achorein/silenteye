PROJECT( testconfig ) # the name of your project

cmake_minimum_required(VERSION 2.6.0)

SET( QT_DONT_USE_QTGUI TRUE )
SET( QT_USE_QTTEST TRUE )

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project

INCLUDE(${QT_USE_FILE})

SET(testconfig_SRCS
  testconfig.cpp
)

SET(testconfig_MOC_HDRS
  testconfig.h
)

INCLUDE_DIRECTORIES( testconfig ${CMAKE_BINARY_DIR} )
include_directories( testconfig ${sef_SOURCE_DIR} )

QT4_WRAP_CPP(testconfig_MOC_SRCS ${testconfig_MOC_HDRS})

link_libraries( testconfig  sef)
ADD_EXECUTABLE( testconfig ${testconfig_SRCS} ${testconfig_MOC_SRCS} )

TARGET_LINK_LIBRARIES(testconfig ${QT_LIBRARIES}) 

IF(WIN32)
	ADD_CUSTOM_COMMAND(TARGET testconfig POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libsef.dll .
	)
ENDIF(WIN32)