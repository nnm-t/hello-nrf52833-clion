## Nordic Semiconductor nRF52833 DKをCLionで開発する検証

C++対応

### 必要なもの

- [nRF52833 DK](https://www.nordicsemi.com/Products/Development-hardware/nRF52833-DK)
  - [nRF52840 DK](https://www.nordicsemi.com/Products/Development-hardware/nRF52840-DK) や [nRF52 DK](https://www.nordicsemi.com/Products/Development-hardware/nRF52-DK) でもファイルを編集すれば通るでしょう
  - [Digi-Key](https://www.digikey.jp/ja/product-highlight/n/nordic-semi/nrf52-bluetooth-5) や [Mouser](https://www.mouser.jp/new/nordic-semiconductor/nrf52-series-soc/) から手配できます
- [JetBrains CLion](https://www.jetbrains.com/ja-jp/clion/)
  - またはCMakeが実行できる開発環境

### 環境変数

- `nRF5_SDK`: [nRF5 SDK](https://www.nordicsemi.com/Products/Development-software/nrf5-sdk) の場所を指定
  - CLionのCMakeプロファイルで設定しておく

#### Path

- 下記ツールのインストール先のパスを通す
  - [nRF Command Line Tools](https://www.nordicsemi.com/Products/Development-tools/nrf-command-line-tools/download)
  - [GNU Arm Embedded Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)