#!/bin/sh

name="qUART"

deploy_dir=`pwd`

mkdir -p ${name}

mkdir -p ../../build-release
cd ../../build-release
cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .

cp ${name}.exe ${deploy_dir}/${name}
cd ${deploy_dir}/${name}
windeployqt-qt6 ${name}.exe
ldd ${name}.exe | grep -Eiv 'system32|winsxs' | awk '{print $3}' | xargs cp -u -t .
if [ -f "${name}.zip" ]; then
    7z u ${deploy_dir}/${name}-windows_x86-64.zip *
else
    7z a ${deploy_dir}/${name}-windows_x86-64.zip *
fi

cd ${deploy_dir}
