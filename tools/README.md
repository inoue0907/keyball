# キーマップ早見表の生成

`my-keymap/keymap.c` から、GitHub Pages に置く早見表と PDF を作る。

```sh
python tools/gen_keymap_viewer.py   # docs/keymap-viewer.html
python tools/gen_keymap_pdf.py      # docs/keymap.pdf（Chrome / Edge のヘッドレス印刷）
```

- main に push すると `.github/workflows/keymap-docs.yml` が作り直してコミットし、Pages に出す。
- レイヤーを足したら `tools/keyball61_layout.py` の `LAYER_NAMES` に日本語名を足す。
- キーの表示名は同じファイルの `label()` で決めている（OS 側は US 配列として読む）。
- 早見表で `?print` を付けると、PDF と同じ全レイヤーの並びで表示される。
