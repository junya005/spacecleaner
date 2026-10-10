#define SDL_MAIN_HANDLED
#include "../engine/Engine.h"
#include <imgui.h>
#include <algorithm>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Engine {
	// ファイルを文字列でロードする関数
	inline std::string LoadFileToString(const std::string& filePath){
		std::ifstream file(filePath);
		if (!file.is_open()) {
			return "";
		}
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	class Shader {
	public:
		unsigned int ID;

		Shader(const char* vertexPath, const char* fragmentPath);
		void use();
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec2(const std::string& name, float x, float y) const;
		void setVec3(const std::string& name, float x, float y, float z) const;
		void setVec4(const std::string& name, float x, float y, float z, float w) const;
	private:
	};
}

// 三角形の頂点座標
inline float triangle_vertex[9] = {
	// 1つ目の三角形
	-0.5f,  -0.5f, 0.0f, // 左下頂点
	0.5f, -0.5f, 0.0f, // 右下頂点
	0.0f, 0.5f, 0.0f,  // 上頂点
};

inline float triangle_vertex2[9] = {
	// 2つ目の三角形
	0.5f,  -0.5f, 0.0f, // 左下頂点
	1.5f, -0.5f, 0.0f, // 右下頂点
	1.0f, 0.5f, 0.0f  // 上頂点
};

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

	// Hello Triangle: https://learnopengl.com/Getting-started/Hello-Triangle
	
	// VAOの作成とバインド
	unsigned int triangleVAOs[2];
	glGenVertexArrays(2, triangleVAOs);
	glBindVertexArray(triangleVAOs[0]);
	
	// 三角形の頂点バッファを作成
	unsigned int triangleVBOs[2];
	glGenBuffers(2, triangleVBOs);

	// 描画する三角形の頂点データを設定
	glBindVertexArray(triangleVAOs[0]);
	// 頂点バッファのバインドとデータ転送
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertex), triangle_vertex, GL_STATIC_DRAW);
	// 頂点属性の設定
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 2つ目の三角形も同様に
	glBindVertexArray(triangleVAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertex2), triangle_vertex2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

#pragma region Create Shader Program

	// 頂点シェーダーのロード
	std::string vertexShaderSource = Engine::LoadFileToString("assets/shader/simple_vertex_shader.glsl");
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSourceCStr = vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderSourceCStr, NULL);
	glCompileShader(vertexShader);
	
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// フラグメントシェーダーのロード
	std::string fragmentShaderSource = Engine::LoadFileToString("assets/shader/simple_fragment_shader.glsl");
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSourceCStr = fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSourceCStr, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// シェーダープログラムの作成
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

#pragma endregion
	
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

		// シェーダーに値を送信する
		float time = SDL_GetTicks() / 1000.0f;
		float greenValue = (sin(time) / 2.0f) + 0.5f;
		// uniform変数の取得
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		// Hello Triangle: 描画処理
		glUseProgram(shaderProgram);
		glBindVertexArray(triangleVAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(triangleVAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
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
