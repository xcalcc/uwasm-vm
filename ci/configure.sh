#!/usr/bin/env bash
echo "Workdir : $(pwd)"
workdir=$(pwd)
set -x;
INSTALL_DIR=$(pwd)/install

if [ ! -z "$1" ]; then
    INSTALL_DIR="$1"
    if [ -d "$INSTALL_DIR" ]; then
        mkdir -p "${INSTALL_DIR}"
    fi
fi

if [ -d "$INSTALL_DIR" ]; then
    mkdir -p "${INSTALL_DIR}"
fi

cmake ${CONFIGURE_SRC_DIR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_MODE:-Debug} "-DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}"