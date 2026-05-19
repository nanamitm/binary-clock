# Qt6 binary clock build script for Windows (MinGW)

$qtPath   = "C:\Qt\6.9.2\mingw_64"
$mingwPath = "C:\Qt\Tools\mingw1310_64\bin"
$cmakePath = "C:\Qt\Tools\CMake_64\bin"
$ninjaPath = "C:\Qt\Tools\Ninja"

$env:PATH = "$mingwPath;$cmakePath;$ninjaPath;$env:PATH"

$buildDir = "build"
if (-not (Test-Path $buildDir)) { New-Item -ItemType Directory $buildDir | Out-Null }

& "$cmakePath\cmake.exe" -S . -B $buildDir `
    -G "Ninja" `
    -DCMAKE_PREFIX_PATH="$qtPath" `
    -DCMAKE_BUILD_TYPE=Release `
    -DCMAKE_C_COMPILER="$mingwPath\gcc.exe" `
    -DCMAKE_CXX_COMPILER="$mingwPath\g++.exe"

& "$cmakePath\cmake.exe" --build $buildDir
