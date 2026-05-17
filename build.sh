#!/bin/bash
# Build keyball61 mona2 keymap
# Run from WSL: bash /mnt/e/keyball/build.sh

set -e

export PATH="$HOME/.local/bin:$PATH"
export QMK_HOME=/mnt/e/keyball/__qmk__

KEYBOARD=${1:-keyball61}
KEYMAP=${2:-mona2}

echo "=== Building keyball/$KEYBOARD:$KEYMAP ==="
qmk compile -j $(nproc) -kb "keyball/$KEYBOARD" -km "$KEYMAP"

echo ""
echo "=== Firmware location ==="
ls -lh "$QMK_HOME"/keyball_${KEYBOARD}_${KEYMAP}*.hex 2>/dev/null || \
ls -lh ./*.hex 2>/dev/null || \
find "$QMK_HOME" -maxdepth 1 -name "*.hex" -newer "$QMK_HOME/Makefile" 2>/dev/null
