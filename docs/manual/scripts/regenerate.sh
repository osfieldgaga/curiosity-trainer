#!/usr/bin/env bash
# Regenerates curiosity-trainer-manual.tex from the Word programming reference and the
# environment snapshot. Run from anywhere: ./docs/manual/scripts/regenerate.sh
set -euo pipefail
HERE="$(cd "$(dirname "$0")" && pwd)"
MANUAL="$(dirname "$HERE")"
DOCS="$(dirname "$MANUAL")"
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT
python3 -c "import sys,zipfile; zipfile.ZipFile(sys.argv[1]).extractall(sys.argv[2])" \
    "$DOCS/Curiosity Nano Programming Reference.docx" "$TMP/ref"
mkdir -p "$MANUAL/figures"
python3 "$HERE/docx2tex.py" "$TMP/ref" "$TMP/part2.tex" "$MANUAL/figures"
python3 "$HERE/build_manual.py" "$TMP/part2.tex" "$DOCS/dev-environment" "$TMP/ref/word/media/image4.png" "$MANUAL"
python3 "$HERE/check_tex.py" "$MANUAL/curiosity-trainer-manual.tex"
