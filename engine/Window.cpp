#include "Window.h"

namespace Engine
{
	Window* Window::s_instance = nullptr;

	Window::Window() : m_window(nullptr), m_glContext(nullptr), m_width(0), m_height(0) {
		s_instance = this;
	}

	Window::~Window() {
		Cleanup();
		if (s_instance == this) {
			s_instance = nullptr;
		}
	}

	bool Window::Initialize(const std::string& title, int width, int height, Uint32 flags, bool vsync) {
		m_width = width;
		m_height = height;

		// ウィンドウ作成
		m_window = SDL_CreateWindow(
			title.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			m_width,
			m_height,
			flags
		);

		if (m_window == nullptr) {
			SDL_Log("Failed to window initialization: %s", SDL_GetError());
			return false;
		}

		// OpenGLコンテキストの作成
		m_glContext = SDL_GL_CreateContext(m_window);
		if (m_glContext == nullptr) {
			SDL_Log("Failed to create OpenGL context: %s", SDL_GetError());
			Cleanup();
			return false;
		}

		// GLADの初期化
		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
			SDL_Log("Failed to initialize GLAD");
			Cleanup();
			return false;
		}

		// 垂直同期（VSync）の設定
		if (vsync) {
			if (SDL_GL_SetSwapInterval(1) < 0) {
				SDL_Log("Warning: Unable to set VSync: %s", SDL_GetError());
			}
		} else {
			SDL_GL_SetSwapInterval(0);
		}

		return true;
	}

	void Window::SwapBuffers() {
		if (m_window) {
			SDL_GL_SwapWindow(m_window);
		}
	}

	void Window::Cleanup() {
		if (m_glContext) {
			SDL_GL_DeleteContext(m_glContext);
			m_glContext = nullptr;
		}
		if (m_window) {
			SDL_DestroyWindow(m_window);
			m_window = nullptr;
		}	
	}
}
