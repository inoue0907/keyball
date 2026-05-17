#!/bin/bash
# QMK 0.22.14 build environment setup for keyball61
# Run once from WSL: bash /mnt/e/keyball/setup_qmk.sh

set -e

QMK_HOME=/mnt/e/keyball/__qmk__
KEYBALL_DIR=/mnt/e/keyball

echo "=== Installing AVR toolchain and build tools ==="
sudo apt-get update -qq
sudo apt-get install -y --no-install-recommends \
  gcc-avr binutils-avr avr-libc \
  make git python3-pip dfu-programmer dfu-util

echo "=== Installing QMK CLI ==="
python3 -m pip install --user qmk
export PATH="$HOME/.local/bin:$PATH"

echo "=== Cloning QMK firmware 0.22.14 ==="
if [ -d "$QMK_HOME" ]; then
  echo "  $QMK_HOME already exists, skipping clone"
else
  git clone --branch 0.22.14 --depth 1 \
    https://github.com/qmk/qmk_firmware "$QMK_HOME"
  cd "$QMK_HOME"
  git submodule update --init --depth 1 \
    lib/lufa lib/printf lib/unity
fi

echo "=== Installing Python requirements ==="
python3 -m pip install --user -r "$QMK_HOME/requirements.txt"

echo "=== Configuring QMK home ==="
qmk config user.qmk_home="$QMK_HOME"

echo "=== Linking keyball keyboards ==="
ln -sfn "$KEYBALL_DIR/qmk_firmware/keyboards/keyball" \
  "$QMK_HOME/keyboards/keyball"

echo ""
echo "Setup complete. To build:"
echo "  bash /mnt/e/keyball/build.sh"
