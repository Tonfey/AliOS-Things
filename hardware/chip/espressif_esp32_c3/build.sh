#!/usr/bin/env bash

a=`uname  -a`
b="Darwin"
c="Linux"
dst="$HOME/.espressif/dist/"

# store workdir path
workdir=$PWD
echo "workdir = ${workdir}"

# get shelldir path
shelldir=$(cd $(dirname ${BASH_SOURCE[0]}); pwd )
echo "shelldir = ${shelldir}"

cd ${shelldir}/esp-idf
./install.sh
cd ${workdir}