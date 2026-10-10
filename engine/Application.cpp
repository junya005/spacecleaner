#include "Application.h"
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL_opengl.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <memory>

namespace Engine {

	namespace {
		/// <summary>メインウィンドウおよびコンテキストを管理するインスタンス</summary>
		std::unique_ptr<Window> s_window = nullptr;

		/// <summary>ゲームループが実行継続中であるかを示すフラグ</summary>
		bool s_isRunning = false;
	}

	bool Init(const std::string& title, int width, int height, bool vsync) {
		// 初期設定の初期化
		AppConfigData config = {
			.title = title,
			.windowWidth = width,
			.windowHeight = height,
			.vsync = vsync,
			.clearColor = { 0.1f, 0.12f, 0.15f, 1.0f }
		};
		Config::Initialize(config);

		// SDLの初期化
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) < 0) {
			SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
			return false;
		}

		// OpenGLコンテキスト属性の設定
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		// ウィンドウおよびOpenGLコンテキストの初期化
		s_window = std::make_unique<Window>();
		if (!s_window->Initialize(title, width, height, SDL_WINDOW_OPENGL, vsync)) {
			SDL_Log("Failed to initialize Window");
			s_window.reset();
			return false;
		}

		// サブシステムの初期化
		Time::Initialize();
		Input::Initialize();
		AssetManager::Initialize();
		SceneManager::Initialize();

		// ImGuiの初期化
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		// ImGuiバックエンドの初期化
		ImGui_ImplSDL2_InitForOpenGL(s_window->GetSDLWindow(), s_window->GetGLContext());
		ImGui_ImplOpenGL3_Init("#version 150");

		s_isRunning = true;
		return true;
	}

	void Shutdown() {
		// シーンマネージャーの終了
		SceneManager::Shutdown();

		// ImGuiの終了処理
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		// 各サブシステムの解放
		AssetManager::Shutdown();

		if (s_window) {
			s_window->Cleanup();
			s_window.reset();
		}

		// SDLの終了
		SDL_Quit();
		s_isRunning = false;
	}

	bool ProcessEvents() {
		if (!s_isRunning) {
			return false;
		}

		// 1. 前フレームの単発打鍵フラグをクリア
		Input::OnBeginFrame();

		// 2. イベントポーリング
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				s_isRunning = false;
			}
			ImGui_ImplSDL2_ProcessEvent(&e);
			Input::OnProcessEvent(e);
		}

		if (!s_isRunning) {
			return false;
		}

		// 3. デルタタイム・時間の更新
		Time::Update();

		return true;
	}

	void Clear(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void BeginImGui() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
	}

	void EndImGui() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Present() {
		if (s_window) {
			s_window->SwapBuffers();
		}
	}

	void Quit() {
		s_isRunning = false;
	}

	bool IsRunning() {
		return s_isRunning;
	}

}
