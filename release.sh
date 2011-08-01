#!/bin/sh

if [ -z "$1" ]; then
  echo "Usage: $0 <version>"
  exit 1
fi

version=$1

echo "Cleaning temporary files..."
scons -c
rm -f .sconsign.dblite 
rm -f install/installer.pyc

echo "Creating release directory..."
mkdir -p releases/CGIplus-$version/

echo "Copying files for release..."
files="install
  include
  COPYING
  src
  SConstruct
  test
  README.md
  conf"  
cp -r $files releases/CGIplus-$version/

echo "Copying files for release..."
mkdir -p releases/CGIplus-$version/doc
files="doc/features.txt
  doc/examples.txt"
cp $files releases/CGIplus-$version/doc

echo "Releasing..."
cd releases
tar -czf CGIplus-$version.tar.gz CGIplus-$version
rm -fr CGIplus-$version
cd ..

echo "Release done! File generated at releases/CGIplus-$version"
