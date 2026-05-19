# Binary Clock

Qt6/C++製のデスクトップアクセサリ型バイナリ時計です。

## 機能

- **2種類の表示スタイル**
  - スタイルA: BCD グリッド（列ごとに各桁をバイナリ表示）
  - スタイルB: ライン（行ごとにバイナリ表示）
- **年月日表示**（ホバー時 / 常時 / クリック切替）
- **10進数ヒント表示**（各桁・行に実際の数値を表示）
- フレームレス・半透明ウィンドウ
- 常に最前面表示（トグル可）
- ドラッグで移動、位置は自動保存
- LED色・背景色・サイズなどカスタマイズ可能

## 操作方法

| 操作 | 動作 |
|---|---|
| 左ドラッグ | ウィンドウ移動 |
| 右クリック | メニュー（スタイル切替・設定など） |
| ホバー | 日付表示（デフォルト設定時） |

## ビルド要件

- Qt 6.x
- CMake 3.16+
- Ninja

## ビルド手順

### Windows (MinGW)

1. `build.ps1` の `$qtPath` を自分の Qt インストールパスに合わせて編集
2. PowerShell で実行:

```powershell
.\build.ps1
```

3. DLL を揃えて実行:

```powershell
& "C:\Qt\6.x.x\mingw_64\bin\windeployqt.exe" "build\BinaryClock.exe"
.\build\BinaryClock.exe
```

### Linux

システムに Qt6 が入っている場合:

```sh
chmod +x build.sh
./build.sh
./build/BinaryClock
```

Qt6 をカスタムパスにインストールしている場合は `build.sh` の `QT_PATH` を設定してください。

> **Note**  
> 半透明表示にはコンポジットWMが必要です。  
> Wayland 環境では常に最前面表示が効かない場合があります。

## ファイル構成

```
src/
├── main.cpp
├── ClockSettings.h         # 設定データ構造体
├── ClockWidget.h/cpp       # メインウィンドウ
├── GridStyleWidget.h/cpp   # スタイルA: BCD グリッド
├── CircleStyleWidget.h/cpp # スタイルB: ライン
└── SettingsDialog.h/cpp    # 設定ダイアログ
resources/
└── app.ico                 # アプリアイコン
```

## ライセンス

MIT
