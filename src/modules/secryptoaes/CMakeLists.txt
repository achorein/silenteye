PROJECT( secryptoaes )
cmake_minimum_required(VERSION 2.6.0)

find_package(Qt4 REQUIRED)
find_package(QCA2) # find QtCrypto
IF(NOT QCA2_FOUND)
  IF(WIN32)
    MESSAGE("** QCA2: using manual configuration for win32")
    set(QCA2_LIBRARIES "${QT_LIBRARY_DIR}/libqca2.a") #qca2.dll
    set(QCA2_INCLUDE_DIR "${QT_INCLUDE_DIR}/QtCrypto")
    set(QCA2_DEFINITIONS "-DQT_SHARED;-I${QCA2_INCLUDE_DIR}")
  ELSE(WIN32)
    MESSAGE("** ERROR: QCA2 NOT CONFIGURED !")
  ENDIF(WIN32)
ENDIF(NOT QCA2_FOUND)
IF(APPLE)
  MESSAGE(STATUS "QCA2_INCLUDE_DIR has value ${QCA2_INCLUDE_DIR}")
  MESSAGE(STATUS "QCA2_LIBRARIES has value ${QCA2_LIBRARIES}")
  MESSAGE(STATUS "QCA2_DEFINITIONS has value ${QCA2_DEFINITIONS}")
ENDIF(APPLE)
IF(STATIC_LIBRARY_FLAGS)
  find_package(OpenSSL)
  SET(QCA2_OSSL "${QT_PLUGINS_DIR}/crypto/libqca-ossl.a")
  IF(EXISTS ${QCA2_OSSL})
    MESSAGE(STATUS "* static QCA-OSSL plugin found.")
    IF(OPENSSL_FOUND)
      MESSAGE(STATUS "* static OpenSSL found.")
    ENDIF(OPENSSL_FOUND)
  ELSE(EXISTS ${QCA2_OSSL})
    MESSAGE(STATUS "/!\\ Cannot find QCA-OSSL: ${QCA2_OSSL} doesn't exists!")
  ENDIF(EXISTS ${QCA2_OSSL})
ELSE(STATIC_LIBRARY_FLAGS)
  SET( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/modules )
ENDIF(STATIC_LIBRARY_FLAGS)

INCLUDE_DIRECTORIES(
 ${QT_INCLUDE_DIR}
 ${QCA2_INCLUDE_DIR}
 ${sef_SOURCE_DIR}
)
INCLUDE(${QT_USE_FILE})

SET(secryptoaes_SRCS
  secryptoaes.cpp
)

SET(secryptoaes_MOC_HDRS
  secryptoaes.h 
)

QT4_WRAP_CPP( secryptoaes_SRCS ${secryptoaes_MOC_HDRS} )

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_NO_DEBUG)
IF(STATIC_LIBRARY_FLAGS)
  ADD_DEFINITIONS(-DQT_STATICPLUGIN)
ELSE(STATIC_LIBRARY_FLAGS)
  ADD_DEFINITIONS(-DQT_PLUGIN)
  ADD_DEFINITIONS(-DQT_SHARED)
ENDIF(STATIC_LIBRARY_FLAGS)
ADD_DEFINITIONS(-DQT_DLL)
ADD_DEFINITIONS(${QCA2_DEFINITIONS})

LINK_LIBRARIES(secryptoaes ${QCA2_LIBRARIES} ${QCA2_OSSL} ${OPENSSL_LIBRARIES} sef)

IF(STATIC_LIBRARY_FLAGS)
  ADD_LIBRARY(secryptoaes STATIC ${secryptoaes_SRCS} ${secryptoaes_MOC_HDRS})
  TARGET_LINK_LIBRARIES (secryptoaes ${QCA2_LIBRARIES} ${QCA2_OSSL} ${OPENSSL_LIBRARIES} ${QT_LIBRARIES} silenteye) 
ELSE(STATIC_LIBRARY_FLAGS)
  ADD_LIBRARY(secryptoaes SHARED ${secryptoaes_SRCS} ${secryptoaes_MOC_HDRS})
  TARGET_LINK_LIBRARIES (secryptoaes ${QT_LIBRARIES}) 
ENDIF(STATIC_LIBRARY_FLAGS)

if($ENV{ENABLE_TESTSUITE})
  message("** Enabled secryptoaes testsuite ")
  subdirs( ./testsuite )
endif($ENV{ENABLE_TESTSUITE})

IF($ENV{ENABLE_PACK})
  IF(NOT STATIC_LIBRARY_FLAGS)
    IF(WIN32)
      install(TARGETS secryptoaes RUNTIME DESTINATION modules COMPONENT secryptoaes)
      find_package(OpenSSL)
      
      STRING(REGEX REPLACE "/lib/lib" "/bin/" SYSTEM_LIB ${QCA2_LIBRARIES})
      STRING(REGEX REPLACE "\\.a" ".dll" SYSTEM_LIB ${SYSTEM_LIB})
      
      install(FILES ${SYSTEM_LIB}
	DESTINATION .
	COMPONENT secryptoaes)
      
      STRING(REGEX REPLACE "/include" "" OPENSSL_DIR ${OPENSSL_INCLUDE_DIR})
      MESSAGE(STATUS "OPENSSL_DIR has value ${OPENSSL_DIR}")	
      
      install(FILES "${OPENSSL_DIR}/libeay32.dll"
	DESTINATION .
	COMPONENT secryptoaes)
      install(FILES "${OPENSSL_DIR}/ssleay32.dll"
	DESTINATION .
	COMPONENT secryptoaes)
      
      install(FILES ${QT_PLUGINS_DIR}/crypto/qca-ossl2.dll
	DESTINATION lib/crypto
	COMPONENT secryptoaes)
    ELSE(WIN32)
      IF(APPLE)
        GET_TARGET_PROPERTY(LIB_LOCATION secryptoaes LOCATION)
        add_custom_command(TARGET secryptoaes POST_BUILD
          COMMAND mkdir ARGS -pv ${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/crypto
          COMMAND cp ARGS \"${QT_PLUGINS_DIR}/crypto/libqca-ossl.dylib\" ${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/crypto/
          COMMAND cp ARGS ${LIB_LOCATION} ${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/modules/
          COMMENT "copy secryptoaes plug-in into silenteye.app"
        )
      ELSE(APPLE)
        install(TARGETS secryptoaes DESTINATION modules COMPONENT secryptoaes)
      ENDIF(APPLE)
    ENDIF(WIN32)
  ENDIF(NOT STATIC_LIBRARY_FLAGS)
ENDIF($ENV{ENABLE_PACK})
