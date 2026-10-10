#include "SceneManager.h"

namespace Engine {

	std::unique_ptr<Scene> SceneManager::s_currentScene = nullptr;
	std::unique_ptr<Scene> SceneManager::s_nextScene = nullptr;
	bool SceneManager::s_hasPendingTransition = false;

	void SceneManager::Initialize() {
		s_currentScene.reset();
		s_nextScene.reset();
		s_hasPendingTransition = false;
	}

	void SceneManager::Shutdown() {
		if (s_currentScene) {
			s_currentScene->OnExit();
			s_currentScene.reset();
		}
		s_nextScene.reset();
		s_hasPendingTransition = false;
	}

	void SceneManager::RequestSceneChange(std::unique_ptr<Scene> newScene) {
		s_nextScene = std::move(newScene);
		s_hasPendingTransition = true;
	}

	void SceneManager::ProcessTransition() {
		if (!s_hasPendingTransition) {
			return;
		}

		if (s_currentScene) {
			s_currentScene->OnExit();
		}

		s_currentScene = std::move(s_nextScene);
		s_hasPendingTransition = false;

		if (s_currentScene) {
			s_currentScene->OnEnter();
		}
	}

	void SceneManager::Update(float deltaTime) {
		// 遅延シーン遷移を適用
		ProcessTransition();

		if (s_currentScene) {
			s_currentScene->OnUpdate(deltaTime);
		}
	}

	void SceneManager::Render() {
		if (s_currentScene) {
			s_currentScene->OnRender();
		}
	}

	void SceneManager::OnImGuiRender() {
		if (s_currentScene) {
			s_currentScene->OnImGuiRender();
		}
	}

	Scene* SceneManager::GetCurrentScene() {
		return s_currentScene.get();
	}

}
