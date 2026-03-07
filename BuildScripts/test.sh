pushd "$(dirname "$(realpath "$0")")" > /dev/null || exit 1
find Hdrasm -type f | while read -r f; do
    ext="${f##*.}"
    if [[ "$ext" =~ ^(cpp|h)$ ]]; then
        echo "file: $f | ext: $ext"
    fi
done
popd > /dev/null || exit 1
