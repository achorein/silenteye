PROJECT( silenteye ) # the name of your project

cmake_minimum_required(VERSION 2.8.5)

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project

SET( QT_USE_QTXML TRUE )
SET( QT_USE_QTNETWORK TRUE )
SET( QT_USE_QTMULTIMEDIA TRUE )

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/)

INCLUDE(${QT_USE_FILE})

SET(silenteye_SRCS
  main.cpp
  modulemanager.cpp
  modulecommander.cpp
  controller.cpp
  mainwindow.cpp
  optiondialog.cpp
  decodedialog.cpp
  encodedialog.cpp
  propertydialog.cpp
  preferencedialog.cpp
  updatedialog.cpp
  mediawidget.cpp
  )

SET(silenteye_MOC_HDRS
  controller.h
  aboutdialog.h
  updatedialog.h
  decodedialog.h
  encodedialog.h
  optiondialog.h
  mainwindow.h
  propertydialog.h
  preferencedialog.h
  mediawidget.h
)

SET(silenteye_UIS
  ui/mainwindow.ui
  ui/mediawidget.ui
  ui/optiondialog.ui
  ui/propertydialog.ui
  ui/preferencedialog.ui
  ui/aboutdialog.ui
  ui/updatedialog.ui
)

SET(silenteye_RCS
  silenteye.qrc
  )

# custom path configuration (conf.h.in)
MESSAGE("** Use configuration file conf.h.in")
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/conf.h.in ${CMAKE_CURRENT_BINARY_DIR}/conf.h)

IF(STATIC_LIBRARY_FLAGS)
  ADD_DEFINITIONS(-DQT_STATICPLUGIN)
ENDIF(STATIC_LIBRARY_FLAGS)

QT4_ADD_RESOURCES( silenteye_RC_SRCS ${silenteye_RCS} )

QT4_WRAP_UI(silenteye_UIS_H ${silenteye_UIS})
QT4_WRAP_CPP(silenteye_MOC_SRCS ${silenteye_MOC_HDRS})

INCLUDE_DIRECTORIES( 
  silenteye 
  ${sef_SOURCE_DIR}
  )

IF(NOT STATIC_LIBRARY_FLAGS)
  LINK_LIBRARIES(silenteye sef)
ELSE(NOT STATIC_LIBRARY_FLAGS)
  LINK_LIBRARIES(silenteye sef
    secryptoaes
    secryptoaes256
    seformatbmp
    seformatjpeg
    seformatwav
    )
ENDIF(NOT STATIC_LIBRARY_FLAGS)

IF(WIN32)
  ADD_EXECUTABLE(silenteye WIN32 ${silenteye_SRCS} ${silenteye_MOC_SRCS} ${silenteye_UIS_H} ${silenteye_RC_SRCS})
endif( WIN32)
IF(APPLE)
  ADD_EXECUTABLE(silenteye MACOSX_BUNDLE ${silenteye_SRCS} ${silenteye_MOC_SRCS} ${silenteye_UIS_H} ${silenteye_RC_SRCS})
  add_custom_command(TARGET silenteye PRE_BUILD 
       COMMAND mkdir ARGS -pv ${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/modules
       COMMENT "Create ./silenteye.app/Contents/MacOS/modules directory")
ENDIF(APPLE)
IF(UNIX AND NOT APPLE)
  ADD_EXECUTABLE(silenteye ${silenteye_SRCS} ${silenteye_MOC_SRCS} ${silenteye_UIS_H} ${silenteye_RC_SRCS})
ENDIF(UNIX AND NOT APPLE)

TARGET_LINK_LIBRARIES(silenteye ${QT_LIBRARIES} ${GUI_LIBS})
IF(STATIC_LIBRARY_FLAGS)
  TARGET_LINK_LIBRARIES(silenteye
    secryptoaes
    secryptoaes256
    seformatbmp
    seformatjpeg
    seformatwav
    )
ENDIF(STATIC_LIBRARY_FLAGS)

############################
# PACKAGING
############################
IF($ENV{ENABLE_PACK})
  IF(NOT APPLE)
    IF(WIN32)
      install(TARGETS silenteye
	RUNTIME DESTINATION . COMPONENT Application
      )
    ELSE(WIN32)
      #unix
      install(TARGETS silenteye
	RUNTIME DESTINATION ${INSTALL_BINPATH} COMPONENT Application
      )
    ENDIF(WIN32)
  ELSE(NOT APPLE)
    install(TARGETS silenteye
      RUNTIME DESTINATION . COMPONENT Application
      BUNDLE DESTINATION . COMPONENT Application
    )
    file(COPY ${QT_PLUGINS_DIR}/iconengines DESTINATION ${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/)
    file(COPY ${QT_PLUGINS_DIR}/imageformats DESTINATION ${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/)
    FILE(GLOB SILENTEYE_LIBRARY "${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/modules/*.dylib")
    FILE(GLOB ICON_LIBRARY "${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/iconengines/*.dylib")
    FILE(GLOB IMAGE_LIBRARY "${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/imageformats/*.dylib")
    FILE(COPY /opt/local/lib/Resources/qt_menu.nib DESTINATION ${CMAKE_BINARY_DIR}/silenteye.app/Contents/Resources/)
    message("${QT_PLUGINS_DIR}")
    install(CODE "
        include(BundleUtilities)
        fixup_bundle(\"${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/silenteye\"  \"${SILENTEYE_LIBRARY};${ICON_LIBRARY};${IMAGE_LIBRARY};${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/crypto/libqca-ossl.dylib\"  \".\")
        " COMPONENT Application
    )
    file(COPY img/silenteye.icns DESTINATION ${CMAKE_BINARY_DIR}/silenteye.app/Contents/Resources/)
  ENDIF(NOT APPLE)
  IF(UNIX AND NOT APPLE)
    install(FILES "img/silenteye.png"
      DESTINATION ${INSTALL_PIXPATH}
      COMPONENT Application)
    IF($ENV{ENABLE_GNOMEMENU})
      install(FILES "../silenteye.desktop"
	DESTINATION /usr/share/applications
	COMPONENT Application)      
    ENDIF($ENV{ENABLE_GNOMEMENU})
  ENDIF(UNIX AND NOT APPLE)
  IF( WIN32 )
    install(FILES "img/silenteye.ico"
      DESTINATION .
      COMPONENT Application)
    
    FOREACH ( SYSTEM_LIB ${QT_LIBRARIES} )
      STRING(REGEX REPLACE "/lib/lib" "/bin/" SYSTEM_LIB ${SYSTEM_LIB})
      STRING(REGEX REPLACE "\\.a" ".dll" SYSTEM_LIB ${SYSTEM_LIB})
      MESSAGE(STATUS "SYSTEM_LIB has value ${SYSTEM_LIB}")
      install(FILES ${SYSTEM_LIB}
	DESTINATION . COMPONENT Application
      )
    ENDFOREACH ( SYSTEM_LIB )
    
    install(FILES ${LIST_SYSTEM_LIB}
      DESTINATION .
      COMPONENT Application)
    
    install(FILES "${QT_BINARY_DIR}/libgcc_s_dw2-1.dll"
      DESTINATION .
      COMPONENT Application)
    
    FILE(GLOB ICONENGINES_LIBRARY "${QT_PLUGINS_DIR}/iconengines/*.dll")
    install(FILES ${ICONENGINES_LIBRARY}
      DESTINATION lib/iconengines
      COMPONENT Application)
    
    FILE(GLOB IMAGEFORMATS_LIBRARY "${QT_PLUGINS_DIR}/imageformats/*.dll")
    install(FILES ${IMAGEFORMATS_LIBRARY}
      DESTINATION lib/imageformats
      COMPONENT Application)
    
    STRING(REGEX REPLACE "mingw32-make.exe" "mingwm10.dll" SYSTEM_LIB ${CMAKE_MAKE_PROGRAM})
    install(FILES ${SYSTEM_LIB}
      DESTINATION .
      COMPONENT Application)
  ENDIF( WIN32 )
ENDIF($ENV{ENABLE_PACK})
