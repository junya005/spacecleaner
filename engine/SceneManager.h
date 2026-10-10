#pragma once
#include "Scene.h"
#include <memory>

namespace Engine {

	/// <summary>
	/// シーンインスタンスの生成・破棄、およびフレームの安全なタイミングで切り替えを行う「遅延シーン遷移」を管理するための静的クラスです。
	/// </summary>
	/// <remarks>
	/// 更新中や描画中のシーンがその場で破棄されてダングリングポインタやクラッシュが発生するのを防ぐため、
	/// ChangeScene リクエストは一度保留され、次フレーム開始時に旧シーンの OnExit() と新シーンの OnEnter() が順序通り実行されます。
	/// <code>
	/// // テンプレート引数で新しいシーンを型安全にリクエストする例
	/// Engine::SceneManager::ChangeScene<BattleScene>(playerData, enemyId);
	/// 
	/// // 既存のインスタンスポインタを渡す例
	/// auto newScene = std::make_unique<ResultScene>();
	/// Engine::SceneManager::RequestSceneChange(std::move(newScene));
	/// </code>
	/// </remarks>
	class SceneManager {
	public:
		/// <summary>
		/// シーン管理状態を初期化し、アクティブなシーンと遷移リクエストをクリアするために存在します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// Engine::SceneManager::Initialize();
		/// </code>
		/// </remarks>
		static void Initialize();

		/// <summary>
		/// 現在アクティブなシーンの OnExit() を呼び出して安全に破棄し、保留中の遷移リクエストを破棄するために存在します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// Engine::SceneManager::Shutdown();
		/// </code>
		/// </remarks>
		static void Shutdown();

		/// <summary>
		/// 保留中のシーン遷移が存在すれば適用（旧シーン OnExit → 新シーン OnEnter）し、現在アクティブなシーンの OnUpdate(deltaTime) を実行するために存在します。
		/// </summary>
		/// <param name="deltaTime">前フレームからの経過時間（秒単位）</param>
		/// <remarks>
		/// <code>
		/// Engine::SceneManager::Update(deltaTime);
		/// </code>
		/// </remarks>
		static void Update(float deltaTime);

		/// <summary>
		/// 現在アクティブなシーンの OnRender() を呼び出し、OpenGL による直接描画を実行するために存在します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// Engine::SceneManager::Render();
		/// </code>
		/// </remarks>
		static void Render();

		/// <summary>
		/// 現在アクティブなシーンの OnImGuiRender() を呼び出し、ImGui によるUI描画を実行するために存在します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// Engine::SceneManager::OnImGuiRender();
		/// </code>
		/// </remarks>
		static void OnImGuiRender();

		/// <summary>
		/// 新しいシーンクラスの型とコンストラクタ引数を指定し、次フレームで安全にシーンを切り替えるようリクエストするために存在します。
		/// </summary>
		/// <typeparam name="T">遷移先となる Engine::Scene 派生クラスの型</typeparam>
		/// <typeparam name="...Args">遷移先シーンのコンストラクタに渡す引数の型パック</typeparam>
		/// <param name="...args">遷移先シーンのコンストラクタに転送される可変長引数</param>
		/// <remarks>
		/// <code>
		/// // 引数なしのシーン遷移
		/// Engine::SceneManager::ChangeScene<TitleScene>();
		/// 
		/// // 引数ありのシーン遷移
		/// Engine::SceneManager::ChangeScene<ChoiceScene>(appContext, gameContext);
		/// </code>
		/// </remarks>
		template<typename T, typename... Args>
		static void ChangeScene(Args&&... args) {
			RequestSceneChange(std::make_unique<T>(std::forward<Args>(args)...));
		}

		/// <summary>
		/// 生成済みのシーンユニークポインタを渡し、次フレームで安全にシーンを切り替えるようリクエストするために存在します。
		/// </summary>
		/// <param name="newScene">新しくアクティブにするシーンインスタンスの unique_ptr</param>
		/// <remarks>
		/// <code>
		/// auto scene = std::make_unique<CustomScene>();
		/// Engine::SceneManager::RequestSceneChange(std::move(scene));
		/// </code>
		/// </remarks>
		static void RequestSceneChange(std::unique_ptr<Scene> newScene);

		/// <summary>
		/// 現在アクティブに動作しているシーンインスタンスへの生ポインタを取得します。
		/// </summary>
		/// <returns>現在有効な Scene ポインタ。未設定時は nullptr</returns>
		/// <remarks>
		/// <code>
		/// Engine::Scene* current = Engine::SceneManager::GetCurrentScene();
		/// </code>
		/// </remarks>
		static Scene* GetCurrentScene();

	private:
		/// <summary>現在アクティブに動作・更新されているシーンインスタンス</summary>
		static std::unique_ptr<Scene> s_currentScene;

		/// <summary>次フレーム開始時に切り替えるために保留されている次のシーンインスタンス</summary>
		static std::unique_ptr<Scene> s_nextScene;

		/// <summary>保留中のシーン遷移が存在するかを示すフラグ</summary>
		static bool s_hasPendingTransition;

		/// <summary>保留中の遷移リクエストを適用し、旧シーン OnExit → 新シーン OnEnter を順序実行します。</summary>
		static void ProcessTransition();
	};

}
