# halt on error
set -e

. /etc/environment

apt-get install -y cmake

cd /vagrant_data

git clone --branch 0.4 https://github.com/achorein/silenteye.git
cd silenteye

export PATH=/usr/local/Qt-4.8.7-release/bin:$PATH
ENABLE_MODULE=1 cmake .

make
