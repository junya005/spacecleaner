#define SDL_MAIN_HANDLED
#include "../engine/Engine.h"
#include <imgui.h>
#include <algorithm>

/// <summary>
/// アプリケーションのエントリポイント
/// </summary>
int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;

	// 1. エンジンの初期化
	if (!Engine::Init("Template Game", 1280, 720)) {
		return -1;
	}

	// プレイヤーの変数（初期位置、速度、サイズ）
	float playerX = 100.0f;
	float playerY = 100.0f;
	float playerSpeed = 300.0f;
	float playerSize = 50.0f;

	// 2. メインゲームループ
	while (Engine::ProcessEvents()) {
		float deltaTime = Engine::Time::GetDeltaTime();

		// WASDキー入力によるプレイヤー移動
		float moveDistance = playerSpeed * deltaTime;
		if (Engine::Input::GetKey(SDL_SCANCODE_W)) {
			playerY -= moveDistance;
		}
		if (Engine::Input::GetKey(SDL_SCANCODE_S)) {
			playerY += moveDistance;
		}
		if (Engine::Input::GetKey(SDL_SCANCODE_A)) {
			playerX -= moveDistance;
		}
		if (Engine::Input::GetKey(SDL_SCANCODE_D)) {
			playerX += moveDistance;
		}

		// 画面外への飛び出し防止（クランプ処理）
		int screenWidth = Engine::Window::Width();
		int screenHeight = Engine::Window::Height();
		if (screenWidth > 0 && screenHeight > 0) {
			playerX = std::clamp(playerX, 0.0f, static_cast<float>(screenWidth) - playerSize);
			playerY = std::clamp(playerY, 0.0f, static_cast<float>(screenHeight) - playerSize);
		}

		// 画面クリア
		Engine::Clear(0.1f, 0.12f, 0.15f, 1.0f);

		// ImGui フレーム開始
		Engine::BeginImGui();

		// プレイヤー四角形の描画（ImGui BackgroundDrawList）
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();
		ImVec2 pMin(playerX, playerY);
		ImVec2 pMax(playerX + playerSize, playerY + playerSize);
		drawList->AddRectFilled(pMin, pMax, IM_COL32(255, 255, 255, 255));
		drawList->AddRect(pMin, pMax, IM_COL32(100, 200, 255, 255), 0.0f, 0, 2.0f);

		// デバッグ・操作情報ウィンドウの描画
		ImGui::SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(320.0f, 220.0f), ImGuiCond_Once);

		if (ImGui::Begin("Template Controller & Debug")) {
			ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "[Controls]");
			ImGui::BulletText("W / A / S / D : Move Square");

			ImGui::Separator();
			ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "[Performance]");
			ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
			ImGui::Text("DeltaTime: %.4f s", Engine::Time::GetDeltaTime());
			ImGui::Text("TotalTime: %.1f s", Engine::Time::GetTotalTime());

			ImGui::Separator();
			ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "[Player Info]");
			ImGui::Text("Position: (%.1f, %.1f)", playerX, playerY);
			ImGui::SliderFloat("Speed", &playerSpeed, 100.0f, 1000.0f);

			if (ImGui::Button("Reset Position")) {
				playerX = 100.0f;
				playerY = 100.0f;
			}
		}
		ImGui::End();

		// ImGui 描画発行
		Engine::EndImGui();

		// バックバッファスワップ（画面反映）
		Engine::Present();
	}

	// 3. エンジンの終了
	Engine::Shutdown();
	return 0;
}
