#include "Time.h"
#include <SDL2/SDL.h>
#include <algorithm>

namespace Engine {

	uint64_t Time::s_lastTicks = 0;
	float Time::s_deltaTime = 0.0f;
	float Time::s_totalTime = 0.0f;
	uint64_t Time::s_frameCount = 0;
	float Time::s_maxDeltaTime = 0.1f; // デフォルトで最大0.1秒（10FPS相当）に制限

	void Time::Initialize() {
		s_lastTicks = SDL_GetPerformanceCounter();
		s_deltaTime = 0.0f;
		s_totalTime = 0.0f;
		s_frameCount = 0;
	}

	void Time::Update() {
		uint64_t currentTicks = SDL_GetPerformanceCounter();
		uint64_t frequency = SDL_GetPerformanceFrequency();

		if (frequency == 0) {
			s_deltaTime = 0.0f;
			return;
		}

		float rawDelta = static_cast<float>(currentTicks - s_lastTicks) / static_cast<float>(frequency);
		s_lastTicks = currentTicks;

		// デルタタイムのクランプ（デバッグ停止後や極端な負荷時のスパイク防止）
		s_deltaTime = std::min(rawDelta, s_maxDeltaTime);
		s_totalTime += s_deltaTime;
		s_frameCount++;
	}

	float Time::GetDeltaTime() {
		return s_deltaTime;
	}

	float Time::GetTotalTime() {
		return s_totalTime;
	}

	uint64_t Time::GetFrameCount() {
		return s_frameCount;
	}

	void Time::SetMaxDeltaTime(float maxDeltaTime) {
		s_maxDeltaTime = maxDeltaTime;
	}

	float Time::GetMaxDeltaTime() {
		return s_maxDeltaTime;
	}

}
