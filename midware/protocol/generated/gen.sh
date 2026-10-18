#!/usr/bin/env bash
# Generate .pb.c/.pb.h using an externally installed nanopb 0.4.9.2 generator.
# Usage: ./gen.sh FILE_OR_DIRECTORY [...] [-o OUTPUT_DIR] [--clean]
# Set NANOPB_GENERATOR to generator/nanopb_generator.py from release 0.4.9.2.
# Python requires protobuf; the runtime sources are not needed in this repository.
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PYTHON="${NANOPB_PYTHON:-python3}"
OUT_DIR="${SCRIPT_DIR}"
CLEAN=0
INPUTS=()
while [[ $# -gt 0 ]]; do
    case "$1" in
        -o|--out)
            [[ $# -ge 2 ]] || { echo "Missing output directory" >&2; exit 2; }
            OUT_DIR="$2"; shift 2 ;;
        --clean) CLEAN=1; shift ;;
        -h|--help)
            echo "Usage: $0 FILE_OR_DIRECTORY [...] [-o OUTPUT_DIR] [--clean]"
            exit 0 ;;
        -*) echo "Unknown option: $1" >&2; exit 2 ;;
        *) INPUTS+=("$1"); shift ;;
    esac
done
[[ ${#INPUTS[@]} -gt 0 ]] || { echo "Specify a .proto file or directory" >&2; exit 2; }
command -v protoc >/dev/null || { echo "Install protobuf-compiler (protoc) first" >&2; exit 1; }
GENERATOR="${NANOPB_GENERATOR:-}"
if [[ -z "$GENERATOR" ]]; then
    GENERATOR="$("$PYTHON" -c 'import nanopb.generator.nanopb_generator as g; print(g.__file__)' 2>/dev/null)" || {
        echo "Set NANOPB_GENERATOR to nanopb 0.4.9.2 generator/nanopb_generator.py" >&2
        exit 1
    }
fi
VERSION="$("$PYTHON" "$GENERATOR" --version)"
[[ "$VERSION" == "nanopb-0.4.9.2" ]] || {
    echo "Expected nanopb-0.4.9.2 generator; got: $VERSION" >&2
    exit 1
}
INCLUDE_ARGS=()
PROTO_FILES=()
for input in "${INPUTS[@]}"; do
    [[ -e "$input" ]] || { echo "Input does not exist: $input" >&2; exit 1; }
    abs="$(cd "$(dirname "$input")" && pwd)/$(basename "$input")"
    if [[ -d "$abs" ]]; then
        INCLUDE_ARGS+=(-I "$(dirname "$abs")")
        while IFS= read -r -d '' file; do
            PROTO_FILES+=("$file")
        done < <(find "$abs" -name '*.proto' -print0 | sort -z)
    elif [[ "$abs" == *.proto ]]; then
        INCLUDE_ARGS+=(-I "$(dirname "$abs")")
        PROTO_FILES+=("$abs")
    else
        echo "Not a .proto input: $input" >&2; exit 2
    fi
done
[[ ${#PROTO_FILES[@]} -gt 0 ]] || { echo "No .proto files found" >&2; exit 1; }
mkdir -p "$OUT_DIR"
if [[ "$CLEAN" -eq 1 ]]; then
    find "$OUT_DIR" -type f \( -name '*.pb.c' -o -name '*.pb.h' \) -delete
fi
"$PYTHON" "$GENERATOR" \
    "${INCLUDE_ARGS[@]}" -D "$OUT_DIR" "${PROTO_FILES[@]}"
