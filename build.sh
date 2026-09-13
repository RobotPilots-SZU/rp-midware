#!/usr/bin/env bash
set -eu

BOARD=""
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PREBUILD_DIR="$SCRIPT_DIR/prebuild"

usage() {
    echo "Usage: $0 -b BOARD"
    exit 1
}

while getopts "b:h" opt; do
    case $opt in b) BOARD="$OPTARG" ;; *) usage ;; esac
done
[ -z "$BOARD" ] && usage

# Ensure Zephyr environment
if [ -z "${ZEPHYR_BASE:-}" ]; then
    source "$(west topdir)/zephyr/zephyr-env.sh"
fi

build_zenoh() {
    local board="$1"
    local src="${HOME}/lib/zenoh-pico"
    local git_url="https://github.com/eclipse-zenoh/zenoh-pico.git"
    local build_dir="${PREBUILD_DIR}/zenoh/${board}/build"
    local out_lib="${PREBUILD_DIR}/zenoh/${board}/libzenohpico.a"
    local ver_file="${PREBUILD_DIR}/zenoh/${board}/.version"

    # Download if missing
    [ -d "$src" ] || git clone --depth 1 "$git_url" "$src"

    # Determine version
    local version
    version="$({ cat "$src/version.txt" 2>/dev/null || echo "0.0.0"; })-$(git -C "$src" rev-parse --short HEAD 2>/dev/null || echo "unknown")"

    # Skip if up-to-date
    if [ -f "$out_lib" ] && [ -f "$ver_file" ] && [ "$(cat "$ver_file")" = "$version" ]; then
        echo "  Up-to-date: $out_lib"
        return 0
    fi

    rm -rf "$build_dir"
    mkdir -p "$build_dir"

    cmake -B "$build_dir" -S "$src" \
        -DCMAKE_TOOLCHAIN_FILE="${ZEPHYR_BASE}/cmake/toolchain/zephyr/host-tools.cmake" \
        -DBOARD="$board" \
        -DZP_PLATFORM=zephyr

    cmake --build "$build_dir" --target zenohpico_static

    # Copy output
    find "$build_dir" -name 'libzenohpico.a' -exec cp {} "$out_lib" \;
    echo "$version" > "$ver_file"
    echo "  Done: $out_lib"
}

build_zenoh "$BOARD"re
