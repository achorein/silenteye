PROJECT( seformatjpeg )
cmake_minimum_required(VERSION 2.6.0)

find_package(Qt4 REQUIRED)

IF(NOT STATIC_LIBRARY_FLAGS)
  SET( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/modules )
ENDIF(NOT STATIC_LIBRARY_FLAGS)

INCLUDE_DIRECTORIES(
 ${CMAKE_BINARY_DIR}
 ${QT_INCLUDE_DIR}
 ${sef_SOURCE_DIR}
)
INCLUDE(${QT_USE_FILE})

SET(seformatjpeg_SRCS
  stegotable.cpp
  ycbcr.cpp
  groupedimage.cpp
  imagejpeg.cpp
  optionwidget.cpp
  seformatjpeg.cpp
)

SET(seformatjpeg_MOC_HDRS
  seformatjpeg.h 
  optionwidget.h
)

SET(seformatjpeg_UIS
  ui/optionwidget.ui
)

QT4_WRAP_UI(seformatjpeg_UIS_H ${seformatjpeg_UIS})
QT4_WRAP_CPP( seformatjpeg_SRCS ${seformatjpeg_MOC_HDRS} )

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_NO_DEBUG)
IF(STATIC_LIBRARY_FLAGS)
  ADD_DEFINITIONS(-DQT_STATICPLUGIN)
ELSE(STATIC_LIBRARY_FLAGS)
  ADD_DEFINITIONS(-DQT_PLUGIN)
  ADD_DEFINITIONS(-DQT_SHARED)
ENDIF(STATIC_LIBRARY_FLAGS)
ADD_DEFINITIONS(-DQT_DLL)

LINK_LIBRARIES(seformatjpeg sef)

IF(STATIC_LIBRARY_FLAGS)
  ADD_LIBRARY(seformatjpeg STATIC ${seformatjpeg_SRCS} ${seformatjpeg_MOC_HDRS} ${seformatjpeg_UIS_H})
  TARGET_LINK_LIBRARIES (seformatjpeg ${QT_LIBRARIES} silenteye) 
ELSE(STATIC_LIBRARY_FLAGS)
  ADD_LIBRARY(seformatjpeg SHARED ${seformatjpeg_SRCS} ${seformatjpeg_MOC_HDRS} ${seformatjpeg_UIS_H})
  TARGET_LINK_LIBRARIES (seformatjpeg ${QT_LIBRARIES} ) 
ENDIF(STATIC_LIBRARY_FLAGS)

if($ENV{ENABLE_TESTSUITE})
  message("** Enabled seformatjpeg testsuite ")
  subdirs( ./testsuite )
endif($ENV{ENABLE_TESTSUITE})

IF($ENV{ENABLE_PACK})
  IF(NOT STATIC_LIBRARY_FLAGS)
    IF(WIN32)
      install(TARGETS seformatjpeg RUNTIME DESTINATION modules COMPONENT seformatjpeg)
    ELSE(WIN32)
      IF(APPLE)
        GET_TARGET_PROPERTY(LIB_LOCATION seformatjpeg LOCATION)
        add_custom_command(TARGET seformatjpeg POST_BUILD
          COMMAND cp ARGS ${LIB_LOCATION} ${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/modules/
          COMMENT "copy seformatjpeg plug-in into silenteye.app"
        )
      ELSE(APPLE)
        install(TARGETS seformatjpeg DESTINATION modules COMPONENT seformatjpeg)
      ENDIF(APPLE)
    ENDIF(WIN32)
  ENDIF(NOT STATIC_LIBRARY_FLAGS)
ENDIF($ENV{ENABLE_PACK})
