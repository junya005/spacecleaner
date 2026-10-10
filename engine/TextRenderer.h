#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "Renderer.h"

namespace Engine
{
	class Renderer;

	/// <summary>
	/// SDL_ttf ライブラリの初期化・管理を行い、TrueTypeフォントを使用したテキスト文字列のサーフェス生成および画面描画を行うためのクラスです。
	/// </summary>
	/// <remarks>
	/// SDLネイティブレンダラーを使用したテキスト描画パイプラインを提供します。
	/// <code>
	/// Engine::TextRenderer textRenderer;
	/// if (textRenderer.Initialize()) {
	///     TTF_Font* font = Engine::AssetManager::LoadFont("assets/font.ttf", 24);
	///     SDL_Color white = { 255, 255, 255, 255 };
	///     textRenderer.DrawTextBySDL(renderer, font, "Score: 100", 20, 20, white);
	/// }
	/// </code>
	/// </remarks>
	class TextRenderer {
	public:
		/// <summary>
		/// TextRenderer インスタンスを構築し、初期化フラグを false に設定します。
		/// </summary>
		TextRenderer();

		/// <summary>
		/// インスタンス破棄時に Cleanup() を呼び出してフォントサブシステムを安全に終了します。
		/// </summary>
		~TextRenderer();

		/// <summary>
		/// SDL_ttf サブシステム（TTF_Init）を初期化するために存在します。
		/// </summary>
		/// <returns>TTF初期化に成功した場合は true、失敗した場合は false</returns>
		/// <remarks>
		/// <code>
		/// textRenderer.Initialize();
		/// </code>
		/// </remarks>
		bool Initialize();

		/// <summary>
		/// SDL_ttf サブシステム（TTF_Quit）を終了し、フォント関連リソースを解放するために存在します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// textRenderer.Cleanup();
		/// </code>
		/// </remarks>
		void Cleanup();

		/// <summary>
		/// 指定されたフォント・文字列・色からテキストサーフェスおよびテクスチャを一時生成し、指定座標にレンダリングするために存在します。
		/// </summary>
		/// <param name="renderer">描画に使用する Engine::Renderer 参照</param>
		/// <param name="font">描画に使用する TTF_Font ポインタ</param>
		/// <param name="text">描画する文字列（UTF-8）</param>
		/// <param name="x">画面上の描画開始X座標（ピクセル）</param>
		/// <param name="y">画面上の描画開始Y座標（ピクセル）</param>
		/// <param name="color">文字色を表す SDL_Color（RGBA）</param>
		/// <remarks>
		/// <code>
		/// SDL_Color yellow = { 255, 255, 0, 255 };
		/// textRenderer.DrawTextBySDL(renderer, font, "Game Over", 300, 200, yellow);
		/// </code>
		/// </remarks>
		void DrawTextBySDL(Renderer& renderer, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color);

	private:
		/// <summary>TTF_Init による初期化が完了しているかを示すフラグ</summary>
		bool m_is_initialized = false;
	};
}
