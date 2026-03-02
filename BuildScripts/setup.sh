#!/usr/bin/env bash
pushd "$(dirname "$(realpath "$0")")" > /dev/null || exit 1
source setup_vendor.sh
cd .. || exit 1
premake5 gmake
popd > /dev/null || exit 1

