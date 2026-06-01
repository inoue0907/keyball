@echo off
echo === [1/2] APT パッケージインストール (root) ===
wsl -u root apt-get install -y --no-install-recommends ^
  gcc-avr binutils-avr avr-libc make git python3-pip

echo.
echo === [2/2] QMK CLI・ファームウェアセットアップ ===
wsl bash /mnt/e/keyball/setup_qmk.sh
pause
