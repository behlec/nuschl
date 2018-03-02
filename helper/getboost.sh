#!/bin/bash

if [ -e download/boost_1_59_0.tar.gz ]; then
  echo 'Boost is already there'
  exit
fi

echo 'Downloading Boost'
mkdir -p download
cd download
wget -O boost_1_59_0.tar.gz https://github.com/boostorg/boost/archive/boost-1.59.0.tar.gz
