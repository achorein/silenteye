PROJECT( testcryptoaes256 ) # the name of your project

cmake_minimum_required(VERSION 2.6.0)

SET( QT_DONT_USE_QTGUI TRUE )
SET( QT_USE_QTTEST TRUE )

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project

INCLUDE(${QT_USE_FILE})

SET(testcryptoaes256_SRCS
  testcryptoaes256.cpp
)

SET(testcryptoaes256_MOC_HDRS
  testcryptoaes256.h
)

INCLUDE_DIRECTORIES( testcryptoaes256 ${CMAKE_BINARY_DIR} )
include_directories( testcryptoaes256 	${sef_SOURCE_DIR} )

QT4_WRAP_CPP(testcryptoaes256_MOC_SRCS ${testcryptoaes256_MOC_HDRS})

link_libraries( testcryptoaes256 sef)
ADD_EXECUTABLE( testcryptoaes256 ${testcryptoaes256_SRCS} ${testcryptoaes256_MOC_SRCS} )

TARGET_LINK_LIBRARIES(testcryptoaes256 ${QT_LIBRARIES}) 

IF(WIN32)
	ADD_CUSTOM_COMMAND(TARGET testcryptoaes256 POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libsef.dll .
	)
ENDIF(WIN32)