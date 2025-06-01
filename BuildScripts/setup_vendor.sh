#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
pushd "$SCRIPT_DIR" >/dev/null


mkdir -p nwtmp

mkdir -p ../Dependencies/freetype/lib/linux32
mkdir -p ../Dependencies/freetype/lib/linux64
mkdir -p ../Dependencies/freetype/include

mkdir -p ../Dependencies/glew/lib/release/linux32
mkdir -p ../Dependencies/glew/lib/release/linux64
mkdir -p ../Dependencies/glew/lib/debug/linux32
mkdir -p ../Dependencies/glew/lib/debug/linux64
mkdir -p ../Dependencies/glew/include

mkdir -p ../Dependencies/vendor/stb

cd nwtmp
if [[ ! -d "freetype" ]]; then
  git clone https://github.com/freetype/freetype.git
fi

if [[ ! -d "glew-cmake" ]]; then
  git clone https://github.com/Perlmint/glew-cmake.git
fi

if [[ ! -d "stb" ]]; then
  git clone https://github.com/nothings/stb.git
fi

#
# stb
#
cp stb/stb_image.h    ../../Dependencies/vendor/stb/stb_image.h
cp stb/stb_image_write.h  ../../Dependencies/vendor/stb/stb_image_write.h

### Build FreeType 
cd freetype

mkdir -p build64
pushd build64 >/dev/null
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DFT_BUILD_SHARED_LIBS=OFF \
  -DCMAKE_C_FLAGS="-m64" \
  -DCMAKE_INSTALL_PREFIX="$SCRIPT_DIR/../Dependencies/freetype"

make -j"$(nproc)"

# On most Linux FreeType CMake builds put libfreetype.a under ./.libs or under build tree.
if [[ -f "objs/.libs/libfreetype.a" ]]; then

  cp objs/.libs/libfreetype.a "$SCRIPT_DIR/../Dependencies/freetype/lib/linux64/freetype.a"
else
  # fallback: try top-level .a
  cp libfreetype.a "$SCRIPT_DIR/../Dependencies/freetype/lib/linux64/freetype.a"
fi

# Copy headers 
cp -r ../include/. "$SCRIPT_DIR/../Dependencies/freetype/include"

popd >/dev/null


### 6.2) Build FreeType (32-bit, Release, static) ###
mkdir -p build32
pushd build32 >/dev/null
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DFT_BUILD_SHARED_LIBS=OFF \
  -DCMAKE_C_FLAGS="-m32" \
  -DCMAKE_INSTALL_PREFIX="$SCRIPT_DIR/../Dependencies/freetype"
make -j"$(nproc)"

if [[ -f "objs/.libs/libfreetype.a" ]]; then
  cp objs/.libs/libfreetype.a "$SCRIPT_DIR/../Dependencies/freetype/lib/linux32/freetype.a"
else
  cp libfreetype.a "$SCRIPT_DIR/../Dependencies/freetype/lib/linux32/freetype.a"

fi
popd >/dev/null

cd ..

#
# Build GLEW 
#
cd glew-cmake

mkdir -p build
pushd build >/dev/null

# Release, x64 static
cmake .. \
  -DREL=ON \
  -DSTATIC=1 \
  -DGLEW_BUILD=ON \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_FLAGS="-m64"

make -j"$(nproc)" libglew_static

cp lib/libGLEW.a "$SCRIPT_DIR/../Dependencies/glew/lib/release/linux64/glew32s.a"

# Release, x86 static
make clean

cmake .. \
  -DREL=ON \
  -DSTATIC=1 \
  -DGLEW_BUILD=ON \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_FLAGS="-m32"
make -j"$(nproc)" libglew_static
cp lib/libGLEW.a "$SCRIPT_DIR/../Dependencies/glew/lib/release/linux32/glew32s.a"

# Debug, x64 static

make clean
cmake .. \
  -DREL=OFF \
  -DSTATIC=1 \
  -DGLEW_BUILD=ON \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_C_FLAGS="-m64"
make -j"$(nproc)" libglew_static
cp lib/libglewd.a "$SCRIPT_DIR/../Dependencies/glew/lib/debug/linux64/glew32s.a"

# Debug, x86 static
make clean
cmake .. \
  -DREL=OFF \
  -DSTATIC=1 \
  -DGLEW_BUILD=ON \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_C_FLAGS="-m32"
make -j"$(nproc)" libglew_static
cp lib/libglewd.a "$SCRIPT_DIR/../Dependencies/glew/lib/debug/linux32/glew32s.a"

# COpy headers 
cp -r ../include/. "$SCRIPT_DIR/../Dependencies/glew/include"

popd >/dev/null  # exit build/
cd ..           # exit glew-cmake/

#

# 8) Clean up and return
#
popd >/dev/null  # back to SCRIPT_DIR

echo "Dependencies build complete."
