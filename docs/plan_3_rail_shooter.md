# 第3作目 開発計画書：3D レールシューター

## 1. 目的

ゲームエンジン内製化サイクルの第3作目として、OpenGL による 3D 描画を用いたレールシューターを制作する。
完成後に、3D 描画機能を `engine/` へ組み込む。

- 開発期間：20 日
- 管理方法：日割りではなく、総時間とマイルストーンで管理する（日ごとの作業時間にばらつきがあるため）
- 役割分担：コードはユーザーが作成する。ドキュメント生成と設計提案は AI が担当する。

### 現状のエンジンの前提

> [!NOTE]
> `docs/engine_architecture_report_2.md` の `Application` クラス、`GameApp`、`ScreenBase` などは、本テンプレートでは**削除済み**である。
> 削除の意図：AI 生成によるリファクタリングの結果、コードを追えなくなってしまったため、ゲームコードの記述機構を削除した。
> 詳細は report_2 の「6. 第3作目での変更点」を参照。本サイクルでは、次の現状コードを前提とする。

- `engine/Application.cpp` は自由関数群（`Init` / `ProcessEvents` / `Clear` / `BeginImGui` / `EndImGui` / `Present` / `Shutdown`）である。`Engine::Application` クラスと `GameApp` は存在しない。
- `src/main.cpp` が `while (Engine::ProcessEvents())` のループを自前で記述する。
- `Scene` / `SceneManager` は存在するが、`Application.cpp` からは駆動されない。`main` から `Update` / `Render` を呼ぶ。
- OpenGL は 3.2 Core、ImGui の GLSL は `#version 150` である。
- 深度バッファのサイズ指定（`SDL_GL_DEPTH_SIZE`）と `glEnable(GL_DEPTH_TEST)` は未設定である。

---

## 2. 確定した方針

| 項目 | 決定内容 |
|---|---|
| ジャンル | レールシューター（奥へ自動前進、自機は平面移動、カメラは固定寄り） |
| 3D 描画範囲 | シェーダー管理、メッシュ、Assimp でのモデル読込、Phong 風ライティング 1 灯、深度テスト、カメラ |
| 対象外 | シャドウマッピング、ポストエフェクト、インスタンス描画、スカイボックス |
| エンジン組込 | 先に `src/` へ実装し、後半に `engine/` へ抽出する |
| 素材 | MagicaVoxel で自作（OBJ 書き出し、パレットテクスチャ付き） |
| 数学ライブラリ | GLM（vcpkg で導入） |
| ゲーム規模 | 1 ステージ完結（自機、雑魚 2〜3 種、ボス 1 体、HP/スコア UI、タイトル/リザルト） |
| 当たり判定 | 球（Sphere）のみ |
| シェーダー管理 | `.vert`/`.frag` をアセットとして管理し、コンパイルエラーを表示する |
| オブジェクト設計 | `GameObject` 基底クラスを `Scene` が `vector` で保持。弾はオブジェクトプール |
| 素材制作 | 並行制作。初期は立方体の仮モデルで進める |
| ループ構造 | 現状の関数型ループのまま、`Scene` と `SceneManager` を `main` から使う。`Application` クラス化はスコープ外 |
| GLSL バージョン | `#version 150 core`（OpenGL 3.2 Core に対応） |

---

## 3. 切り捨て優先度

遅れた場合は、下の番号が大きいものから削る。

| 優先 | 内容 |
|---|---|
| ① | 3D 基礎、モデル読込、カメラ、ライト |
| ② | 自機、弾、雑魚 1 種、当たり判定 |
| ③ | エンジン抽出 |
| ④ | ボス |
| ⑤ | 雑魚の追加 |
| ⑥ | UI、タイトル、リザルトの作り込み |
| ⑦ | ホットリロードなどの追加機能 |

> [!IMPORTANT]
> 本サイクルの目的は「エンジンへの 3D 描画組込」である。
> ボスや雑魚を削ってでも、③のエンジン抽出は確保する。

---

## 4. マイルストーン

日数は目安である。完了条件を満たしたら次へ進む。

| ID | 名称 | 目安 | 完了条件 |
|---|---|---|---|
| M1 | 3D 基礎 | 1〜4 日目 | 立方体が回転して表示される。`SDL_GL_DEPTH_SIZE` 24 の設定と深度テスト、カメラ、シェーダー読込（`#version 150 core`）が動く。GLM を導入済み |
| M2 | モデルとライティング | 4〜6 日目 | MagicaVoxel の OBJ を Assimp で読み込んで表示できる。Phong 風ライト 1 灯が効く |
| M3 | 自機と弾 | 6〜9 日目 | 自機が平面移動できる。弾の発射と弾プールが動く。カメラが奥へ自動前進する |
| M4 | 敵と当たり判定 | 9〜12 日目 | 雑魚 1 種が出現する。球同士の判定で弾が命中し、敵が倒れ、自機がダメージを受ける |
| M5 | エンジン抽出 | 13〜18 日目 | Shader / Mesh / Model / Camera を `engine/` へ移し、ゲーム側がそれを使うだけになる。ビルドが通る |
| M6 | 仕上げと文書 | 19〜20 日目 | `engine_architecture_report_3.md` が完成する。バッファとして余裕を残す |

M3 の前に旧 `Engine::Renderer`（`SDL_Renderer` 前提の 2D 用）の扱いを決める。M5 では撤去か改名を行う。

### 余力があれば追加する機能（優先度順）
1. 雑魚の追加（2〜3 種へ）
2. ボス 1 体
3. HP/スコア UI、タイトル、リザルト画面の作り込み
4. シェーダーのホットリロード

---

## 5. 素材制作の方針

- 最初は立方体の仮モデルで進め、素材待ちでコードを止めない。
- 各マイルストーンの直前に、必要なモデルだけ作る。
- 必要モデル数の目安：自機 1、雑魚 2〜3、ボス 1、背景ブロック 1〜2。

| マイルストーン | 必要な素材 |
|---|---|
| M2 | 動作確認用のモデル 1 つ |
| M3 | 自機、弾 |
| M4 | 雑魚 1 種 |
| 余力 | 雑魚追加、ボス、背景 |

---

## 6. 設計概要

### 6.1 描画パイプライン（案）
現状の `main` ループに沿った順序にする。3D 描画は `BeginImGui` の前に行う。
```
while (Engine::ProcessEvents())
  ├─ ロジック更新（SceneManager::Update(dt) など）
  ├─ Engine::Clear()                 // COLOR | DEPTH | STENCIL をクリア
  ├─ glEnable(GL_DEPTH_TEST)
  ├─ Camera: View / Projection を計算
  ├─ Shader を Use し、uniform を設定（行列、ライト、カメラ位置）
  ├─ 各 GameObject の Model を描画（SceneManager::Render()）
  ├─ Engine::BeginImGui()            // ここから UI
  ├─ UI 描画（OnImGuiRender など）
  ├─ Engine::EndImGui()              // ImGui の GL コマンドを発行
  └─ Engine::Present()
```
- VAO / VBO / EBO は `Mesh` が所有する（RAII。コピー禁止、ムーブ可）。`GameObject` は `Model*` の参照だけを持つ。
- GL リソースは `Engine::Init()` の後に生成する。

### 6.2 追加するクラス（案）
| クラス | 場所（M4 まで） | 場所（M5 以降） | 役割 |
|---|---|---|---|
| Shader | `src/` | `engine/` | GLSL のロード、コンパイル、uniform 設定 |
| Mesh | `src/` | `engine/` | VAO/VBO/EBO の管理と描画 |
| Model | `src/` | `engine/` | Assimp による読込。複数 Mesh を保持 |
| Camera | `src/` | `engine/` | View/Projection の生成 |
| GameObject | `src/` | `src/` | 位置、回転、スケール、当たり半径 |
| Player / Enemy / Bullet | `src/` | `src/` | ゲーム固有 |
| BulletPool | `src/` | `src/` | 弾の再利用 |

### 6.3 当たり判定
```cpp
// 球同士の判定（二乗距離で比較し、平方根を避ける）
bool Intersects(const glm::vec3& a, float ra, const glm::vec3& b, float rb)
{
    glm::vec3 d = a - b;
    float r = ra + rb;
    return glm::dot(d, d) <= r * r;
}
```

---

## 7. リスク

| リスク | 影響 | 対策 |
|---|---|---|
| MagicaVoxel の素材制作に時間がかかる | M3〜M4 が遅れる | 仮モデルで進める。モデル数を絞る |
| パレットテクスチャの扱い | M2 が遅れる | M2 でテクスチャ付き OBJ を最初に検証する |
| OpenGL のデバッグが難しい | M1 が遅れる | `glGetError` とシェーダーのエラーログを最初に実装する |
| エンジン抽出で依存関係が崩れる | M5 が遅れる | M1 から `src/` 内で Shader/Mesh を独立したクラスとして書く |
| 旧 `Renderer` との混在 | 設計が混乱する | M3 前に撤去か改名かを決める |
| 既存ドキュメントと現状コードの乖離 | 設計判断を誤る | 本計画書の「現状のエンジンの前提」を正とする。report_2 は参考にとどめる |
| 深度バッファ未設定 | 深度テストが効かない | M1 で `SDL_GL_DEPTH_SIZE` を設定する |
| 作業時間のばらつき | 全体が遅れる | 切り捨て優先度に従って機能を削る |

---

## 8. 導入するライブラリ

| ライブラリ | 状態 | 用途 |
|---|---|---|
| GLM | 新規（`vcpkg.json` に追加） | ベクトル、行列 |
| Assimp | 導入済み | モデル読込 |
| glad / SDL2 / ImGui | 導入済み | 既存 |

---

## 9. `engine_architecture_report_3.md` の章立て雛形

前回の `engine_architecture_report_2.md` と同じ形式で、M5 完了後に作成する。

1. はじめに：開発サイクルの背景と目的
2. 開発サイクルにおける進化の系譜（1作目〜3作目の比較表）
3. アーキテクチャ構造図（mermaid）
4. 主要な設計変更点と技術的ハイライト
   - Shader 管理
   - Mesh / Model（Assimp）
   - Camera
   - 3D 描画パイプライン
   - 旧 `Renderer` の整理
5. 学んだ OpenGL のブラックボックス（座標変換、深度テスト、ライティング）
6. 今後の展望

---

## 10. 次のアクション

1. `vcpkg.json` に `glm` を追加する。
2. `Application.cpp` に `SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24)` を追加する。
3. M1 の着手：Shader クラス、立方体の描画、カメラを実装する。
