PROJECT( testformatjpeg ) # the name of your project

cmake_minimum_required(VERSION 2.6.0)

SET( QT_DONT_USE_QTGUI TRUE )
SET( QT_USE_QTTEST TRUE )

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project

INCLUDE(${QT_USE_FILE})

SET(testformatjpeg_SRCS
  ../../imagejpeg.cpp
  ../../ycbcr.cpp
  ../../groupedimage.cpp
  ../../stegotable.cpp
  testformatjpeg.cpp
)

SET(testformatjpeg_MOC_HDRS
  testformatjpeg.h
)

INCLUDE_DIRECTORIES(testformatjpeg ${CMAKE_BINARY_DIR} )
INCLUDE_DIRECTORIES(testformatjpeg ${sef_SOURCE_DIR} )


QT4_WRAP_CPP(testformatjpeg_MOC_SRCS ${testformatjpeg_MOC_HDRS})

LINK_LIBRARIES( testformatjpeg sef)
ADD_EXECUTABLE( testformatjpeg ${testformatjpeg_SRCS} ${testformatjpeg_MOC_SRCS} )

TARGET_LINK_LIBRARIES(testformatjpeg ${QT_LIBRARIES})

IF(WIN32)
	ADD_CUSTOM_COMMAND(TARGET testformatjpeg POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libsef.dll .
	)
ENDIF(WIN32)
