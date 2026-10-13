# -*- coding: utf-8 -*-
"""レイヤーを切り替えて見るキーマップ表を書き出す。

    python tools/gen_keymap_viewer.py   ->  docs/keymap-viewer.html

keymap.template.html の __DATA__ に、全レイヤーの割り当てを
JSON で埋め込むだけ。単体の HTML なのでブラウザで直接開ける。
"""
import io
import json
import os

from keyball61_layout import DOCS, GEO, label, parse_combos, parse_layers, write

TEMPLATE = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                        'keymap.template.html')


def main():
    parsed = parse_layers()
    layers = [{
        'id': L['id'],
        'ja': L['ja'],
        'keys': [dict(zip(('main', 'sub', 'cat'), label(b)), raw=b)
                 for b in L['raw']],
    } for L in parsed]

    base = parsed[0]['raw']
    combos = []
    for c in parse_combos():
        missing = [k for k in c['keys'] if k not in base]
        if missing:
            raise SystemExit('コンボのキー %s がベースレイヤーにありません' % missing)
        combos.append({'pos': [base.index(k) for k in c['keys']],
                       'keys': [label(k)[0] for k in c['keys']],
                       'out': label(c['out'])[0]})

    data = {'layers': layers, 'combos': combos,
            'cols': max(x for x, _ in GEO) + 1, 'rows': max(y for _, y in GEO) + 1,
            'geo': [{'x': x, 'y': y} for x, y in GEO]}
    payload = json.dumps(data, ensure_ascii=False, separators=(',', ':'))
    if '</script' in payload:
        raise SystemExit('データに </script が含まれていて埋め込めません')

    tpl = io.open(TEMPLATE, encoding='utf-8').read()
    if '__DATA__' not in tpl:
        raise SystemExit('%s に __DATA__ がありません' % TEMPLATE)

    if not os.path.isdir(DOCS):
        os.makedirs(DOCS)
    write(os.path.join(DOCS, 'keymap-viewer.html'), tpl.replace('__DATA__', payload))
    print('%d レイヤー x %d キー, コンボ %d' % (len(layers), len(GEO), len(combos)))


if __name__ == '__main__':
    main()
