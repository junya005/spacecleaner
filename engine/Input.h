#pragma once
#include <SDL2/SDL.h>
#include <unordered_map>

namespace Engine {

	/// <summary>
	/// キーボードの押下状態（押し続け判定）および瞬間的な打鍵イベント（押した瞬間判定）を追跡・提供するための入力管理クラスです。
	/// </summary>
	/// <remarks>
	/// Application::Run() のイベントポーリング時に SDL_Event を受け取り内部状態を更新します。
	/// ゲームロジックからは静的アクセサを介して任意のタイミングでキー入力を取得できます。
	/// <code>
	/// // 押し続け判定（移動などに使用）
	/// if (Engine::Input::GetKey(SDL_SCANCODE_RIGHT)) {
	///     playerX += speed * dt;
	/// }
	/// 
	/// // 押した瞬間判定（ジャンプやメニュー決定などに使用）
	/// if (Engine::Input::GetKeyDown(SDLK_SPACE)) {
	///     player.Jump();
	/// }
	/// </code>
	/// </remarks>
	class Input {
	public:
		/// <summary>
		/// Input クラスのインスタンスを生成し、SDLキーボード状態ポインタを取得してシングルトン参照を設定します。
		/// </summary>
		Input();

		/// <summary>
		/// インスタンス破棄時にシングルトン参照を解除します。
		/// </summary>
		~Input();

		/// <summary>
		/// 静的入力マネージャーインスタンスを初期化し、利用可能にするために存在します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// Engine::Input::Initialize();
		/// </code>
		/// </remarks>
		static void Initialize();

		/// <summary>
		/// フレーム開始時に前フレームの単発打鍵フラグ（Key Down）をクリアするために存在します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// input.BeginFrame();
		/// </code>
		/// </remarks>
		void BeginFrame();

		/// <summary>
		/// SDL_PollEvent から取得した SDL_Event を解析し、キー押下イベントを内部状態へ記録するために存在します。
		/// </summary>
		/// <param name="e">処理対象の SDL_Event</param>
		/// <remarks>
		/// <code>
		/// input.ProcessEvent(e);
		/// </code>
		/// </remarks>
		void ProcessEvent(const SDL_Event& e);

		/// <summary>
		/// 指定されたスキャンコードのキーが現在押されている状態かを判定します。
		/// </summary>
		/// <param name="scancode">判定対象の物理キー位置を表す SDL_Scancode</param>
		/// <returns>キーが押されている場合は true、離されている場合は false</returns>
		/// <remarks>
		/// <code>
		/// bool isRightPressed = input.IsKey(SDL_SCANCODE_RIGHT);
		/// </code>
		/// </remarks>
		bool IsKey(SDL_Scancode scancode) const;

		/// <summary>
		/// 指定されたキーコードのキーが現在のフレームで新しく押された瞬間かを判定します。
		/// </summary>
		/// <param name="keycode">判定対象のキーを表す SDL_Keycode</param>
		/// <returns>今フレームでキーが押された瞬間であれば true、それ以外は false</returns>
		/// <remarks>
		/// <code>
		/// bool isEnterTriggered = input.IsKeyDown(SDLK_RETURN);
		/// </code>
		/// </remarks>
		bool IsKeyDown(SDL_Keycode keycode) const;

		// 互換性・静的アクセサ

		/// <summary>
		/// 現在アクティブな Input シングルトンインスタンスへのポインタを取得します。
		/// </summary>
		/// <returns>現在有効な Input インスタンスポインタ</returns>
		/// <remarks>
		/// <code>
		/// Engine::Input* input = Engine::Input::Get();
		/// </code>
		/// </remarks>
		static Input* Get() { return s_instance; }

		/// <summary>
		/// グローバルアクセスにより、フレーム開始時の打鍵フラグクリア処理を呼び出します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// Engine::Input::OnBeginFrame();
		/// </code>
		/// </remarks>
		static void OnBeginFrame();

		/// <summary>
		/// グローバルアクセスにより、SDLイベントを入力マネージャーへ転送します。
		/// </summary>
		/// <param name="e">受信した SDL_Event</param>
		/// <remarks>
		/// <code>
		/// Engine::Input::OnProcessEvent(e);
		/// </code>
		/// </remarks>
		static void OnProcessEvent(const SDL_Event& e);

		/// <summary>
		/// グローバルアクセスにより、指定された物理キーが押されている最中かを判定します。
		/// </summary>
		/// <param name="scancode">判定対象のキー位置を表す SDL_Scancode</param>
		/// <returns>キーが押されている場合は true、離されている場合は false</returns>
		/// <remarks>
		/// <code>
		/// if (Engine::Input::GetKey(SDL_SCANCODE_W)) {
		///     // 前進
		/// }
		/// </code>
		/// </remarks>
		static bool GetKey(SDL_Scancode scancode);

		/// <summary>
		/// グローバルアクセスにより、指定されたキーが今フレームで新しく押された瞬間かを判定します。
		/// </summary>
		/// <param name="keycode">判定対象のキーを表す SDL_Keycode</param>
		/// <returns>今フレームで押された瞬間であれば true、押し続けや離されている場合は false</returns>
		/// <remarks>
		/// <code>
		/// if (Engine::Input::GetKeyDown(SDLK_SPACE)) {
		///     // ジャンプ
		/// }
		/// </code>
		/// </remarks>
		static bool GetKeyDown(SDL_Keycode keycode);

	private:
		/// <summary>フレーム内で新しく押されたキーの打鍵フラグマップ（キーリピート除外）</summary>
		std::unordered_map<SDL_Keycode, bool> m_key_downs;

		/// <summary>SDLが管理するキーボード配列全体のリアルタイム押下状態バッファへのポインタ</summary>
		const Uint8* m_keyboard_state = nullptr;

		/// <summary>シングルトンアクセス用の静的インスタンス参照ポインタ</summary>
		static Input* s_instance;
	};

}
