#pragma once
#include <SDL2/SDL.h>
#include <SDL_opengl.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <string>

namespace Engine
{
	/// <summary>
	/// SDL_Renderer による2Dプリミティブ描画（四角形やテクスチャ転送）をカプセル化・実行するためのクラスです。
	/// </summary>
	/// <remarks>
	/// SDLネイティブ描画パイプラインを使用する場合に利用します。デストラクタでリソース解放を行うため、ヒープメモリに無用な保持は避けてください。
	/// <code>
	/// Engine::Renderer renderer;
	/// if (renderer.Initialize(window.GetSDLWindow())) {
	///     renderer.Clear(0, 0, 0, 255);
	///     renderer.DrawRect(100, 100, 50, 50);
	///     renderer.Present();
	/// }
	/// </code>
	/// </remarks>
	class Renderer
	{
	public:
		/// <summary>
		/// Renderer インスタンスを構築し、内部の SDL_Renderer ポインタを初期化します。
		/// </summary>
		Renderer();

		/// <summary>
		/// レンダラーリソースを破棄し、メモリリークを防ぎます。
		/// </summary>
		~Renderer();

		/// <summary>
		/// 指定された SDL_Window に対するハードウェアアクセラレーション有効の SDL_Renderer を生成するために存在します。
		/// </summary>
		/// <param name="window">描画対象となる SDL_Window ポインタ</param>
		/// <returns>レンダラー生成に成功した場合は true、失敗した場合は false</returns>
		/// <remarks>
		/// <code>
		/// renderer.Initialize(window.GetSDLWindow());
		/// </code>
		/// </remarks>
		bool Initialize(SDL_Window *window);

		/// <summary>
		/// 保持している SDL_Renderer を安全に破棄するために存在します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// renderer.Cleanup();
		/// </code>
		/// </remarks>
		void Cleanup();

		/// <summary>
		/// レンダリングターゲットを指定色でクリアするために存在します。
		/// </summary>
		/// <param name="r">赤成分（0～255）</param>
		/// <param name="g">緑成分（0～255）</param>
		/// <param name="b">青成分（0～255）</param>
		/// <param name="a">アルファ成分（0～255、既定値: 255）</param>
		/// <remarks>
		/// <code>
		/// renderer.Clear(30, 30, 30, 255);
		/// </code>
		/// </remarks>
		void Clear(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255);

		/// <summary>
		/// 描画プリミティブ（矩形や直線など）で使用される描画色を設定するために存在します。
		/// </summary>
		/// <param name="r">赤成分（0～255）</param>
		/// <param name="g">緑成分（0～255）</param>
		/// <param name="b">青成分（0～255）</param>
		/// <param name="a">アルファ成分（0～255）</param>
		/// <remarks>
		/// <code>
		/// renderer.SetRendererDrawColor(255, 0, 0, 255);
		/// </code>
		/// </remarks>
		void SetRendererDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

		/// <summary>
		/// バックバッファの描画内容を画面へ転送・提示するために存在します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// renderer.Present();
		/// </code>
		/// </remarks>
		void Present();

		/// <summary>
		/// 座標とサイズを指定して矩形の輪郭線を描画するために存在します。
		/// </summary>
		/// <param name="x">左上X座標</param>
		/// <param name="y">左上Y座標</param>
		/// <param name="width">矩形の横幅</param>
		/// <param name="height">矩形の縦幅</param>
		/// <remarks>
		/// <code>
		/// renderer.DrawRect(10, 10, 100, 50);
		/// </code>
		/// </remarks>
		void DrawRect(int x, int y, int width, int height);

		/// <summary>
		/// SDL_Rect 構造体で指定された領域を塗りつぶし矩形として描画するために存在します。
		/// </summary>
		/// <param name="rect">描画する領域を表す SDL_Rect 参照</param>
		/// <remarks>
		/// <code>
		/// SDL_Rect box = { 50, 50, 200, 100 };
		/// renderer.DrawRect(box);
		/// </code>
		/// </remarks>
		void DrawRect(SDL_Rect& rect);

		/// <summary>
		/// 指定された SDL_Texture を画面の目標領域へ描画・転送するために存在します。
		/// </summary>
		/// <param name="texture">描画元の SDL_Texture ポインタ</param>
		/// <param name="src_rect">テクスチャ内の切り出し領域（全体の場合は nullptr）</param>
		/// <param name="dst_rect">描画先となる画面座標領域</param>
		/// <param name="flip_horizontal">水平反転描画を行うか（既定値: false）</param>
		/// <remarks>
		/// <code>
		/// SDL_Rect dst = { 100, 100, 64, 64 };
		/// renderer.DrawTexture(playerTex, nullptr, &amp;dst, false);
		/// </code>
		/// </remarks>
		void DrawTexture(SDL_Texture *texture, const SDL_Rect *src_rect, const SDL_Rect *dst_rect, bool flip_horizontal = false);
		
		/// <summary>
		/// 内部で保持しているネイティブ SDL_Renderer ポインタを取得します。
		/// </summary>
		/// <returns>有効な SDL_Renderer ポインタ</returns>
		/// <remarks>
		/// <code>
		/// SDL_Renderer* rawRenderer = renderer.GetSDLRenderer();
		/// </code>
		/// </remarks>
		SDL_Renderer* GetSDLRenderer() const { return m_renderer; }

	private:
		/// <summary>SDLが管理する2Dレンダリングコンテキストオブジェクト</summary>
		SDL_Renderer* m_renderer = nullptr;
	};
	
	/// <summary>
	/// ImGui のカーソル位置を設定し、指定された OpenGL テクスチャを四角形ウィジェットとして描画するために存在するヘルパー関数です。
	/// </summary>
	/// <param name="textureID">描画する OpenGL テクスチャ識別子</param>
	/// <param name="position">描画領域の左上座標（ImVec2）</param>
	/// <param name="size">テクスチャの描画サイズ（ImVec2）</param>
	/// <remarks>
	/// <code>
	/// Engine::DrawTexture(logoTexID, ImVec2(100.0f, 50.0f), ImVec2(200.0f, 100.0f));
	/// </code>
	/// </remarks>
	inline void DrawTexture(GLuint textureID, const ImVec2& position, const ImVec2& size) {
		ImGui::SetCursorPos(position);
		ImGui::Image((ImTextureID)(intptr_t)textureID, size);
	}

	/// <summary>
	/// 指定された中心座標を基準として、OpenGL テクスチャを中心揃えで ImGui ウィジェットとして描画するために存在するヘルパー関数です。
	/// </summary>
	/// <param name="textureID">描画する OpenGL テクスチャ識別子</param>
	/// <param name="centerPosition">画像の中心位置となる座標（ImVec2）</param>
	/// <param name="size">テクスチャの描画サイズ（ImVec2）</param>
	/// <remarks>
	/// <code>
	/// ImVec2 screenCenter(Engine::Window::CenterX(), Engine::Window::CenterY());
	/// Engine::DrawTextureCentered(logoTexID, screenCenter, ImVec2(384.0f, 128.0f));
	/// </code>
	/// </remarks>
	inline void DrawTextureCentered(GLuint textureID, const ImVec2& centerPosition, const ImVec2& size) {
		ImVec2 position = ImVec2(centerPosition.x - size.x * 0.5f, centerPosition.y - size.y * 0.5f);
		DrawTexture(textureID, position, size);
	}

}
