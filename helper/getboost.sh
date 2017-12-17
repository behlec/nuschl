#!/bin/bash

if [ -d download ]; then
  exit
fi

mkdir download
cd download
wget -O boost_1_59_0.tar.gz http://sourceforge.net/projects/boost/files/boost/1.59.0/boost_1_59_0.tar.gz/download
