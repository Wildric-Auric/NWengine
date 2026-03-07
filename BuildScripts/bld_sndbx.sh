pushd "$(dirname "$(realpath "$0")")" > /dev/null || exit 1
cd ../Build
make -f Sandbox.make config=debug_x64
mv Bin/Sandbox/Debug64/Sandbox Sandbox
popd > /dev/null || exit 1
