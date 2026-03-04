pushd "$(dirname "$(realpath "$0")")" > /dev/null || exit 1
./bld_sndbx.sh
./run_sndbx.sh
popd > /dev/null || exit 1
