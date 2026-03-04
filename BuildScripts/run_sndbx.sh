pushd "$(dirname "$(realpath "$0")")" > /dev/null || exit 1
../Build/Sandbox
popd > /dev/null || exit 1

#No standard terminal in linux so we
#Keep executing like this?
