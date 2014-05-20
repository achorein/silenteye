PROJECT( testformatwav ) # the name of your project

cmake_minimum_required(VERSION 2.6.0)

SET( QT_DONT_USE_QTGUI TRUE )
SET( QT_USE_QTTEST TRUE )

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project

INCLUDE(${QT_USE_FILE})

SET(testformatwav_SRCS
  ../audiowav.cpp
  testformatwav.cpp
)

SET(testformatwav_MOC_HDRS
  testformatwav.h
)

INCLUDE_DIRECTORIES(testformatwav ${CMAKE_BINARY_DIR} )
INCLUDE_DIRECTORIES(testformatwav ${sef_SOURCE_DIR} )

QT4_WRAP_CPP(testformatwav_MOC_SRCS ${testformatwav_MOC_HDRS})

LINK_LIBRARIES( testformatwav sef)
ADD_EXECUTABLE( testformatwav ${testformatwav_SRCS} ${testformatwav_MOC_SRCS} )

TARGET_LINK_LIBRARIES(testformatwav ${QT_LIBRARIES}) 

IF(WIN32)
	ADD_CUSTOM_COMMAND(TARGET testformatwav POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libsef.dll .
	)
ENDIF(WIN32)
