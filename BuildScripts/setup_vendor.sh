#!/usr/bin/env bash
pushd "$(dirname "$(realpath "$0")")" > /dev/null || exit 1

mkdir nwtmp
mkdir -p ../Dependencies/freetype
mkdir -p ../Dependencies/freetype/lib/lnx86
mkdir -p ../Dependencies/freetype/lib/lnx64

mkdir -p ../Dependencies/glew/lib/release/lnx86
mkdir -p ../Dependencies/glew/lib/release/lnx64
mkdir -p ../Dependencies/glew/lib/debug/lnx86
mkdir -p ../Dependencies/glew/lib/debug/lnx64
mkdir -p ../Dependencies/vendor/stb

cd nwtmp || exit 1

git clone https://github.com/freetype/freetype.git
git clone https://github.com/Perlmint/glew-cmake.git
git clone https://github.com/nothings/stb.git

cp stb/stb_image.h ../../Dependencies/vendor/stb/stb_image.h
cp stb/stb_image_write.h ../../Dependencies/vendor/stb/stb_image_write.h

cd freetype 

cmake -B build_x86 -S . \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_C_FLAGS="-m32" \
      -DCMAKE_CXX_FLAGS="-m32" \
      -DCMAKE_EXE_LINKER_FLAGS="-m32" \
      -DCMAKE_SHARED_LINKER_FLAGS="-m32" \
      -DFT_DISABLE_PNG=TRUE \
      -DFT_DISABLE_BZIP2=TRUE \
      -DFT_DISABLE_BROTLI=TRUE \
      -DFT_DISABLE_ZLIB=TRUE

cmake -B build_x64 -S . \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_C_FLAGS="-m64" \
      -DCMAKE_CXX_FLAGS="-m64" \
      -DCMAKE_EXE_LINKER_FLAGS="-m64" \
      -DCMAKE_SHARED_LINKER_FLAGS="-m64" \
      -DFT_DISABLE_PNG=TRUE \
      -DFT_DISABLE_BZIP2=TRUE \
      -DFT_DISABLE_BROTLI=TRUE \
      -DFT_DISABLE_ZLIB=TRUE
cd build_x86/
echo Buliding Freetype x86
make clean
make
cp libfreetype.a ../../../../Dependencies/freetype/lib/lnx86/freetype.a
cd ../build_x64
echo Buliding Freetype x64
make clean
make
cp libfreetype.a ../../../../Dependencies/freetype/lib/lnx64/freetype.a
cd ../
cp -r include ../../../Dependencies/freetype/
cd ../
cd glew-cmake

# debug 
make clean
make debug CFLAGS.EXTRA="-m32" LDFLAGS.EXTRA="-m32"
cp lib/libGLEW.a ../../../Dependencies/glew/lib/debug/lnx86/glew32s.a
make clean
make debug CFLAGS.EXTRA="-m64 -fPIC" LDFLAGS.EXTRA="-m64"
cp lib/libGLEW.a ../../../Dependencies/glew/lib/debug/lnx64/glew32s.a
# release
make clean
make CFLAGS.EXTRA="-m32" LDFLAGS.EXTRA="-m32"
cp lib/libGLEW.a ../../../Dependencies/glew/lib/release/lnx86/glew32s.a
make clean
make CFLAGS.EXTRA="-m64 -fPIC" LDFLAGS.EXTRA="-m64"
cp lib/libGLEW.a ../../../Dependencies/glew/lib/release/lnx64/glew32s.a
cp -r include/ ../../../Dependencies/glew/

popd > /dev/null || exit 1
