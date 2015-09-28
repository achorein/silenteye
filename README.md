silenteye
=========

SilentEye is a cross-platform application design for an easy use of steganography, in this case hiding messages into pictures or sounds. It provides a pretty nice interface and an easy integration of new steganography algorithm and cryptography process by using a plug-ins system.
SilentEye is free to use (under GNU GPL v3).

read more : http://www.silenteye.org

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

# How can i compile source code?

## Qt

You have to install Qt from source with the following commands:

```Shell
root$ wget http://download.qt-project.org/official_releases/qt/4.8/4.8.6/qt-everywhere-opensource-src-4.8.6.tar.gz
root$ tar -zxvf qt-everywhere-opensource-src-4.8.6.tar.gz
root$ cd qt-everywhere-opensource-src-4.8.6
root$ ./configure -release -nomake examples -nomake demos -no-qt3support -no-scripttools -no-opengl -no-webkit -no-phonon -no-sql-sqlite -gtkstyle -opensource -prefix /usr/local/Qt-4.8.6-release
root$ make
root$ make install
```

or for static compilation :
```
root$ ./configure -static -nomake examples -nomake demos -no-qt3support -no-scripttools -no-opengl -no-webkit -no-phonon -no-sql-sqlite -gtkstyle -opensource -prefix /usr/local/Qt-4.8.6-release
```

## QCA2

QCA is also required for encryption support (modules) :

```Shell
root$ wget http://delta.affinix.com/download/qca/2.0/qca-2.0.3.tar.bz2
root$ bunzip2 qca-2.0.3.tar.bz2 && tar -xvf qca-2.0.3.tar
root$ cd qca-2.0.3
root$ patch src/botantools/botan/botan/secmem.h fix_build_gcc4.7.diff
root$ ./configure --qtdir=/usr/local/Qt-4.8.6-release/
root$ make
root$ make install
```

download patch : [fix_build_gcc4.7.diff](https://github.com/DarkReZuS/silenteye/blob/0.4/vagrant/vagrant_data/fix_build_gcc4.7.diff)

or for static compilation :
```
./configure --qtdir=/usr/local/Qt-4.8.6-static/ --static
```

## QCA-OSSL

next is QCA plugin for OpenSSL support :

```Shell
root$ wget http://delta.affinix.com/download/qca/2.0/plugins/qca-ossl-2.0.0-beta3.tar.bz2
root$ bunzip2 qca-ossl-2.0.0-beta3.tar.bz2 && tar -xvf qca-ossl-2.0.0-beta3.tar
root$ cd qca-ossl-2.0.0-beta3
root$ patch qca-ossl.cpp < detect_ssl2_available.diff
root$ patch qca-ossl.cpp < detect_md2_available.diff
root$ ./configure --qtdir=/usr/local/Qt-4.8.6-release/
root$ make
root$ make install
```

download patch : [detect_ssl2_available.diff](https://github.com/DarkReZuS/silenteye/blob/0.4/vagrant/vagrant_data/detect_ssl2_available.diff) and [detect_md2_available.diff](https://github.com/DarkReZuS/silenteye/blob/0.4/vagrant/vagrant_data/detect_md2_available.diff)

or for static compilation :
```
./configure --qtdir=/usr/local/Qt-4.8.6-static/ --static
```

## CMake

CMake (version > 2.8.10 to build project) :

```Shell
root$ apt-get install cmake
```

## Get source code

Then download, decompress and move into source folder :

```Shell
user$ git clone --branch 0.4 https://github.com/DarkReZuS/silenteye.git
user$ cd silenteye
```

## Let's compile

Now you can generate MakeFile like this :

```Shell
user$ export PATH=/usr/local/Qt-4.8.6-release/bin:$PATH
user$ ENABLE_MODULE=1 cmake .
```

or for static compilation :
```
user$ export PATH=$PATH:/usr/local/Qt-4.8.6-static/bin
user$ ENABLE_MODULE=1 ENABLE_STATIC=1 cmake .
```

Just run make command to build the project

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

## Compilation options

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

