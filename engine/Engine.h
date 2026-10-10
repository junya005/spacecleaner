#pragma once

/// <summary>
/// ゲームエンジンの全コアモジュール（Application, Window, Time, Input, AssetManager, SceneManager, Config等）を一括インクルードするためのマスターヘッダーです。
/// </summary>
/// <remarks>
/// ゲーム側のソースファイルはこのヘッダーをインクルードするだけで、エンジンの全主要機能を利用できます。
/// <code>
/// #include "Engine.h"
/// 
/// int main(int argc, char* argv[]) {
///     Engine::Init("My Game", 1280, 720);
///     while (Engine::ProcessEvents()) {
///         Engine::Clear();
///         Engine::BeginImGui();
///         // ゲーム描画・UI
///         Engine::EndImGui();
///         Engine::Present();
///     }
///     Engine::Shutdown();
///     return 0;
/// }
/// </code>
/// </remarks>

#include "Config.h"
#include "Time.h"
#include "Window.h"
#include "Renderer.h"
#include "TextRenderer.h"
#include "AssetManager.h"
#include "Input.h"
#include "UiWidget.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Application.h"
