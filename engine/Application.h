#pragma once

#include "Window.h"
#include "Input.h"
#include "AssetManager.h"
#include "Time.h"
#include "Config.h"
#include "Scene.h"
#include "SceneManager.h"
#include <string>

namespace Engine {

	/// <summary>
	/// エンジン全体のサブシステム（SDL、OpenGL、GLAD、ImGui、Window、Time、Input、AssetManager等）を一括初期化します。
	/// </summary>
	/// <param name="title">ウィンドウのタイトルバーに表示される文字列</param>
	/// <param name="width">ウィンドウの横幅（ピクセル単位、既定値: 1280）</param>
	/// <param name="height">ウィンドウの縦幅（ピクセル単位、既定値: 720）</param>
	/// <param name="vsync">垂直同期を有効にするか（既定値: true）</param>
	/// <returns>初期化に成功した場合は true、失敗した場合は false</returns>
	/// <remarks>
	/// <code>
	/// if (!Engine::Init("My Game", 1280, 720)) {
	///     return -1;
	/// }
	/// </code>
	/// </remarks>
	bool Init(const std::string& title = "Template Game", int width = 1280, int height = 720, bool vsync = true);

	/// <summary>
	/// エンジン全体のサブシステムおよび確保されたリソースを安全に解放・シャットダウンします。
	/// </summary>
	/// <remarks>
	/// <code>
	/// Engine::Shutdown();
	/// </code>
	/// </remarks>
	void Shutdown();

	/// <summary>
	/// SDLイベントのポーリング、Inputの更新、Time（DeltaTime）の更新を一括で処理します。
	/// 終了要求（ウィンドウの閉じるボタン押下や Quit() 呼び出し）があった場合は false を返します。
	/// </summary>
	/// <returns>ゲームループを継続する場合は true、終了する場合は false</returns>
	/// <remarks>
	/// <code>
	/// while (Engine::ProcessEvents()) {
	///     // 毎フレームのロジック・描画処理
	/// }
	/// </code>
	/// </remarks>
	bool ProcessEvents();

	/// <summary>
	/// 画面バッファを指定された色でクリアします。
	/// </summary>
	/// <param name="r">赤成分（0.0f ～ 1.0f、既定値: 0.1f）</param>
	/// <param name="g">緑成分（0.0f ～ 1.0f、既定値: 0.12f）</param>
	/// <param name="b">青成分（0.0f ～ 1.0f、既定値: 0.15f）</param>
	/// <param name="a">アルファ成分（0.0f ～ 1.0f、既定値: 1.0f）</param>
	/// <remarks>
	/// <code>
	/// Engine::Clear(0.1f, 0.12f, 0.15f, 1.0f);
	/// </code>
	/// </remarks>
	void Clear(float r = 0.1f, float g = 0.12f, float b = 0.15f, float a = 1.0f);

	/// <summary>
	/// ImGui の新しい描画フレームを開始します（ImGui_ImplOpenGL3_NewFrame 等を呼び出します）。
	/// ImGui BackgroundDrawList によるゲーム描画や ImGui ウィジェット描画を行う前に呼び出してください。
	/// </summary>
	/// <remarks>
	/// <code>
	/// Engine::BeginImGui();
	/// // 描画処理や UI ウィジェット構築
	/// Engine::EndImGui();
	/// </code>
	/// </remarks>
	void BeginImGui();

	/// <summary>
	/// ImGui の描画コマンドを発行・レンダリングします（ImGui::Render およびバックエンド描画発行）。
	/// </summary>
	/// <remarks>
	/// <code>
	/// Engine::EndImGui();
	/// </code>
	/// </remarks>
	void EndImGui();

	/// <summary>
	/// バックバッファとフロントバッファをスワップし、描画内容を画面へ反映します。
	/// </summary>
	/// <remarks>
	/// <code>
	/// Engine::Present();
	/// </code>
	/// </remarks>
	void Present();

	/// <summary>
	/// 現在実行中のメインループに対して終了フラグをセットします（次回の ProcessEvents() が false を返します）。
	/// </summary>
	/// <remarks>
	/// <code>
	/// if (Engine::Input::GetKeyDown(SDLK_ESCAPE)) {
	///     Engine::Quit();
	/// }
	/// </code>
	/// </remarks>
	void Quit();

	/// <summary>
	/// メインゲームループが現在実行中であるかを取得します。
	/// </summary>
	/// <returns>実行中であれば true、終了フラグが立っていれば false</returns>
	/// <remarks>
	/// <code>
	/// bool running = Engine::IsRunning();
	/// </code>
	/// </remarks>
	bool IsRunning();

}
