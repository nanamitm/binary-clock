#!/bin/sh
# Qt6 binary clock build script for Linux
# Adjust QT_PATH to your Qt6 installation if not installed system-wide

QT_PATH=""   # e.g. "/home/user/Qt/6.9.2/gcc_64" — leave empty to use system Qt6

BUILD_DIR="build"
mkdir -p "$BUILD_DIR"

if [ -n "$QT_PATH" ]; then
    PREFIX_ARG="-DCMAKE_PREFIX_PATH=$QT_PATH"
else
    PREFIX_ARG=""
fi

cmake -S . -B "$BUILD_DIR" \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    $PREFIX_ARG

cmake --build "$BUILD_DIR"
