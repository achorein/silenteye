#!/bin/sh

#
# Script de provisionnement de la box centos 7
#
# Installée avec les packets suivants :
# - docker 1.7.1
# - vim wget curl
#

# termine directement le script si une erreur survient
set -e

echo
echo "================================================="
echo "Provisionnement de la machine Vagrant"
echo "================================================="
echo

# configuration du proxy
cat <<EOF >> /etc/environment
export http_proxy=http://ntes.proxy.corp.sopra:8080
export https_proxy=http://ntes.proxy.corp.sopra:8080
export HTTP_PROXY=http://ntes.proxy.corp.sopra:8080
export HTTPS_PROXY=http://ntes.proxy.corp.sopra:8080
EOF
. /etc/environment

# installation des différents packets
apt-get update \
  && apt-get -y upgrade \
  && apt-get -y install wget curl nano vim git

# tuning du prompt...
echo "PS1='\[\e[0;31m\]\u\[\e[m\] \[\e[1;34m\]\w\[\e[m\] \[\e[0;31m\]\$ \[\e[m\]\[\e[0;32m\]'" \
  >> /root/.bashrc

echo
echo "================================================="
echo "Fin du provisionnement de la machine !"
echo "================================================="
echo
