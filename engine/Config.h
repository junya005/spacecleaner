#pragma once
#include <string>
#include <array>

namespace Engine {

	/// <summary>
	/// ウィンドウサイズやタイトル、垂直同期、背景クリアカラーなど、エンジンの初期挙動を定義するための設定構造体です。
	/// </summary>
	/// <remarks>
	/// エンジン標準ではニュートラルな値が設定されており、ゲーム側の Application コンストラクタ等から上書きして利用します。
	/// <code>
	/// Engine::AppConfigData config;
	/// config.title = "My Action Game";
	/// config.windowWidth = 1920;
	/// config.windowHeight = 1080;
	/// config.vsync = true;
	/// config.clearColor = { 0.2f, 0.2f, 0.2f, 1.0f };
	/// </code>
	/// </remarks>
	struct AppConfigData {
		/// <summary>ウィンドウのタイトルバーに表示されるアプリケーション名</summary>
		std::string title = "Application";

		/// <summary>ウィンドウの初期横幅（ピクセル単位）</summary>
		int windowWidth = 1280;

		/// <summary>ウィンドウの初期縦幅（ピクセル単位）</summary>
		int windowHeight = 720;

		/// <summary>ディスプレイのリフレッシュレートと同期（垂直同期）を行うか</summary>
		bool vsync = true;

		/// <summary>毎フレームの画面クリア時に使用されるRGBAカラー値（各成分 0.0f ～ 1.0f）</summary>
		std::array<float, 4> clearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
	};

	/// <summary>
	/// アプリケーション全体の設定データ（AppConfigData）をグローバルに保持・参照・変更するために存在する静的管理クラスです。
	/// </summary>
	/// <remarks>
	/// Application::Initialize() で初期化され、エンジン内部およびゲーム側から現在の画面設定を参照する際に利用します。
	/// <code>
	/// // 現在の背景クリアカラーを取得して確認する例
	/// const auto& config = Engine::Config::Get();
	/// float clearR = config.clearColor[0];
	/// 
	/// // 設定を動的に更新する例
	/// Engine::AppConfigData newConfig = Engine::Config::Get();
	/// newConfig.clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	/// Engine::Config::Set(newConfig);
	/// </code>
	/// </remarks>
	class Config {
	public:
		/// <summary>
		/// 指定された設定データを用いて設定マネージャーを初期化するために存在します。
		/// </summary>
		/// <param name="initialData">初期値としてセットする AppConfigData 構造体</param>
		/// <remarks>
		/// <code>
		/// Engine::Config::Initialize({ .title = "Game Title", .windowWidth = 1280, .windowHeight = 720 });
		/// </code>
		/// </remarks>
		static void Initialize(const AppConfigData& initialData = AppConfigData{});

		/// <summary>
		/// 現在保持されているアプリケーション設定データの参照を取得します。
		/// </summary>
		/// <returns>現在有効な AppConfigData への不変参照</returns>
		/// <remarks>
		/// <code>
		/// const auto& cfg = Engine::Config::Get();
		/// int currentWidth = cfg.windowWidth;
		/// </code>
		/// </remarks>
		static const AppConfigData& Get();

		/// <summary>
		/// 新しい設定データを適用して設定情報を更新するために存在します。
		/// </summary>
		/// <param name="data">更新後の新しい AppConfigData</param>
		/// <remarks>
		/// <code>
		/// Engine::Config::Set(updatedConfig);
		/// </code>
		/// </remarks>
		static void Set(const AppConfigData& data);

	private:
		/// <summary>アプリケーション全体で共有される静的設定データの実体</summary>
		static AppConfigData s_configData;
	};

}
