PROJECT( seformatwav )
cmake_minimum_required(VERSION 2.8.5)

find_package(Qt4 REQUIRED)

SET( QT_USE_QTXML TRUE )

IF(NOT STATIC_LIBRARY_FLAGS)
  SET( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/modules )
ENDIF(NOT STATIC_LIBRARY_FLAGS)

INCLUDE_DIRECTORIES(
 ${CMAKE_BINARY_DIR}
 ${QT_INCLUDE_DIR}
 ${sef_SOURCE_DIR}
)
INCLUDE(${QT_USE_FILE})

SET(seformatwav_SRCS
  audiowav.cpp
  seformatwav.cpp
  optionwidget.cpp
)

SET(seformatwav_MOC_HDRS
  seformatwav.h 
  optionwidget.h
)

SET(seformatwav_UIS
  ui/optionwidget.ui
)

QT4_WRAP_UI(seformatwav_UIS_H ${seformatwav_UIS})
QT4_WRAP_CPP( seformatwav_SRCS ${seformatwav_MOC_HDRS} )

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

LINK_LIBRARIES(seformatwav sef)

IF(STATIC_LIBRARY_FLAGS)
  ADD_LIBRARY(seformatwav STATIC ${seformatwav_SRCS} ${seformatwav_MOC_HDRS} ${seformatwav_UIS_H})
  TARGET_LINK_LIBRARIES (seformatwav ${QT_LIBRARIES} silenteye) 
ELSE(STATIC_LIBRARY_FLAGS)
  ADD_LIBRARY(seformatwav SHARED ${seformatwav_SRCS} ${seformatwav_MOC_HDRS} ${seformatwav_UIS_H})
  TARGET_LINK_LIBRARIES (seformatwav ${QT_LIBRARIES} ) 
ENDIF(STATIC_LIBRARY_FLAGS)

if($ENV{ENABLE_TESTSUITE})
  message("** Enabled seformatwav testsuite ")
  subdirs( ./testsuite )
endif($ENV{ENABLE_TESTSUITE})

IF($ENV{ENABLE_PACK})
  IF(NOT STATIC_LIBRARY_FLAGS)
    IF(WIN32)
      install(TARGETS seformatwav RUNTIME DESTINATION modules COMPONENT seformatwav)
    ELSE(WIN32)
      IF(APPLE)
        GET_TARGET_PROPERTY(LIB_LOCATION seformatwav LOCATION)
        add_custom_command(TARGET seformatwav POST_BUILD
          COMMAND cp ARGS ${LIB_LOCATION} ${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/modules/
          COMMENT "copy seformatwav plug-in into silenteye.app"
        )
      ELSE(APPLE)
        install(TARGETS seformatwav DESTINATION ${INSTALL_LIBPATH}/modules COMPONENT seformatwav)
      ENDIF(APPLE)
    ENDIF(WIN32)
  ENDIF(NOT STATIC_LIBRARY_FLAGS)
ENDIF($ENV{ENABLE_PACK})
