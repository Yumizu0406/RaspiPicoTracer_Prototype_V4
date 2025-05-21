# RaspiPicoTracer_Prototype_V4
RaspiPicoTracerは白線の上をたどって走るロボットであり、
機能をできる限り少なくし、回路、メカ、組み立て、プログラミングなどロボット作成に必要な経験を一通り短期間で行えるように開発中です。
このロボットはマイクロマウス大会のロボトレース競技に参加することを想定して作成しています。
競技の詳細はロボトレース競技規定をご確認ください。

https://www.ntf.or.jp/?page_id=68

![RaspiPicoTracer_photo](https://github.com/Yumizu0406/RaspiPicoTracer_Prototype_V4/blob/main/document/figure/photo.png)

<br>

# スペック
| 項目 | 詳細 |
| ---- | ---- |
| サイズ | 幅:150mm  長さ:148mm  高さ:46mm |
| 重さ | 約220g |
| 電源 | 単四電池4本 |
| モーター | FA-130 |
| モータードライバ | TB6612 |
| CPU | RP2350 |
| ラインセンサ | LBR-127HLD×4 |
| ホイール | 径:36mm 幅:16mm |

# 開発環境について
VSCodeでRaspberry Pi PicoのExtensionを使用します。
Raspberry Pi財団が公開しているドキュメント「Getting started with Raspberry Pi Pico-series」の内容に沿って行ってください。

cloneした後はImport Projectを行ってください。

# 回路図
![CircuitDiagram](https://github.com/Yumizu0406/RaspiPicoTracer_Prototype_V4/blob/main/document/circuit/RaspiPicoTracer_Prototype_V4.png)

# 部品表

| 名称 | メーカー | 単価 | 個数 | 備考 |
| ---- | ---- | ---- | ---- | ---- |
| メイン基板 | - | - | 1 | - |
| センサ基板 | - | - | 1 | - |
| Raspberry Pi Pico 2 | ラズベリーパイ財団 | 1000 | 1 | Raspberry Pi Picoでも可 |
| ダブルギヤボックス（左右独立４速タイプ） | 株式会社タミヤ | 924 | 1 | ギヤ比12.7:1で利用 |
| トラックタイヤセット （36mm径） | 株式会社タミヤ | 396 | 1 | - |
| 電池ボックス 単3×4本 リード線・フタ・スイッチ付 | COMFORTABLE ELECTRONIC CO., LTD(COMF) | 160 | 1 | - |
| 耐熱電子ワイヤー 黒色 20m 導体外径0.54mm(AWG24相当) | 住友電気工業株式会社 | 600 | 1 | モータードライバとモーター接続用<br>14cmのものを4つ利用 |
| TB6612モータードライバー ブレークアウトボードキット | 秋月電子 | 380 | 1 | 1 |
| LED(LC303PBL1-30Q) | - | - | 4 | 高輝度で3mm径のものならなんでもOK |
| 抵抗器 3.3kΩ | - | - | 4 | LED用 |
| ピンヘッダー 1×40 (40P) | Useconn Electronics Ltd. | 35 | 2 | - |
| 分割ロングピンソケット 1×42 (42P) | Useconn Electronics Ltd. | 80 | 2 | - |
| タクトスイッチ(DTS-63-N-V-BLK) | Cosland Co,. Ltd. | 15 | 2 | - |
| フォトリフレクター(反射型フォトセンサー) LBR-127HLD | Letex Technology Corp. | 75 | 6 | - |
| 半固定ボリューム TSR-3323S 10kΩ | SUNTAN TECHNOLOGY CO LTD | 40 | 6 | - |
| 抵抗器 68Ω | - | - | 6 | フォトリフレクター用 |

<br>

# 組み立て方法

準備として以下の工具を揃えてください。
| 名称 | 備考 |
| ---- | ---- |
| はんだごて | 15w～30wのものがよい |
| こて先クリーナー | - |
| はんだ | スズ60％ 鉛入りはんだ Φ0.8mmだとよい |
| 精密作業用ニッパー | - |
| ＋ドライバー | サイズは2番 |
| ワイヤストリッパー | あると嬉しい<br>なくてもOK |

<br>

モーターとの接続について以下の画像のように接続してください。
![ConnectToMotor](https://github.com/Yumizu0406/RaspiPicoTracer_Prototype_V4/blob/main/document/figure/PXL_20250506_041316845.png)
