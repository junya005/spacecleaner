# YamanekoEngine — OpenGL ゲーム開発テンプレート

SDL2 と OpenGL を用いた C++ ゲーム開発の出発点となるテンプレートリポジトリです。
CMake および vcpkg による依存関係管理が構成済みで、クローン後の手順に従うだけで開発を開始できます。

---

## 前提条件

| ツール | 備考 |
|---|---|
| [Visual Studio 2022](https://visualstudio.microsoft.com/ja/vs/) | **C++ によるデスクトップ開発** ワークロードが必要 |
| [Git](https://git-scm.com/) | vcpkg・サードパーティライブラリのクローンに使用 |
| CMake 3.15 以上 | Visual Studio 2022 に同梱されているものでも可 |

---

## セットアップ手順

このエンジンは、テンプレートとして動作します。<br>
リポジトリページの右上の [Use this template] ボタンをクリックし、[Create a new repository] をクリックしてください。<br>
それから、リポジトリ名を入力してこのテンプレートをベースにしたリポジトリを作成してください。

以下に、クローンしてローカルでセットアップする手順を示します。

> [!IMPORTANT]
> `vcpkg/` および `third_party/` は `.gitignore` により除外されています。
> クローン後、以下の手順をすべて実施してください。

### 1. リポジトリのクローン

```bat
git clone https://github.com/YOUR_USERNAME/YOUR_REPOSITORYNAME.git
cd YOUR_REPOSITORYNAME
```

### 2. vcpkg のセットアップ

リポジトリルートに vcpkg をクローンし、ブートストラップスクリプトで初期化します。

```bat
git clone https://github.com/microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat
```

### 3. 依存ライブラリのインストール

`vcpkg.json` に記載された依存パッケージをインストールします。

```bat
.\vcpkg\vcpkg install
```

<details>
<summary>手動で個別インストールする場合</summary>

```bat
.\vcpkg\vcpkg install sdl2 sdl2-image sdl2-ttf glad imgui assimp opengl
```

</details>

### 4. サードパーティライブラリのセットアップ

ヘッダーオンリーのトゥイーンライブラリ [tweeny](https://github.com/mobius3/tweeny) をクローンします。

```bat
mkdir third_party
cd third_party
git clone https://github.com/mobius3/tweeny.git
cd ..
```

### 5. Visual Studio 2022 でビルド・実行

1. Visual Studio 2022 を起動する
2. **「ローカルフォルダーを開く」** でリポジトリルートフォルダーを選択する
3. リポジトリ内の `CMakeSettings.json` が自動で読み込まれ、**Windows x64 (vcpkg)** 構成が使用される
4. CMake キャッシュの生成完了後、スタートアップアイテムとして **`YamanekoEngine.exe`** を選択する
5. **F5** キーを押してビルドと実行を行う

> [!NOTE]
> ビルド時に `assets/` フォルダーが実行ファイルと同じディレクトリへ自動コピーされます。

---

## ディレクトリ構成

```
yamaneko_engine/
├── engine/             # エンジンコア（Window, Renderer, Input など）
├── src/
│   └── main.cpp        # エントリーポイント（ここからゲームロジックを実装）
├── assets/             # ゲームアセット（画像・フォントなど）
├── docs/               # ドキュメント
├── third_party/        # サードパーティライブラリ（要セットアップ、gitignore 対象）
├── vcpkg/              # パッケージマネージャー（要セットアップ、gitignore 対象）
├── CMakeLists.txt      # ビルド定義
├── CMakeSettings.json  # Visual Studio 向け CMake プリセット
└── vcpkg.json          # 依存パッケージ定義
```

---

## 新しいプロジェクトへのカスタマイズ

`src/main.cpp` の `Engine::Init()` の引数を変更すると、プロダクト名とウィンドウサイズを設定できます。

```cpp
// ウィンドウタイトル、横幅、縦幅の指定
Engine::Init("My Product", 1280, 720);
```

---

## 使用ライブラリ

| ライブラリ | 用途 |
|---|---|
| [SDL2](https://www.libsdl.org/) | ウィンドウ作成・イベント処理 |
| [SDL2_image](https://github.com/libsdl-org/SDL_image) | 画像読み込み |
| [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf) | フォント・テキスト描画 |
| [glad](https://glad.dav1d.de/) | OpenGL 関数ローダー |
| [Dear ImGui](https://github.com/ocornut/imgui) | デバッグ・ツール用 GUI |
| [Assimp](https://github.com/assimp/assimp) | 3D モデル読み込み |
| [tweeny](https://github.com/mobius3/tweeny) | トゥイーンアニメーション |

---

## ライセンス

[LICENSE](./LICENSE) を参照してください。
