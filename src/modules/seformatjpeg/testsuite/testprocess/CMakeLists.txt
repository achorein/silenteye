PROJECT( testprocess ) # the name of your project

cmake_minimum_required(VERSION 2.6.0)

SET( QT_DONT_USE_QTGUI TRUE )
SET( QT_USE_QTTEST TRUE )

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project

INCLUDE(${QT_USE_FILE})

SET(testprocess_SRCS
  ../../stegotable.cpp
  ../../ycbcr.cpp
  ../../groupedimage.cpp
  testprocess.cpp
)

SET(testprocess_MOC_HDRS
  testprocess.h
)

INCLUDE_DIRECTORIES(testprocess ${CMAKE_BINARY_DIR} )
INCLUDE_DIRECTORIES(testprocess ${sef_SOURCE_DIR} )

QT4_WRAP_CPP(testprocess_MOC_SRCS ${testprocess_MOC_HDRS})

LINK_LIBRARIES( testprocess sef)
ADD_EXECUTABLE( testprocess ${testprocess_SRCS} ${testprocess_MOC_SRCS} )

TARGET_LINK_LIBRARIES(testprocess ${QT_LIBRARIES})

IF(WIN32)
	ADD_CUSTOM_COMMAND(TARGET testprocess POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libsef.dll .
	)
ENDIF(WIN32)
