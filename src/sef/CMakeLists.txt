PROJECT( sef )

cmake_minimum_required(VERSION 2.8.3)

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project

SET( QT_USE_QTXML TRUE )
SET( QT_USE_QTMULTIMEDIA TRUE )

find_package(Qt4 REQUIRED)

IF(NOT STATIC_LIBRARY_FLAGS)
  SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
ENDIF(NOT STATIC_LIBRARY_FLAGS)

INCLUDE_DIRECTORIES(
  ${CMAKE_BINARY_DIR}
  ${QT_INCLUDE_DIR}
  ${sef_SOURCE_DIR}
)
INCLUDE(${QT_USE_FILE})

SET(sef_SRCS
    config.cpp
    data.cpp
    encodeddata.cpp
    media.cpp
    image.cpp
    audio.cpp
    video.cpp
    silenteyeexception.cpp
    moduleexception.cpp
    moduleinterface.h
    modulewidgetinterface.h
    formatmoduleinterface.h
    imagemoduleinterface.h
    audiomoduleinterface.h
    videomoduleinterface.h
    logger.cpp
)

SET(sef_MOC_HDRS
)

QT4_WRAP_CPP(sef_MOC_SRCS ${sef_MOC_HDRS})

ADD_DEFINITIONS(${QT_DEFINITIONS})
IF(NOT STATIC_LIBRARY_FLAGS)
  ADD_DEFINITIONS(-DQT_SHARED)
ENDIF(NOT STATIC_LIBRARY_FLAGS)
ADD_DEFINITIONS(-DQT_DLL)

IF(STATIC_LIBRARY_FLAGS)
  ADD_LIBRARY( sef STATIC ${sef_SRCS} ${sef_MOC_HDRS} ${sef_MOC_SRCS})
  TARGET_LINK_LIBRARIES(sef ${QT_LIBRARIES} silenteye)
ELSE(STATIC_LIBRARY_FLAGS)
  ADD_LIBRARY( sef SHARED ${sef_SRCS} ${sef_MOC_HDRS} ${sef_MOC_SRCS})
  TARGET_LINK_LIBRARIES(sef ${QT_LIBRARIES})
ENDIF(STATIC_LIBRARY_FLAGS)

if($ENV{ENABLE_TESTSUITE})
  message("** Enabled sef testsuite ")
  subdirs( ./testsuite )
endif($ENV{ENABLE_TESTSUITE})


##############################
# PACKAGING
##############################
IF($ENV{ENABLE_PACK})
  IF(WIN32)
    install(TARGETS sef RUNTIME DESTINATION . COMPONENT Application)
  ELSE(WIN32)
    install(TARGETS sef DESTINATION . COMPONENT Application)
  ENDIF(WIN32)
ENDIF($ENV{ENABLE_PACK})

