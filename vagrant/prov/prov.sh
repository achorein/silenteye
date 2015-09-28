#!/bin/sh

# halt on error
set -e

echo
echo "================================================="
echo "Begin Provision"
echo "================================================="
echo

# add proxy configuration
#cat <<EOF >> /etc/environment
#export http_proxy=http://proxyhost:proxyport
#export https_proxy=http://proxyhost:proxyport
#export HTTP_PROXY=http://proxyhost:proxyport
#export HTTPS_PROXY=http://proxyhost:proxyport
#EOF
. /etc/environment

# add default packages
apt-get update \
  && apt-get -y upgrade \
  && apt-get -y install wget curl nano vim git

# prompt tuning
echo "PS1='\[\e[0;31m\]\u\[\e[m\] \[\e[1;34m\]\w\[\e[m\] \[\e[0;31m\]\$ \[\e[m\]\[\e[0;32m\]'" \
  >> /root/.bashrc

echo
echo "================================================="
echo "End Provision"
echo "================================================="
echo
