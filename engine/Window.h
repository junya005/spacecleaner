#pragma once

#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <SDL_opengl.h>
#include <string>

namespace Engine
{
	/// <summary>
	/// アプリケーションウィンドウの生成・サイズ管理、およびOpenGL描画コンテキスト・GLAD初期化・垂直同期（VSync）を一括制御するためのクラスです。
	/// </summary>
	/// <remarks>
	/// アプリケーション起動時に Engine::Application 内部で自動初期化されます。
	/// 任意の場所から静的アクセサを介して画面サイズやウィンドウハンドルをグローバルに取得可能です。
	/// <code>
	/// // 画面中央の座標を取得して配置する例
	/// int centerX = Engine::Window::CenterX();
	/// int centerY = Engine::Window::CenterY();
	/// 
	/// // 画面解像度を取得する例
	/// int width = Engine::Window::Width();
	/// int height = Engine::Window::Height();
	/// </code>
	/// </remarks>
	class Window {
	public:
		/// <summary>
		/// Window クラスのインスタンスを生成し、シングルトンインスタンスポインタを自身に設定します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// Engine::Window window;
		/// </code>
		/// </remarks>
		Window();

		/// <summary>
		/// ウィンドウおよびOpenGLコンテキストのリソースを破棄します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// // スコープを抜ける際に自動的に Cleanup() が呼ばれます
		/// </code>
		/// </remarks>
		~Window();

		/// <summary>
		/// SDL_Window の生成、OpenGLコンテキスト作成、GLAD関数ポインタのロード、VSync設定を一括実行するために存在します。
		/// </summary>
		/// <param name="title">ウィンドウのタイトルバーに表示される文字列</param>
		/// <param name="width">ウィンドウの横幅（ピクセル単位）</param>
		/// <param name="height">ウィンドウの縦幅（ピクセル単位）</param>
		/// <param name="flags">SDL_WindowFlags（既定値: SDL_WINDOW_OPENGL）</param>
		/// <param name="vsync">垂直同期を有効にするか（既定値: true）</param>
		/// <returns>すべての初期化処理が正常に完了した場合は true、失敗した場合は false</returns>
		/// <remarks>
		/// <code>
		/// Engine::Window window;
		/// if (!window.Initialize("MyGame", 1280, 720, SDL_WINDOW_OPENGL, true)) {
		///     // 初期化失敗時のエラー処理
		/// }
		/// </code>
		/// </remarks>
		bool Initialize(const std::string& title, int width, int height, Uint32 flags = SDL_WINDOW_OPENGL, bool vsync = true);

		/// <summary>
		/// 生成された OpenGL コンテキストおよび SDL_Window を安全に破棄し、リソースリークを防ぐために存在します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// window.Cleanup();
		/// </code>
		/// </remarks>
		void Cleanup();

		/// <summary>
		/// ダブルバッファリングにおけるフロントバッファとバックバッファをスワップし、描画内容を画面へ反映するために存在します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// // 描画パスの最後に呼び出して画面を更新
		/// window.SwapBuffers();
		/// </code>
		/// </remarks>
		void SwapBuffers();

		/// <summary>
		/// 内部で保持している SDL_Window へのネイティブポインタを取得します。
		/// </summary>
		/// <returns>有効な SDL_Window ポインタ。未初期化の場合は nullptr</returns>
		/// <remarks>
		/// <code>
		/// SDL_Window* rawWindow = window.GetSDLWindow();
		/// </code>
		/// </remarks>
		SDL_Window* GetSDLWindow() const { return m_window; }

		/// <summary>
		/// 内部で保持している SDL_GLContext のハンドルを取得します。
		/// </summary>
		/// <returns>有効な SDL_GLContext ハンドル。未初期化の場合は nullptr</returns>
		/// <remarks>
		/// <code>
		/// SDL_GLContext glCtx = window.GetGLContext();
		/// </code>
		/// </remarks>
		SDL_GLContext GetGLContext() const { return m_glContext; }

		/// <summary>
		/// ウィンドウの現在の横幅（ピクセル）を取得します。
		/// </summary>
		/// <returns>ウィンドウの横幅</returns>
		/// <remarks>
		/// <code>
		/// int w = window.GetWidth();
		/// </code>
		/// </remarks>
		int GetWidth() const { return m_width; }

		/// <summary>
		/// ウィンドウの現在の縦幅（ピクセル）を取得します。
		/// </summary>
		/// <returns>ウィンドウの縦幅</returns>
		/// <remarks>
		/// <code>
		/// int h = window.GetHeight();
		/// </code>
		/// </remarks>
		int GetHeight() const { return m_height; }

		/// <summary>
		/// ウィンドウ横幅の中心座標（X座標）を取得します。
		/// </summary>
		/// <returns>ウィンドウ横幅の半値（width / 2）</returns>
		/// <remarks>
		/// <code>
		/// int centerX = window.GetWidthCenter();
		/// </code>
		/// </remarks>
		int GetWidthCenter() const { return m_width / 2; }

		/// <summary>
		/// ウィンドウ縦幅の中心座標（Y座標）を取得します。
		/// </summary>
		/// <returns>ウィンドウ縦幅の半値（height / 2）</returns>
		/// <remarks>
		/// <code>
		/// int centerY = window.GetHeightCenter();
		/// </code>
		/// </remarks>
		int GetHeightCenter() const { return m_height / 2; }

		// 静的アクセサ

		/// <summary>
		/// 現在アクティブな Window シングルトンインスタンスへのポインタを取得します。
		/// </summary>
		/// <returns>現在有効な Window インスタンスポインタ。未生成の場合は nullptr</returns>
		/// <remarks>
		/// <code>
		/// Engine::Window* win = Engine::Window::Get();
		/// </code>
		/// </remarks>
		static Window* Get() { return s_instance; }

		/// <summary>
		/// グローバルアクセスにより SDL_Window ポインタを取得します。
		/// </summary>
		/// <returns>有効な SDL_Window ポインタ。未生成の場合は nullptr</returns>
		/// <remarks>
		/// <code>
		/// SDL_Window* rawWindow = Engine::Window::GetWindow();
		/// </code>
		/// </remarks>
		static SDL_Window* GetWindow() { return s_instance ? s_instance->GetSDLWindow() : nullptr; }

		/// <summary>
		/// グローバルアクセスにより SDL_GLContext ハンドルを取得します。
		/// </summary>
		/// <returns>有効な SDL_GLContext ハンドル。未生成の場合は nullptr</returns>
		/// <remarks>
		/// <code>
		/// SDL_GLContext glContext = Engine::Window::GetContext();
		/// </code>
		/// </remarks>
		static SDL_GLContext GetContext() { return s_instance ? s_instance->GetGLContext() : nullptr; }

		/// <summary>
		/// グローバルアクセスによりウィンドウの横幅（ピクセル）を取得します。
		/// </summary>
		/// <returns>ウィンドウの横幅</returns>
		/// <remarks>
		/// <code>
		/// int screenWidth = Engine::Window::Width();
		/// </code>
		/// </remarks>
		static int Width() { return s_instance ? s_instance->GetWidth() : 0; }

		/// <summary>
		/// グローバルアクセスによりウィンドウの縦幅（ピクセル）を取得します。
		/// </summary>
		/// <returns>ウィンドウの縦幅</returns>
		/// <remarks>
		/// <code>
		/// int screenHeight = Engine::Window::Height();
		/// </code>
		/// </remarks>
		static int Height() { return s_instance ? s_instance->GetHeight() : 0; }

		/// <summary>
		/// グローバルアクセスにより画面の水平中心座標を取得します。
		/// </summary>
		/// <returns>水平中心X座標（Width / 2）</returns>
		/// <remarks>
		/// <code>
		/// int midX = Engine::Window::CenterX();
		/// </code>
		/// </remarks>
		static int CenterX() { return s_instance ? s_instance->GetWidthCenter() : 0; }

		/// <summary>
		/// グローバルアクセスにより画面の垂直中心座標を取得します。
		/// </summary>
		/// <returns>垂直中心Y座標（Height / 2）</returns>
		/// <remarks>
		/// <code>
		/// int midY = Engine::Window::CenterY();
		/// </code>
		/// </remarks>
		static int CenterY() { return s_instance ? s_instance->GetHeightCenter() : 0; }

	private:
		/// <summary>SDLが管理するネイティブウィンドウオブジェクトへのポインタ</summary>
		SDL_Window* m_window = nullptr;

		/// <summary>SDLが管理するOpenGLレンダリングコンテキストハンドル</summary>
		SDL_GLContext m_glContext = nullptr;

		/// <summary>ウィンドウの横幅サイズ（ピクセル単位）</summary>
		int m_width = 0;

		/// <summary>ウィンドウの縦幅サイズ（ピクセル単位）</summary>
		int m_height = 0;

		/// <summary>シングルトンアクセス用の静的インスタンス参照</summary>
		static Window* s_instance;
	};
}
