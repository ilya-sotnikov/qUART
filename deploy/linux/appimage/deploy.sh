#!/bin/sh

set -e

name="qUART"

deploy_dir=`pwd`

root_dir=$(pwd)/../../../

rm -rf AppDir

mkdir -p ${root_dir}/build-release
cd ${root_dir}/build-release

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -G Ninja .. &&
cmake --build . &&
cmake --install . --prefix ${deploy_dir}/AppDir

cd ${deploy_dir}

rm -f ${name}*.AppImage

if [ ! -f "linuxdeploy-x86_64.AppImage" ]; then
    wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
    chmod u+x linuxdeploy*.AppImage
fi

if [ ! -f "linuxdeploy-plugin-qt-x86_64.AppImage" ]; then
    wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
    chmod u+x linuxdeploy*.AppImage
fi

QMAKE=qmake6 ./linuxdeploy-x86_64.AppImage --appdir AppDir -e AppDir/bin/${name} -i ${root_dir}/icons/app.png -d ${deploy_dir}/${name}.desktop --plugin qt --output appimage
cp $(find . -name "${name}*.AppImage") "${name}-linux_x86-64.AppImage"
