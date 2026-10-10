#pragma once

namespace Engine {

	/// <summary>
	/// タイトル画面やバトル画面、リザルト画面など、独立した状態・画面単位でライフサイクルと描画を管理するための抽象基底クラスです。
	/// </summary>
	/// <remarks>
	/// SceneManager を通じて切り替えられ、開始時（OnEnter）、終了時（OnExit）、毎フレーム更新（OnUpdate）、
	/// 直接描画（OnRender）、UI描画（OnImGuiRender）の各フェーズが適切な順序で自動実行されます。
	/// <code>
	/// class TitleScene : public Engine::Scene {
	/// public:
	///     void OnEnter() override {
	///         // BGM再生やリソースロード
	///     }
	///     void OnUpdate(float deltaTime) override {
	///         // 入力判定やアニメーション更新
	///     }
	///     void OnImGuiRender() override {
	///         if (ImGui::Button("Game Start")) {
	///             Engine::SceneManager::ChangeScene<BattleScene>();
	///         }
	///     }
	///     void OnExit() override {
	///         // 一時リソースの解放
	///     }
	/// };
	/// </code>
	/// </remarks>
	class Scene {
	public:
		/// <summary>
		/// 派生クラスのリソースを安全に解放するための仮想デストラクタです。
		/// </summary>
		virtual ~Scene() = default;

		/// <summary>
		/// シーン遷移によって本シーンがアクティブになった直後に、初期化やリソース読み込みを行うために存在します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// void OnEnter() override {
		///     m_logoTexture = Engine::AssetManager::LoadTexture("assets/logo.png");
		/// }
		/// </code>
		/// </remarks>
		virtual void OnEnter() {}

		/// <summary>
		/// 別のシーンへの遷移によって本シーンが非アクティブ化・破棄される直前に、後処理やデータ保存を行うために存在します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// void OnExit() override {
		///     // 一時キャッシュや動的オブジェクトの解放
		/// }
		/// </code>
		/// </remarks>
		virtual void OnExit() {}

		/// <summary>
		/// 毎フレームのシーン固有ロジックや物理挙動を更新するために存在します。
		/// </summary>
		/// <param name="deltaTime">前フレームからの経過時間（秒単位）</param>
		/// <remarks>
		/// <code>
		/// void OnUpdate(float deltaTime) override {
		///     m_player.Update(deltaTime);
		/// }
		/// </code>
		/// </remarks>
		virtual void OnUpdate(float deltaTime) { (void)deltaTime; }

		/// <summary>
		/// 画面クリア後、ImGui描画前に生OpenGL描画（3Dモデル描画や背景スプライト描画など）を行うために存在します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// void OnRender() override {
		///     // 3D背景メッシュの描画
		/// }
		/// </code>
		/// </remarks>
		virtual void OnRender() {}

		/// <summary>
		/// ImGuiのフレーム開始後、ImGuiによるUIウィジェット（ボタン、テキスト、ウィンドウ等）を描画するために存在します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// void OnImGuiRender() override {
		///     ImGui::Text("Hello, Scene!");
		/// }
		/// </code>
		/// </remarks>
		virtual void OnImGuiRender() {}
	};

}
