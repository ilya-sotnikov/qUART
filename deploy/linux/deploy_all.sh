#!/bin/sh

set -e

name="qUART"

cd flatpak
./deploy.sh
cp ${name}-linux*.flatpak ..
cd ..

cd appimage
./deploy.sh
cp ${name}-linux*.AppImage ..
cd ..
