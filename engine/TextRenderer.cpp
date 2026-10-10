#include "TextRenderer.h"

namespace Engine {
	TextRenderer::TextRenderer(): m_is_initialized(false){
	
	}

	TextRenderer::~TextRenderer() {
		Cleanup();
	}

	bool TextRenderer::Initialize() {
		// フォント描画システムの初期化
		if (TTF_Init() == -1) {
			SDL_Log("フォントレンダラーの初期化に失敗しました: %s", SDL_GetError());
			return false;
		}
		m_is_initialized = true;
		return true;
	}

	void TextRenderer::Cleanup() {
		if (m_is_initialized) {
			TTF_Quit();
			m_is_initialized = false;
		}
	}

	// 自由な文字列を描画する関数
	void TextRenderer::DrawTextBySDL(Renderer &renderer, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color) {
		if (text.empty()) return;

		// 文字列からサーフェスを作成 (日本語などの場合は UTF8 を使用)
		SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
		if (!surface) {
			SDL_Log("サーフェスの作成に失敗: %s", TTF_GetError());
			return;
		}

		// サーフェスからテクスチャを作成
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.GetSDLRenderer(), surface);
		if (!texture) {
			SDL_Log("テクスチャの作成に失敗: %s", SDL_GetError());
			SDL_FreeSurface(surface);
			return;
		}

		// 描画サイズと位置を設定
		SDL_Rect dstRect = { x, y, surface->w, surface->h };

		// レンダーにコピーして画面に描画
		renderer.DrawTexture(texture, nullptr, &dstRect);

		// メモリの解放（毎フレーム作成するため必須）
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
	}
}
