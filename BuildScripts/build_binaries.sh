#!/usr/bin/env bash

BASE="$(dirname "$(realpath "$0")")/.."
OUTDIR="$BASE/Template"
LIBDIR="$OUTDIR/lib"
HDRDIR="$OUTDIR/include"
ENGDIR="$BASE/Engine"
PROJ="$BASE/Build/NWengineCore.make"
PROJDIR="$BASE/Build/"
BINDBG="$BASE/Build/Bin/NWengineCore/Debug64"
BINRLS="$BASE/Build/Bin/NWengineCore/Release64"

mkdir -p "$OUTDIR" 2>/dev/null
mkdir -p "$LIBDIR" 2>/dev/null
mkdir -p "$HDRDIR" 2>/dev/null

echo Building Debug Binary.
make -f "$PROJ" config=debug_x64 -C "$PROJDIR"

echo Building Release Binary.
make -f "$PROJ" config=release_x64 -C "$PROJDIR"

cp "$BINDBG/libNWengineCore64d.a" "$LIBDIR/libNWengineCore64d.a"
cp "$BINRLS/libNWengineCore64.a" "$LIBDIR/libNWengineCore64.a"

mkdir -p "$HDRDIR/temp" 2>/dev/null

find "$ENGDIR" -type f | while read -r f; do
    ext="${f##*.}"
    if [[ "$ext" =~ ^(hpp|h)$ ]]; then
        cp "$f" "$HDRDIR/temp/$(basename "$f")"
    fi
done


TMPDIR="$BASE/BuildScripts/Hdrasm"
g++ "$TMPDIR/main.cpp" "$TMPDIR/HeaderAssembler.cpp" -o "$BASE/BuildScripts/hdrasm"
"$BASE/BuildScripts/hdrasm" "$HDRDIR/temp/" "$HDRDIR/" NWengineCore.h NWengine.h
echo $HDRDIR

echo ===== Build Complete =====
