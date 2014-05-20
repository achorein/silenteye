PROJECT( testformatbmp ) # the name of your project

cmake_minimum_required(VERSION 2.6.0)

SET( QT_DONT_USE_QTGUI TRUE )
SET( QT_USE_QTTEST TRUE )

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project

INCLUDE(${QT_USE_FILE})

SET(testformatbmp_SRCS
  ../imagebmp.cpp
  testformatbmp.cpp
)

SET(testformatbmp_MOC_HDRS
  testformatbmp.h
)

INCLUDE_DIRECTORIES(testformatbmp ${CMAKE_BINARY_DIR} )
INCLUDE_DIRECTORIES(testformatbmp ${sef_SOURCE_DIR} )

QT4_WRAP_CPP(testformatbmp_MOC_SRCS ${testformatbmp_MOC_HDRS})

LINK_LIBRARIES( testformatbmp sef)
ADD_EXECUTABLE( testformatbmp ${testformatbmp_SRCS} ${testformatbmp_MOC_SRCS} )

TARGET_LINK_LIBRARIES(testformatbmp ${QT_LIBRARIES}) 

IF(WIN32)
	ADD_CUSTOM_COMMAND(TARGET testformatbmp POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libsef.dll .
	)
ENDIF(WIN32)