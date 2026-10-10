#include "Renderer.h"

namespace Engine 
{
	Renderer::Renderer() : m_renderer(nullptr) {

	}

	Renderer::~Renderer() {
		Cleanup();
	}

	bool Renderer::Initialize(SDL_Window *window) {
		m_renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
		if (m_renderer == nullptr) {
			SDL_Log("レンダラーの初期化に失敗しました: %s", SDL_GetError());
			return false;
		}
		return true;
	}

	void Renderer::Cleanup() {
		if (m_renderer) {
			SDL_DestroyRenderer(m_renderer);
			m_renderer = nullptr;
		}
	}

	void Renderer::Clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
		SDL_RenderClear(m_renderer);
	}

	void Renderer::SetRendererDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
	}

	void Renderer::Present() {
		SDL_RenderPresent(m_renderer);
	}

	void Renderer::DrawRect(int x, int y, int width, int height) {
		SDL_Rect rect = { x, y, width, height };
		SDL_RenderDrawRect(m_renderer, &rect);
	}

	void Renderer::DrawRect(SDL_Rect& rect) {
		SDL_RenderFillRect(m_renderer, &rect);
	}

	void Renderer::DrawTexture(SDL_Texture* texture, const SDL_Rect* src_rect, const SDL_Rect* dst_rect, bool flip_horizontal) {
		if (flip_horizontal) {
			SDL_RenderCopyEx(m_renderer, texture, src_rect, dst_rect, 0.0, nullptr, SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
		}
		else {
			SDL_RenderCopy(m_renderer, texture, src_rect, dst_rect);
		}
	}
}
