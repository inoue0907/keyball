# -*- coding: utf-8 -*-
"""キーマップ表を PDF にする（A4 横、1 ページ 2 レイヤー）。

    python tools/gen_keymap_pdf.py   ->  docs/keymap.pdf

docs/keymap-viewer.html を ?print 付きで headless Chrome に印刷させる。
先に gen_keymap_viewer.py を実行しておくこと。Chrome の場所は環境変数
CHROME で指定でき、無ければ PATH と Windows の既定の場所を探す。
"""
import os
import shutil
import subprocess
import sys

from keyball61_layout import DOCS, ROOT

CANDIDATES = [
    'google-chrome', 'google-chrome-stable', 'chromium', 'chromium-browser', 'chrome',
    r'C:\Program Files\Google\Chrome\Application\chrome.exe',
    r'C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe',
]


def find_chrome():
    if os.environ.get('CHROME'):
        return os.environ['CHROME']
    for c in CANDIDATES:
        path = shutil.which(c) or (c if os.path.isfile(c) else None)
        if path:
            return path
    raise SystemExit('Chrome が見つかりません。環境変数 CHROME で場所を指定してください')


def main():
    html = os.path.join(DOCS, 'keymap-viewer.html')
    if not os.path.isfile(html):
        raise SystemExit('先に python tools/gen_keymap_viewer.py を実行してください')
    out = os.path.join(DOCS, 'keymap.pdf')
    url = 'file:///' + html.replace(os.sep, '/').lstrip('/') + '?print'
    cmd = [find_chrome(), '--headless=new', '--disable-gpu', '--no-sandbox',
           '--no-pdf-header-footer', '--run-all-compositor-stages-before-draw',
           # Web フォントの読み込みと文字の縮小調整 (fit) を待つ
           '--virtual-time-budget=15000',
           '--print-to-pdf=' + out, url]
    subprocess.run(cmd, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    if not os.path.isfile(out) or os.path.getsize(out) == 0:
        raise SystemExit('PDF が書き出されませんでした')
    print('wrote %s (%d bytes)' % (os.path.relpath(out, ROOT), os.path.getsize(out)))


if __name__ == '__main__':
    sys.exit(main())
