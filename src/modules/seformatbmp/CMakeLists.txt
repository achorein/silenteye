PROJECT( seformatbmp )
cmake_minimum_required(VERSION 2.6.0)

find_package(Qt4 REQUIRED)

SET( QT_USE_QTXML TRUE )

IF(NOT STATIC_LIBRARY_FLAGS)
  SET( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/modules )
ENDIF(NOT STATIC_LIBRARY_FLAGS)

INCLUDE(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
  ${CMAKE_BINARY_DIR}
  ${QT_INCLUDE_DIR}
  ${sef_SOURCE_DIR}
  )

SET(seformatbmp_SRCS
  seformatbmp.cpp
  imagebmp.cpp
  optionwidget.cpp
  )

SET(seformatbmp_MOC_HDRS
  seformatbmp.h 
  optionwidget.h
  )

SET(seformatbmp_UIS
  ui/optionwidget.ui
  )

QT4_WRAP_UI(seformatbmp_UIS_H ${seformatbmp_UIS})
QT4_WRAP_CPP( seformatbmp_SRCS ${seformatbmp_MOC_HDRS} )

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_NO_DEBUG)
IF(STATIC_LIBRARY_FLAGS)
  ADD_DEFINITIONS(-DQT_STATICPLUGIN)
ELSE(STATIC_LIBRARY_FLAGS)
  ADD_DEFINITIONS(-DQT_PLUGIN)
  ADD_DEFINITIONS(-DQT_SHARED)
ENDIF(STATIC_LIBRARY_FLAGS)
ADD_DEFINITIONS(-DQT_DLL)

LINK_LIBRARIES(seformatbmp sef)

IF(STATIC_LIBRARY_FLAGS)
  ADD_LIBRARY(seformatbmp STATIC ${seformatbmp_SRCS} ${seformatbmp_MOC_HDRS} ${seformatbmp_UIS_H})
  TARGET_LINK_LIBRARIES (seformatbmp
    silenteye
    ${QT_LIBRARIES} )
ELSE(STATIC_LIBRARY_FLAGS)
  ADD_LIBRARY(seformatbmp SHARED ${seformatbmp_SRCS} ${seformatbmp_MOC_HDRS} ${seformatbmp_UIS_H})
  TARGET_LINK_LIBRARIES (seformatbmp ${QT_LIBRARIES} )
ENDIF(STATIC_LIBRARY_FLAGS)

if($ENV{ENABLE_TESTSUITE})
  message("** Enabled seformatbmp testsuite ")
  subdirs( ./testsuite )
endif($ENV{ENABLE_TESTSUITE})

IF($ENV{ENABLE_PACK})
  IF(NOT STATIC_LIBRARY_FLAGS)
    IF(WIN32)
      install(TARGETS seformatbmp RUNTIME DESTINATION modules COMPONENT seformatbmp)
    ELSE(WIN32)
      IF(APPLE)
        GET_TARGET_PROPERTY(LIB_LOCATION seformatbmp LOCATION)
        add_custom_command(TARGET seformatbmp POST_BUILD
          COMMAND cp ARGS ${LIB_LOCATION} ${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/modules/
          COMMENT "copy seformatbmp plug-in into silenteye.app"
        )
      ELSE(APPLE)
        install(TARGETS seformatbmp DESTINATION ${INSTALL_LIBPATH}/modules COMPONENT seformatbmp)
      ENDIF(APPLE)
    ENDIF(WIN32)
  ENDIF(NOT STATIC_LIBRARY_FLAGS)
ENDIF($ENV{ENABLE_PACK})
