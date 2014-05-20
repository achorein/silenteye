IF(WIN32)
  PROJECT(SilentEye)
ELSE(WIN32)
  PROJECT(silenteye)
ENDIF(WIN32)

cmake_minimum_required(VERSION 2.8.5)

IF($ENV{ENABLE_STATIC})
  MESSAGE("** Build in static mode : enable static plugins")
  OPTION(STATIC_BUILD "enable static build support" ON)
  SET(STATIC_LIBRARY_FLAGS TRUE)
ELSE($ENV{ENABLE_STATIC})
  MESSAGE("** Build in shared mode")
ENDIF($ENV{ENABLE_STATIC})

# BIN PATH
IF(NOT "$ENV{BINPATH_VALUE}" STREQUAL "")
  SET(BINPATH_VALUE $ENV{BINPATH_VALUE})
  MESSAGE("** Use custom bin path: ${BINPATH_VALUE}")
  SET(INSTALL_BINPATH ${BINPATH_VALUE}) # default install
ELSEIF($ENV{ENABLE_PACK})
  IF(UNIX AND NOT APPLE)
    SET(INSTALL_BINPATH "/usr/bin") # default install dir
    MESSAGE("** Use default bin path: ${INSTALL_BINPATH}")
    SET(BINPATH_VALUE ${INSTALL_BINPATH})
  ELSE(UNIX AND NOT APPLE)
    SET(INSTALL_BINPATH ".")
  ENDIF(UNIX AND NOT APPLE)
ENDIF(NOT "$ENV{BINPATH_VALUE}" STREQUAL "")
# CONF PATH
IF($ENV{CONFPATH_USERHOME})
  ADD_DEFINITIONS(-DCONFPATH_USERHOME)
  MESSAGE("** Use user's home as configuration path for application: $HOME/.silenteye/silenteye.conf")
  IF("$ENV{CONFPATH_VALUE}" STREQUAL "")
    SET(INSTALL_CONFPATH "/etc")
    MESSAGE("** Use default configuration path installation: ${INSTALL_CONFPATH}/silenteye.conf")
  ELSE("$ENV{CONFPATH_VALUE}" STREQUAL "")
    SET(INSTALL_CONFPATH $ENV{CONFPATH_VALUE}) # install dir
    MESSAGE("** Use custom configuration path installation: ${INSTALL_CONFPATH}/silenteye.conf")
  ENDIF("$ENV{CONFPATH_VALUE}" STREQUAL "")
ELSEIF(NOT "$ENV{CONFPATH_VALUE}" STREQUAL "")
  SET(CONFPATH_VALUE $ENV{CONFPATH_VALUE})
  MESSAGE("** Use custom configuration path: ${CONFPATH_VALUE}/silenteye.conf")
  SET(INSTALL_CONFPATH ${CONFPATH_VALUE}) # install dir
ELSEIF($ENV{ENABLE_PACK})
  IF(UNIX AND NOT APPLE)
    SET(INSTALL_CONFPATH "/etc") # default install dir
  ELSE(UNIX AND NOT APPLE)
    SET(INSTALL_CONFPATH ".") # default install dir
  ENDIF(UNIX AND NOT APPLE)
  SET(CONFPATH_VALUE ${INSTALL_CONFPATH})
  MESSAGE("** Use default configuration path: ${INSTALL_CONFPATH}/silenteye.conf")
ENDIF($ENV{CONFPATH_USERHOME})
# LIB PATH
IF(NOT "$ENV{LIBPATH_VALUE}" STREQUAL "")
  SET(LIBPATH_VALUE $ENV{LIBPATH_VALUE})
  MESSAGE("** Use custom lib path: ${LIBPATH_VALUE}")
  SET(INSTALL_LIBPATH $ENV{LIBPATH_VALUE}) # install dir
ELSEIF($ENV{ENABLE_PACK})
  IF(UNIX AND NOT APPLE)
    SET(INSTALL_LIBPATH "/usr/lib/silenteye") # default install dir
    MESSAGE("** Use default lib path: ${INSTALL_LIBPATH}/modules/*.so")
    SET(LIBPATH_VALUE ${INSTALL_LIBPATH})
  ELSE(UNIX AND NOT APPLE)
    SET(INSTALL_LIBPATH ".")
  ENDIF(UNIX AND NOT APPLE)
ENDIF(NOT "$ENV{LIBPATH_VALUE}" STREQUAL "")
# COMMON PATH
IF(NOT "$ENV{CMNPATH_VALUE}" STREQUAL "")
  SET(CMNPATH_VALUE $ENV{CMNPATH_VALUE})
  MESSAGE("** Use custom common path: ${CMNPATH_VALUE}/version.xml")
  SET(INSTALL_CMNPATH ${CMNPATH_VALUE}) # default install
ELSEIF($ENV{ENABLE_PACK})
  IF(UNIX AND NOT APPLE)
    SET(INSTALL_CMNPATH "/usr/share/silenteye") # default install dir
    MESSAGE("** Use default common path: ${INSTALL_CMNPATH}/version.xml")
    SET(CMNPATH_VALUE ${INSTALL_CMNPATH})
  ELSE(UNIX AND NOT APPLE)
    SET(INSTALL_CMNPATH ".")
  ENDIF(UNIX AND NOT APPLE)
ENDIF(NOT "$ENV{CMNPATH_VALUE}" STREQUAL "")
# LOG PATH
IF($ENV{LOGPATH_USERHOME})
  ADD_DEFINITIONS(-DLOGPATH_USERHOME)
  MESSAGE("** Use user's home as log path: $HOME/.silenteye/silenteye.log")
ELSEIF(NOT "$ENV{LOGPATH_VALUE}" STREQUAL "")
  SET(LOGPATH_VALUE $ENV{LOGPATH_VALUE})
  MESSAGE("** Use custom log path: ${LOGPATH_VALUE}/silenteye.log")
ELSEIF($ENV{ENABLE_PACK})
  IF(UNIX AND NOT APPLE)
    SET(LOGPATH_VALUE "/var/log")
    MESSAGE("** Use default log path: ${LOGPATH_VALUE}/silenteye.log")
  ENDIF(UNIX AND NOT APPLE)
ENDIF($ENV{LOGPATH_USERHOME})
# DOC PATH
IF($ENV{ENABLE_PACK})
  IF(NOT "$ENV{DOCPATH_VALUE}" STREQUAL "")
    SET(INSTALL_DOCPATH "$ENV{DOCPATH_VALUE}")
    MESSAGE("** Use custom doc path: ${INSTALL_DOCPATH}")
  ELSE(NOT "$ENV{DOCPATH_VALUE}" STREQUAL "")
    SET(INSTALL_DOCPATH "/usr/share/doc/silenteye")
    MESSAGE("** Use default doc path: ${INSTALL_DOCPATH}")
  ENDIF(NOT "$ENV{DOCPATH_VALUE}" STREQUAL "")
ENDIF($ENV{ENABLE_PACK})
# PIX PATH
IF($ENV{ENABLE_PACK})
  IF(NOT "$ENV{PIXPATH_VALUE}" STREQUAL "")
    SET(INSTALL_PIXPATH "$ENV{PIXPATH_VALUE}")
    MESSAGE("** Use custom pixmap path: ${INSTALL_PIXPATH}")
  ELSE(NOT "$ENV{PIXPATH_VALUE}" STREQUAL "")
    SET(INSTALL_PIXPATH "/usr/share/pixmaps")
    MESSAGE("** Use default pixmap path: ${INSTALL_PIXPATH}")
  ENDIF(NOT "$ENV{PIXPATH_VALUE}" STREQUAL "")
  IF($ENV{ENABLE_GNOMEMENU})
    MESSAGE("** Use configuration file install/linux/silenteye.desktop.cmake")
    CONFIGURE_FILE(${CMAKE_BINARY_DIR}/install/linux/silenteye.desktop.cmake ${CMAKE_BINARY_DIR}/silenteye.desktop)
  ENDIF($ENV{ENABLE_GNOMEMENU})
ENDIF($ENV{ENABLE_PACK})

#######################################################
# MANAGE PROJECT COMPILATION
#######################################################

SET(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}/src/cmake)

IF(APPLE)
  SET(MACOSX_BUNDLE_INFO_STRING "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
  SET(MACOSX_BUNDLE_ICON_FILE "silenteye.icns")
  SET(MACOSX_BUNDLE_GUI_IDENTIFIER "SilentEye")  
  SET(MACOSX_BUNDLE_LONG_VERSION_STRING "${CPACK_PACKAGE_VERSION}")
  SET(MACOSX_BUNDLE_BUNDLE_NAME "SilentEye")
  SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "${CPACK_PACKAGE_VERSION}")
  SET(MACOSX_BUNDLE_VERSION "${CPACK_PACKAGE_VERSION}")
  SET(MACOSX_BUNDLE_COPYRIGHT "GNU GPLv3")
ENDIF(APPLE)
IF(WIN32)
  # Install specific cmake module files
  MESSAGE("** Install cmake module files")
  FILE(GLOB cmakeFiles "${CMAKE_BINARY_DIR}/src/cmake/*.cmake")

  SET(MODULE_INSTALL_DIR ${CMAKE_BINARY_DIR}/src/cmake)

  INSTALL(FILES ${cmakeFiles}
    DESTINATION ${MODULE_INSTALL_DIR})
ENDIF(WIN32)
IF(UNIX)
  # don't skip the full RPATH for the build tree
  SET(CMAKE_SKIP_BUILD_RPATH FALSE)
  # when building, don't use the install RPATH already
  SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
  # the RPATH to be used when installing (installation dir)
  SET(CMAKE_INSTALL_RPATH ${INSTALL_LIBPATH})
  # add the automatically determined parts of the RPATH
  # which point to directories outside the build tree to the install RPATH
  SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE) 
ENDIF(UNIX)

# Set the compilation parameters	
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -Wall")

IF($ENV{ENABLE_DEBUG})
  SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -Wall -g -ggdb")
  MESSAGE("** Enabled DebugMode")
ENDIF($ENV{ENABLE_DEBUG})

SUBDIRS( ./src/sef ) #compile the silent eye framework

# build modules (plugins)
IF($ENV{ENABLE_MODULE})
  MESSAGE("** Enabled modules compilation ")
  SUBDIRS( ./src/modules )
ENDIF($ENV{ENABLE_MODULE})

SUBDIRS( ./src ) 

#######################################################
# MANAGE PROJECT PACKAGING
#######################################################

IF($ENV{ENABLE_PACK})
  install(FILES silenteye.conf
    DESTINATION ${INSTALL_CONFPATH}
    COMPONENT Application)
  
  install(FILES version.xml
    DESTINATION ${INSTALL_CMNPATH}
    COMPONENT Application)
  
  IF(WIN32)
    install(FILES "install/windows/qt.conf"
      DESTINATION .
      COMPONENT Application)
#    install(FILES "install/windows/seshell.exe"
#      DESTINATION lib
#      COMPONENT ContextMenu)
#    install(FILES "install/windows/ShellMenu.exe"
#      DESTINATION lib
#      COMPONENT ContextMenu)
  ENDIF(WIN32)
  
  IF(APPLE)
    file(COPY silenteye.conf DESTINATION ${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/)
    file(COPY version.xml DESTINATION ${CMAKE_BINARY_DIR}/silenteye.app/Contents/MacOS/)
  ENDIF(APPLE)

  install(FILES README
    DESTINATION ${INSTALL_DOCPATH}
    COMPONENT Application)
  
  install(FILES COPYING
    DESTINATION ${INSTALL_DOCPATH}
    COMPONENT Application)
  
  SET(CPACK_PACKAGE_NAME "silenteye")
  SET(CPACK_PACKAGE_VENDOR "SilentEye")
  SET(CPACK_PACKAGE_CONTACT "Anselme CHOREIN <anselme@chorein.fr>")
  SET(CPACK_PACKAGE_DESCRIPTION "SilentEye: Steganography is yours!")
  SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "SilentEye is a cross-plateform application design for an easy use of steganography, in this case hidding messages into pictures and sounds. It provides a pretty nice interface and an easy integration of new steganography algorithm and cryptography process by using a plug-ins system.")
  SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
  SET(CPACK_PACKAGE_VERSION_MAJOR "0")
  SET(CPACK_PACKAGE_VERSION_MINOR "4")
  SET(CPACK_PACKAGE_VERSION_PATCH "2")
  set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
  
  SET(CPACK_COMPONENTS_ALL Application seformatjpeg seformatbmp secryptoaes secryptoaes256 seformatwav )
  
  SET(CPACK_COMPONENT_APPLICATION_REQUIRED TRUE)
  SET(CPACK_COMPONENT_APPLICATION_DISPLAY_NAME "Application GUI")
  SET(CPACK_COMPONENT_APPLICATION_DESCRIPTION "An extremely useful application")
  
#  SET(CPACK_COMPONENT_CONTEXTMENU_DISPLAY_NAME "Context Menu Entry")
#  SET(CPACK_COMPONENT_CONTEXTMENU_DESCRIPTION "Explorer context menu entry for SilentEye: Encode and decode image you want in SilentEye from Windows Explorer.")
  
  SET(CPACK_COMPONENT_LIBRARIES_DEPENDS APPLICATIONS)
  
  SET(CPACK_COMPONENT_SEFORMATBMP_GROUP "Plugins")
  SET(CPACK_COMPONENT_SEFORMATBMP_DISPLAY_NAME "SEFormatJPEG")
  SET(CPACK_COMPONENT_SEFORMATBMP_DESCRIPTION "Support for encoding and decoding JPEG image format")
  
  SET(CPACK_COMPONENT_SEFORMATBMP_GROUP "Plugins")
  SET(CPACK_COMPONENT_SEFORMATBMP_DISPLAY_NAME "SEFormatBMP")
  SET(CPACK_COMPONENT_SEFORMATBMP_DESCRIPTION "Support for encoding and decoding BMP image format")
  
  SET(CPACK_COMPONENT_SECRYPTOAES_GROUP "Plugins")
  SET(CPACK_COMPONENT_SECRYPTOAES_DISPLAY_NAME "SEFormatAES")
  SET(CPACK_COMPONENT_SECRYPTOAES_DESCRIPTION "Support for AES-128 Encryption")
  
  SET(CPACK_COMPONENT_SECRYPTOAES_GROUP "Plugins")
  SET(CPACK_COMPONENT_SECRYPTOAES_DISPLAY_NAME "SEFormatAES256")
  SET(CPACK_COMPONENT_SECRYPTOAES_DESCRIPTION "Support for AES-256 Encryption")
  
  SET(CPACK_COMPONENT_SEFORMATBMP_GROUP "Plugins")
  SET(CPACK_COMPONENT_SEFORMATBMP_DISPLAY_NAME "SEFormatWAV")
  SET(CPACK_COMPONENT_SEFORMATBMP_DESCRIPTION "Support for encoding and decoding WAV audio format")
  
  SET(CPACK_COMPONENT_GROUP_PLUGINS_DESCRIPTION "All of the plugins you'll ever need")
  
  IF(WIN32)
    SET(CPACK_GENERATOR "TZ") # package for Debian/Ubuntu
    SET(CPACK_PACKAGE_INSTALL_DIRECTORY "SilentEye")
    SET(CPACK_PACKAGE_EXECUTABLES "SilentEye" "${CPACK_PACKAGE_NAME}") # used for startmenu
    SET(ARCHI "win32")
  ELSEIF(UNIX)
    SET(CPACK_GENERATOR "TZ")
    #SET(CPACK_GENERATOR "DEB") # package for Debian/Ubuntu
    #SET(CPACK_PACKAGING_INSTALL_PREFIX "/opt/${CPACK_PACKAGE_NAME}")
    #SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libqtgui4 (= 4:4.7.1), libqtmultimediakit1(>= 1.0.2), libqca2 (>= 2.0.0)")
    #SET(CPACK_DEBIAN_PACKAGE_RECOMMENDS "libqca2-plugin-ossl (>= 0.1~20070904-4)")
    SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE  "i386")
    #SET(CPACK_DEBIAN_PACKAGE_SECTION "utils")
    #SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    #SET(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_BINARY_DIR}/install/linux/debian/postinst;${CMAKE_BINARY_DIR}/install/linux/debian/postrm")
    SET(ARCHI ${CPACK_DEBIAN_PACKAGE_ARCHITECTURE} )
  ENDIF(WIN32)

  IF(WIN32)
    SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${ARCHI}")  
  ELSE(WIN32)
    SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${ARCHI}")  
    #SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${ARCHI}")
  ENDIF(WIN32)

  INCLUDE(CPack)
ENDIF($ENV{ENABLE_PACK})
