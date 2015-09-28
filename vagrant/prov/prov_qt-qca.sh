. /etc/environment

apt-get install -y g++ \
	libfontconfig1-dev \
	libfreetype6-dev \
	libx11-dev \
	libxcursor-dev \
	libxext-dev \
	libxfixes-dev \
	libxft-dev \
	libxi-dev \
	libxrandr-dev \
	libxrender-dev

QT_PREFIX=/usr/local/Qt-4.8.6-release/

# Download and Compile Qt
wget -nv -t 1 http://download.qt-project.org/official_releases/qt/4.8/4.8.6/qt-everywhere-opensource-src-4.8.6.tar.gz
tar -zxf qt-everywhere-opensource-src-4.8.6.tar.gz
cd qt-everywhere-opensource-src-4.8.6
echo "yes" | ./configure -release -nomake examples -nomake demos -no-qt3support -no-scripttools -no-opengl -no-webkit -no-phonon -no-sql-sqlite -gtkstyle -opensource -prefix $QT_PREFIX
make && make install


# Download and Compile QCA2
wget -nv -t 1 http://delta.affinix.com/download/qca/2.0/qca-2.0.3.tar.bz2
bunzip2 qca-2.0.3.tar.bz2 && tar -xvf qca-2.0.3.tar
cd qca-2.0.3
patch src/botantools/botan/botan/secmem.h /vagrant_data/fix_build_gcc4.7.diff
./configure --qtdir=$QT_PREFIX
make && make install

# Download and Compile QCA-OSSL
wget -nv -t 1 http://delta.affinix.com/download/qca/2.0/plugins/qca-ossl-2.0.0-beta3.tar.bz2
bunzip2 qca-ossl-2.0.0-beta3.tar.bz2 && tar -xvf qca-ossl-2.0.0-beta3.tar
cd qca-ossl-2.0.0-beta3
patch qca-ossl.cpp < /vagrant_data/detect_ssl2_available.diff
patch qca-ossl.cpp < /vagrant_data/detect_md2_available.diff
./configure --qtdir=$QT_PREFIX
make && make install
