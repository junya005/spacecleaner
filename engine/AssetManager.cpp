#include "AssetManager.h"

namespace Engine {

	AssetManager* AssetManager::s_instance = nullptr;

	AssetManager::AssetManager() {
		s_instance = this;
	}

	AssetManager::~AssetManager() {
		Cleanup();
		if (s_instance == this) {
			s_instance = nullptr;
		}
	}

	void AssetManager::Initialize() {
		static AssetManager defaultManager;
		s_instance = &defaultManager;
	}

	void AssetManager::Shutdown() {
		if (s_instance) {
			s_instance->Cleanup();
		}
	}

	SDL_Surface* AssetManager::GetSurface(const std::string& file_path) {
		auto it = m_surfaces.find(file_path);
		if (it != m_surfaces.end()) {
			return it->second;
		}
		SDL_Surface* surface = IMG_Load(file_path.c_str());
		if (!surface) {
			SDL_Log("画像の読み込みに失敗 %s: %s", file_path.c_str(), SDL_GetError());
			return nullptr;
		}
		m_surfaces[file_path] = surface;
		return surface;
	}

	TTF_Font* AssetManager::GetFont(const std::string& file_path, int font_size) {
		std::string key = file_path + "_" + std::to_string(font_size);

		auto it = m_fonts.find(key);
		if (it != m_fonts.end()) {
			return it->second;
		}

		TTF_Font* font = TTF_OpenFont(file_path.c_str(), font_size);
		if (!font) {
			SDL_Log("Font Load Error %s: %s", file_path.c_str(), SDL_GetError());
			return nullptr;
		}
		m_fonts[key] = font;
		return font;
	}

	GLuint AssetManager::GetTexture(const std::string& file_path) {
		auto it = m_textures.find(file_path);
		if (it != m_textures.end()) {
			return it->second;
		}

		SDL_Surface* surface = GetSurface(file_path);
		if (!surface) {
			return 0;
		}

		GLuint textureID = CreateTextureFromSurface(surface);
		if (textureID != 0) {
			m_textures[file_path] = textureID;
		}
		return textureID;
	}

	void AssetManager::Cleanup() {
		for (auto& pair : m_textures) {
			if (pair.second != 0) {
				glDeleteTextures(1, &pair.second);
			}
		}
		m_textures.clear();

		for (auto& pair : m_surfaces) {
			if (pair.second) {
				SDL_FreeSurface(pair.second);
			}
		}
		m_surfaces.clear();

		for (auto& pair : m_fonts) {
			if (pair.second) {
				TTF_CloseFont(pair.second);
			}
		}
		m_fonts.clear();
	}

	SDL_Surface* AssetManager::LoadSurface(const std::string& file_path) {
		if (s_instance) {
			return s_instance->GetSurface(file_path);
		}
		return nullptr;
	}

	TTF_Font* AssetManager::LoadFont(const std::string& file_path, int font_size) {
		if (s_instance) {
			return s_instance->GetFont(file_path, font_size);
		}
		return nullptr;
	}

	GLuint AssetManager::LoadTexture(const std::string& file_path) {
		if (s_instance) {
			return s_instance->GetTexture(file_path);
		}
		return 0;
	}

}