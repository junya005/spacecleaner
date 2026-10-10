#pragma once
#include <cstdint>

namespace Engine {

	/// <summary>
	/// SDL高精度タイマーを利用して前フレームからの経過時間（デルタタイム）、起動からの総経過時間、累計フレーム数を計測・管理するための静的クラスです。
	/// </summary>
	/// <remarks>
	/// フレームレートに依存しない物理移動やアニメーション更新、時間経過イベントなどの実装に利用します。
	/// デバッグ停止や重い処理によるデルタタイムの極端な跳ね上がりを防ぐクランプ機能（SetMaxDeltaTime）も備えています。
	/// <code>
	/// // キャラクターを毎秒 200 ピクセル移動させる例
	/// float dt = Engine::Time::GetDeltaTime();
	/// playerX += 200.0f * dt;
	/// 
	/// // 起動からの経過秒数を使った周期アニメーションの例
	/// float wave = std::sin(Engine::Time::GetTotalTime() * 3.0f);
	/// </code>
	/// </remarks>
	class Time {
	public:
		/// <summary>
		/// タイマーの基準カウンタを取得し、時間計測状態を初期化するために存在します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// Engine::Time::Initialize();
		/// </code>
		/// </remarks>
		static void Initialize();

		/// <summary>
		/// 現在の高精度ティックを取得し、前フレームからの経過時間（デルタタイム）および総経過時間を更新するために存在します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// // 毎フレームのループ先頭で呼び出す
		/// Engine::Time::Update();
		/// </code>
		/// </remarks>
		static void Update();

		/// <summary>
		/// 前フレームから現在フレームまでに経過した時間（秒単位）を取得します。
		/// </summary>
		/// <returns>経過時間（秒単位の float 値、クランプ済み）</returns>
		/// <remarks>
		/// <code>
		/// float dt = Engine::Time::GetDeltaTime();
		/// position += velocity * dt;
		/// </code>
		/// </remarks>
		static float GetDeltaTime();

		/// <summary>
		/// アプリケーション起動（Initialize呼出）時からの累積経過時間（秒単位）を取得します。
		/// </summary>
		/// <returns>総経過時間（秒単位の float 値）</returns>
		/// <remarks>
		/// <code>
		/// float elapsedSeconds = Engine::Time::GetTotalTime();
		/// </code>
		/// </remarks>
		static float GetTotalTime();

		/// <summary>
		/// アプリケーション起動後に経過した総フレームカウント数を取得します。
		/// </summary>
		/// <returns>総描画フレーム数</returns>
		/// <remarks>
		/// <code>
		/// uint64_t frames = Engine::Time::GetFrameCount();
		/// </code>
		/// </remarks>
		static uint64_t GetFrameCount();

		/// <summary>
		/// 1フレームあたりの最大許容デルタタイムを設定し、処理落ち時やデバッグブレーク時のオブジェクト突き抜け・暴走を防ぐために存在します。
		/// </summary>
		/// <param name="maxDeltaTime">最大許容秒数（例: 0.1f で最低10FPS相当の時間に制限）</param>
		/// <remarks>
		/// <code>
		/// // 最大デルタタイムを0.05秒（20FPS相当）に制限する例
		/// Engine::Time::SetMaxDeltaTime(0.05f);
		/// </code>
		/// </remarks>
		static void SetMaxDeltaTime(float maxDeltaTime);

		/// <summary>
		/// 現在設定されている最大許容デルタタイム（秒）を取得します。
		/// </summary>
		/// <returns>最大許容デルタタイム（秒）</returns>
		/// <remarks>
		/// <code>
		/// float maxDt = Engine::Time::GetMaxDeltaTime();
		/// </code>
		/// </remarks>
		static float GetMaxDeltaTime();

	private:
		/// <summary>前フレーム更新時の SDL パフォーマンスカウンタ値</summary>
		static uint64_t s_lastTicks;

		/// <summary>直近フレームの経過時間（秒）</summary>
		static float s_deltaTime;

		/// <summary>アプリケーション起動からの累計経過時間（秒）</summary>
		static float s_totalTime;

		/// <summary>アプリケーション起動からの累計フレーム数</summary>
		static uint64_t s_frameCount;

		/// <summary>デルタタイムの最大制限値（秒）</summary>
		static float s_maxDeltaTime;
	};

}
