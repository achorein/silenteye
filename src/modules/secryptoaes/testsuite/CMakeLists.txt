PROJECT( testcryptoaes ) # the name of your project

cmake_minimum_required(VERSION 2.6.0)

SET( QT_DONT_USE_QTGUI TRUE )
SET( QT_USE_QTTEST TRUE )

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project

INCLUDE(${QT_USE_FILE})

SET(testcryptoaes_SRCS
  testcryptoaes.cpp
)

SET(testcryptoaes_MOC_HDRS
  testcryptoaes.h
)

INCLUDE_DIRECTORIES( testcryptoaes ${CMAKE_BINARY_DIR} )
include_directories( testcryptoaes 	${sef_SOURCE_DIR} )

QT4_WRAP_CPP(testcryptoaes_MOC_SRCS ${testcryptoaes_MOC_HDRS})

link_libraries( testcryptoaes  sef)
ADD_EXECUTABLE( testcryptoaes ${testcryptoaes_SRCS} ${testcryptoaes_MOC_SRCS} )

TARGET_LINK_LIBRARIES(testcryptoaes ${QT_LIBRARIES}) 

IF(WIN32)
	ADD_CUSTOM_COMMAND(TARGET testcryptoaes POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libsef.dll .
	)
ENDIF(WIN32)