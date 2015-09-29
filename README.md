silenteye
=========

SilentEye is a cross-platform application design for an easy use of steganography, in this case hiding messages into pictures or sounds. It provides a pretty nice interface and an easy integration of new steganography algorithm and cryptography process by using a plug-ins system.
SilentEye is free to use (under GNU GPL v3).

Read more : http://www.silenteye.org

Download executable : http://www.silenteye.org/download.html?i2 or http://sourceforge.net/projects/silenteye/files/

# Main Features

* Hide information into images and sounds (LSB) : JPEG, BMP, WAVE
* Encrypte data : AES128, AES256
* Capacity to hide text or file
* zlib compression of message
* Drag & Drop

# How can i run SilentEye from command line? (batch mode)

## Requirements

Batch mode is only available from version 0.4.3 of SilentEye. (source here on branch 0.4)

You can change encode and decode options by editing the following files :
* silenteye.conf
* seformatbmp.conf
* seformatjpeg.conf
* seformatwav.conf
Configuration files can be found into user's home or SilentEye directory

## Examples

Basics:
```
./silenteye --batch encode ./myfile.png --message "my secret message"
./silenteye --batch encode ./myfile.png --message_file ./message.txt
```

output format and directory depends on silenteye.conf

Encryption:
```
./silenteye --batch encode ./myfile.png --message "my secret message" --password "my password"
```

# How can i compile source code? on Ubuntu/Unix

## Requirments

```
apt-get install -y g++ libfontconfig1-dev libfreetype6-dev libx11-dev libxcursor-dev libxext-dev libxfixes-dev libxft-dev libxi-dev libxrandr-dev libxrender-dev libssl-dev
```

## Qt

You have to install Qt from source with the following commands:

```Shell
root$ wget http://download.qt.io/official_releases/qt/4.8/4.8.7/qt-everywhere-opensource-src-4.8.7.tar.gz
root$ tar -zxvf qt-everywhere-opensource-src-4.8.7.tar.gz
root$ cd qt-everywhere-opensource-src-4.8.7
root$ ./configure -release -nomake examples -nomake demos -no-qt3support -no-scripttools -no-opengl -no-webkit -no-phonon -no-sql-sqlite -gtkstyle -opensource -prefix /usr/local/Qt-4.8.7-release
root$ make
root$ make install
```

> or for static compilation :
```
root$ ./configure -static -nomake examples -nomake demos -no-qt3support -no-scripttools -no-opengl -no-webkit -no-phonon -no-sql-sqlite -gtkstyle -opensource -prefix /usr/local/Qt-4.8.7-release
```

## QCA2

QCA is also required for encryption support (modules) :

```Shell
root$ wget http://delta.affinix.com/download/qca/2.0/qca-2.0.3.tar.bz2
root$ bunzip2 qca-2.0.3.tar.bz2 && tar -xvf qca-2.0.3.tar
root$ cd qca-2.0.3
root$ patch src/botantools/botan/botan/secmem.h fix_build_gcc4.7.diff
root$ ./configure --qtdir=/usr/local/Qt-4.8.7-release/
root$ make
root$ make install
```

Download patch : [fix_build_gcc4.7.diff](https://github.com/DarkReZuS/silenteye/blob/0.4/vagrant/vagrant_data/fix_build_gcc4.7.diff)

> or for static compilation :
```
./configure --qtdir=/usr/local/Qt-4.8.7-static/ --static
```

## QCA-OSSL

next is QCA plugin for OpenSSL support :

```Shell
root$ wget http://delta.affinix.com/download/qca/2.0/plugins/qca-ossl-2.0.0-beta3.tar.bz2
root$ bunzip2 qca-ossl-2.0.0-beta3.tar.bz2 && tar -xvf qca-ossl-2.0.0-beta3.tar
root$ cd qca-ossl-2.0.0-beta3
root$ patch qca-ossl.cpp < detect_ssl2_available.diff
root$ patch qca-ossl.cpp < detect_md2_available.diff
root$ ./configure --qtdir=/usr/local/Qt-4.8.7-release/
root$ make
root$ make install
```

Download patch : [detect_ssl2_available.diff](https://github.com/DarkReZuS/silenteye/blob/0.4/vagrant/vagrant_data/detect_ssl2_available.diff) and [detect_md2_available.diff](https://github.com/DarkReZuS/silenteye/blob/0.4/vagrant/vagrant_data/detect_md2_available.diff)

> or for static compilation :
```
./configure --qtdir=/usr/local/Qt-4.8.7-static/ --static
```

## CMake

CMake (version > 2.8.10 required) :

```Shell
root$ apt-get install cmake
```

## Get source code

Then download and move into source folder :

```Shell
user$ git clone --branch 0.4 https://github.com/DarkReZuS/silenteye.git
user$ cd silenteye
```

## Let's compile

Now you can generate MakeFile like this :

```Shell
user$ export PATH=/usr/local/Qt-4.8.7-release/bin:$PATH
user$ ENABLE_MODULE=1 cmake .
```

> or for static compilation :
```
user$ export PATH=$PATH:/usr/local/Qt-4.8.7-static/bin
user$ ENABLE_MODULE=1 ENABLE_STATIC=1 cmake .
```

Just run make command to build the project
```
make
```

## Run

You can now run silenteye with the following command :

```Shell
./silenteye
```

## Install on system

Installation information can be found in "Compilation options" section (use of ENABLE_PACK=1).

### Installation Example

```
$root> ENABLE_PACK=1 ENABLE_MODULE=1 LOGPATH_USERHOME=1 CONFPATH_USERHOME=1 ENABLE_GNOMEMENU=1 cmake .
$root> make
$root> make install
or
$root> make install DESTDIR=$PKG
```

# How can i compile source code? on Windows

## Requirments

Download and install the following tools :

* Qt 4.8 SDK (MinGW) : http://download.qt.io/official_releases/qt/4.8/4.8.7/qt-opensource-windows-x86-mingw482-4.8.7.exe
* CMake 2.8.12 : http://cmake.org/files/v2.8/cmake-2.8.12.2-win32-x86.exe
* OpenSSL 1.0.2d for windows : https://www.openssl.org/community/binaries.html
* Download QCA 2.0.1 for MinGW : http://delta.affinix.com/download/qca/2.0/qca-2.0.1-mingw.zip
  * copy qca2.dll and qcad2.dll into C:\Qt\2010.04\qt\bin
  * copy libqca2.a and libqcad2.a into C:\Qt\2010.04\qt\lib
  * copy QtCrypto directory into C:\Qt\2010.04\qt\include
* Download QCA-OSSL 2.0.0 for MinGW : http://delta.affinix.com/download/qca/2.0/plugins/qca-ossl-2.0.0-beta3.tar.bz2
  * copy qca-ossl2.dll into C:\Qt\2010.04\qt\plugins\crypto

> Making QCA works (compilation and packaging) with mingw was really painfull, that's why i provide you an archive you just have to uncompress into your qt installation directory : [qca2-qtfolder.zip](https://github.com/DarkReZuS/silenteye/blob/0.4/install/windows/qca2-qtfolder.zip)

## Get source code

Then download :

```Shell
user$ git clone --branch 0.4 https://github.com/DarkReZuS/silenteye.git
```

## Import project into Qt Creator 2

* Download QtCreator : http://download.qt.io/official_releases/qtcreator/3.5/3.5.0/qt-creator-opensource-windows-x86-3.5.0.exe

1. Configure Kit and cmake http://doc.qt.io/qtcreator/creator-targets.html
2. Open file or project => select silenteye/CMakeLists.txt
3. Choose mingw generator and set parameter to "."
4. Configure project compilation
  1. add environnement variable: ENABLE_MODULE set to 1
  2. add step (before mingw32-make). => cmake.exe with parameter "."
5. Close Qt Creator and reopen it using your last session (force files scan)

## Run

Just use the run function of Qt Creator

# Compilation options

| Option  | Description |
| ------------- | ------------- |
| ENABLE_MODULE=1  | In order to build modules for silenteye, set the following environnement variable to 1 |
| ENABLE_TESTSUITE=1 | In order to build testsuites for silenteye, set the following environnement variable to 1 |
| ENABLE_STATIC=1 | In order to build application and modules (plugins) in static mode |
| ENABLE_PACK=1 | In order to enable "make install" command (also enable CPack) |
| BINPATH_VALUE=/usr/bin | Change default lib path (install executable to /usr/bin/silenteye) |
| LIBPATH_VALUE=/usr/lib/silenteye/ | Change default lib path (use /usr/lib/silenteye/libsef.so and /usr/lib/silenteye/modules/*.so) |
| CONFPATH_VALUE=/etc/silenteye | Change default configuration path (use /etc/silenteye/silenteye.conf and /etc/silenteye/version.xml) |
| or CONFPATH_USERHOME=1 | Change default configuration path (use $HOME/.silenteye/silenteye.conf and $HOME/.silenteye/version.xml) |
| LOGPATH_VALUE=/var/log/ | Change default log path (use /var/log/silenteye.log) |
| or LOGPATH_USERHOME=1 | Change default log path (use $HOME/.silenteye/silenteye.log) | 
| CMNPATH_VALUE=/usr/share/silenteye/ | Change default share path (use /usr/share/silenteye/version.xml) |
| DOCPATH_VALUE=/usr/share/doc | Change default doc path (installation only) |
| PIXPATH_VALUE=/usr/share/pixmaps | Change default pixmaps path (installation only) |
| ENABLE_GNOMEMENU=1 | Create gnome menu entry on install (create /usr/share/applications/silenteye.desktop) |



# RELEASE HISTORY

## Version 0.4.3 - Changes since 0.4.2:

FEACTURES:
* Batch mode (process file from command line)
* SEFormatBMP/SEFormatJPEG/SEFormatWAV : Use external configuration file for default values

ENHANCEMENT:
* Improve dialog's tooltips for jpeg module
* Add "compress data" default value into preferences

BUG FIXES:
* Show module tooltip even if status is OK

## Version 0.4.2 - Changes since 0.4.1:

ENHANCEMENTS:
* add executable's options in order to change default logpath and confpath (--logpath and --confpath)

INTERNAL:
* configuration/log/lib/share paths are configurable on compilation (CMake options)
* "make install" works fine for linux distribution
* Move to Qt 4.8.2

## Version 0.4.1b (only Mac OSX) - Changes since 0.4.1:
* Encryption (AES 128/256) didn't work on Mac OS X. Package (installer) had been updated to fix libraries dependences.

## Version 0.4.1 - Changes since 0.4.0:

FEACTURES:
* Encode message into JPEG file (SEFormatJPEG plug-in)

ENHANCEMENTS:
* Enable manual edit for encoding destination
* Generate AES128 and AES256 key from MD5

BUG FIXES:
* "Open file" function doesn't handle ".tif"

INTERNAL:
* Update application and plu-ins for static build =## needed by standalone installer

## Version 0.4.0 - Changes since 0.3.1:

ENHANCEMENTS:
* Audio file supported on main application(can open WAVE/PCM file)
* "Check for new version" feacture added
* New SEFormatWAV plug-in added (encode informations into wav files)
* New installer gives possibility to associate file extensions to silenteye (Windows only)
* Add tooltip showing absolute file path for current media
* Copy action works for images
* Add format selection on Property dialog (display capacity)
* SECryptoAES256 integration into application distribution package

BUG FIXES:
* GUI update for more compatibility on KDE
* SEFormatBMP: change label to "bit(s) per pixel per color"
* Open two files with same shortname and diferent localisation
* Now when decoding process succeed, an other try on decode button works.

INTERNAL:
* SilentEyeFramework architecture is now ready for video plug-in

DEVELOPER RELATED:
* CMake now search QCA2 on Qt directories
* Move to Qt 4.7.0 or later (using last QtMultimedia)
* Move to CMake 2.8.3 (update for QtMultimedia)

## Version 0.3.1 - Changes since 0.3.0

BUG FIXES:
* "Can't use equidistribution mode for most image and message." : SEFormatBMP plug-in updated to version 1.0.1
* GUI update for more compatibility when using Gnome. (update for KDE soon...)

## Version 0.3.0 - Changes since 0.2.0

NEW FEACTURES:
* Data compression
* Capacity to hide a file
* Improvement of BMP plug-in
* Add module state into preferences dialog

REPORTED FEACTURES:
* Use of context menu in Windows explorer reported to next version

## Version 0.2.0 - Changes since 0.1.1

NEW FEACTURES:
* Plug-ins system
* Encryption support (QCA2)
* Adding logger

## Version 0.1.1 - Changes since 0.1.0

NEW FEACTURES:
* Keep image ratio on window resize event

BUGS:
many bugs fixed
