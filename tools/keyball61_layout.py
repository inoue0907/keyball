# -*- coding: utf-8 -*-
"""keyball61 の物理配置と my-keymap を読むための共通モジュール。

キー位置番号 0-63 は LAYOUT_universal (= LAYOUT_no_ball) の引数の順で、
qmk_firmware/keyboards/keyball/keyball61/info.json の並びと一致させている。
"""
import io
import json
import os
import re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
KB_DIR = os.path.join(ROOT, 'qmk_firmware', 'keyboards', 'keyball', 'keyball61')
KEYMAP = os.path.join(KB_DIR, 'keymaps', 'my-keymap', 'keymap.c')
INFO = os.path.join(KB_DIR, 'info.json')
DOCS = os.path.join(ROOT, 'docs')

# 右手を 1u 右へずらして、左右の間に隙間を作る（info.json は隙間なしで詰めている）
SPLIT_X = 7
SPLIT_GAP = 1


def load_geo(path=INFO):
    keys = json.load(io.open(path, encoding='utf-8'))['layouts']['LAYOUT_no_ball']['layout']
    return [(k['x'] + (SPLIT_GAP if k['x'] >= SPLIT_X else 0), k['y']) for k in keys]


GEO = load_geo()
COLS = max(x for x, _ in GEO) + 1
ROWS = max(y for _, y in GEO) + 1

LAYER_NAMES = {
    'L_BASE':  u'ベース',
    'L_SYM':   u'記号・Fキー',
    'L_NUM':   u'テンキー・マウス',
    'L_SET':   u'スクロール・ナビ',
    'L_MOUSE': u'オートマウス',
}


def _strip_comments(src):
    src = re.sub(r'/\*.*?\*/', '', src, flags=re.S)
    return re.sub(r'//[^\n]*', '', src)


def split_args(body):
    """トップレベルのカンマで分割する（S(KC_A) のような入れ子を保つ）。"""
    out, depth, cur = [], 0, []
    for ch in body:
        if ch == '(':
            depth += 1
        elif ch == ')':
            depth -= 1
        if ch == ',' and depth == 0:
            out.append(''.join(cur))
            cur = []
        else:
            cur.append(ch)
    out.append(''.join(cur))
    return [' '.join(t.split()).replace(' ', '') for t in out if t.strip()]


def _call_body(src, start):
    """src[start] が '(' のとき、対応する ')' までの中身を返す。"""
    depth = 0
    for i in range(start, len(src)):
        if src[i] == '(':
            depth += 1
        elif src[i] == ')':
            depth -= 1
            if depth == 0:
                return src[start + 1:i]
    raise SystemExit('括弧が閉じていません')


def layer_order(src):
    m = re.search(r'enum\s+my_layers\s*\{(.*?)\}', src, re.S)
    names = [re.sub(r'=.*', '', n).strip() for n in m.group(1).split(',')]
    return [n for n in names if n]


def parse_layers(path=KEYMAP):
    """keymap.c から [{'id', 'node', 'ja', 'raw': [64 個のキーコード]}] を返す。"""
    src = _strip_comments(io.open(path, encoding='utf-8').read())
    order = layer_order(src)
    layers = []
    for m in re.finditer(r'\[(\w+)\]\s*=\s*LAYOUT_universal\s*\(', src):
        node = m.group(1)
        toks = split_args(_call_body(src, m.end() - 1))
        if len(toks) != len(GEO):
            raise SystemExit('%s: キーが %d 個（%d 個のはず）' % (node, len(toks), len(GEO)))
        if node not in LAYER_NAMES:
            raise SystemExit('%s: LAYER_NAMES に名前を追加してください' % node)
        layers.append({'id': order.index(node), 'node': node,
                       'ja': LAYER_NAMES[node], 'raw': toks})
    layers.sort(key=lambda L: L['id'])
    return layers


def parse_combos(path=KEYMAP):
    """COMBO(name, kc) と name[] = {...} から [{'keys': [...], 'out': kc}] を返す。"""
    src = _strip_comments(io.open(path, encoding='utf-8').read())
    arrays = {}
    for m in re.finditer(r'(\w+)\s*\[\s*\]\s*=\s*\{([^}]*)\}', src):
        toks = split_args(m.group(2))
        if toks and toks[-1] == 'COMBO_END':
            arrays[m.group(1)] = toks[:-1]
    combos = []
    for m in re.finditer(r'\bCOMBO\s*\(', src):
        args = split_args(_call_body(src, m.end() - 1))
        if len(args) == 2 and args[0] in arrays:
            combos.append({'keys': arrays[args[0]], 'out': args[1]})
    return combos


# ---------------------------------------------------------------- 表示ラベル
# OS 側は US 配列として読む（記号レイヤーの KC_EQL が "=" だった履歴から）。
# "\n" は意図した改行。キー内では自動折り返ししない。
KC = {
    'MINS': '-', 'EQL': '=', 'LBRC': '[', 'RBRC': ']', 'BSLS': '\\',
    'NUHS': '#', 'SCLN': ';', 'QUOT': "'", 'GRV': '`', 'COMM': ',',
    'DOT': '.', 'SLSH': '/', 'INT1': u'ろ', 'INT3': u'¥',
    'BSPC': 'BS', 'DEL': 'Del', 'ESC': 'Esc', 'TAB': 'Tab', 'ENT': 'Enter',
    'SPC': 'Space', 'LNG1': u'かな', 'LNG2': u'英数',
    'LSFT': 'Shift', 'RSFT': 'Shift', 'LCTL': 'Ctrl', 'RCTL': 'Ctrl',
    'LGUI': 'Win', 'RGUI': 'Win', 'LALT': 'Alt', 'RALT': 'Alt',
    'HOME': 'Home', 'END': 'End', 'PGUP': 'PgUp', 'PGDN': 'PgDn',
    'LEFT': u'←', 'RGHT': u'→', 'UP': u'↑', 'DOWN': u'↓',
    'PSCR': 'PrtSc',
}
SHIFTED = {
    '1': '!', '2': '@', '3': '#', '4': '$', '5': '%', '6': '^', '7': '&',
    '8': '*', '9': '(', '0': ')', 'MINS': '_', 'EQL': '+', 'LBRC': '{',
    'RBRC': '}', 'BSLS': '|', 'NUHS': '~', 'SCLN': ':', 'QUOT': '"',
    'GRV': '~', 'COMM': '<', 'DOT': '>', 'SLSH': '?', 'INT1': '_', 'INT3': '|',
}
MODF = {'S': 'Shift', 'C': 'Ctrl', 'A': 'Alt', 'G': 'Win',
        'LSFT': 'Shift', 'LCTL': 'Ctrl', 'LALT': 'Alt', 'LGUI': 'Win',
        'RSFT': 'Shift', 'RCTL': 'Ctrl', 'RALT': 'Alt', 'RGUI': 'Win'}
MODS = ('LSFT', 'RSFT', 'LCTL', 'RCTL', 'LGUI', 'RGUI', 'LALT', 'RALT')
MB = {'BTN1': u'左\nクリック', 'BTN2': u'右\nクリック', 'BTN3': u'中\nクリック',
      'BTN4': u'戻る', 'BTN5': u'進む'}
WRAP = {'WR_DQUO': '"', 'WR_QUOT': "'", 'WR_GRV': '`',
        'WR_PRN': '(', 'WR_CBR': '{', 'WR_BRC': '['}


def modsplit(code):
    """S(C(KC_TAB)) -> (['Shift', 'Ctrl'], 'KC_TAB')"""
    mods = []
    while True:
        m = re.match(r'^(S|C|A|G)\((.*)\)$', code)
        if not m:
            break
        mods.append(MODF[m.group(1)])
        code = m.group(2)
    return mods, code


def keycode(code):
    k = code[3:] if code.startswith('KC_') else code
    if k in KC:
        return KC[k]
    if re.match(r'^(F\d+|[A-Z0-9])$', k):
        return k
    return k.replace('_', ' ').title()


def label(code):
    """キーコード 1 個 -> (main, sub, category)。main は "\\n" を含みうる。

    sub は「長押し側」や修飾キーを載せる小さい上段。
    category は色分け用: base / mod / layer / mouse / macro / system / trans
    """
    if code in ('_______', 'KC_TRNS', 'KC_TRANSPARENT'):
        return (u'▽', '', 'trans')
    if code in ('XXXXXXX', 'KC_NO'):
        return (u'✕', '', 'trans')
    m = re.match(r'^LT\((\w+),(.*)\)$', code)
    if m:
        main, sub, _ = label(m.group(2))
        return (main, 'L' + m.group(1), 'layer')
    m = re.match(r'^(\w+)_T\((.*)\)$', code)
    if m:
        return (label(m.group(2))[0], MODF.get(m.group(1), m.group(1)), 'mod')
    m = re.match(r'^(MO|TG|TO|TT|DF)\((\w+)\)$', code)
    if m:
        how = {'MO': u'長押し', 'TG': u'トグル', 'TO': u'固定',
               'TT': u'タップ切替', 'DF': u'既定'}[m.group(1)]
        return ('L' + m.group(2), how, 'layer')
    if code in WRAP:
        return (WRAP[code], u'長押しで囲む', 'macro')
    if code == 'TD(TD_PAIR)':
        return (u'()\n[] {}', u'長押し Shift', 'macro')
    if code == 'SEL_LINE':
        return (u'行選択', '', 'macro')
    if code == 'QK_BOOT':
        return (u'ブート\nローダ', '', 'system')
    if code == 'BOOT_HLD':
        return (u'ブート\nローダ', u'1秒長押し', 'system')
    if code in ('QK_RBT', 'KBC_RST', 'EE_CLR', 'KBC_SAVE'):
        return (code, '', 'system')

    mods, base = modsplit(code)
    k = base[3:] if base.startswith('KC_') else base
    if k in MB:
        return (MB[k], '', 'mouse')
    if mods == ['Shift'] and k in SHIFTED:
        return (SHIFTED[k], '', 'base')
    if mods:
        order = ['Ctrl', 'Shift', 'Alt', 'Win']
        return (keycode(base), '+'.join(sorted(mods, key=order.index)), 'mod')
    if k in MODS:
        return (keycode(base), '', 'mod')
    if base.startswith('KC_'):
        return (keycode(base), '', 'base')
    # 未知のキーコード: そのまま出す（気づけるように）
    return (code, '', 'macro')


def write(path, text):
    io.open(path, 'w', encoding='utf-8', newline='\n').write(text)
    print('wrote %s (%d bytes)' % (os.path.relpath(path, ROOT), len(text)))
