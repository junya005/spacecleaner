#include "Input.h"

namespace Engine
{
	Input* Input::s_instance = nullptr;

	Input::Input() : m_keyboard_state(nullptr) {
		m_keyboard_state = SDL_GetKeyboardState(nullptr);
		s_instance = this;
	}

	Input::~Input() {
		if (s_instance == this) {
			s_instance = nullptr;
		}
	}

	void Input::Initialize() {
		static Input defaultInput;
		s_instance = &defaultInput;
	}

	void Input::BeginFrame() {
		m_key_downs.clear();
	}

	void Input::ProcessEvent(const SDL_Event& e) {
		if (e.type == SDL_KEYDOWN) {
			if (e.key.repeat == 0) {
				m_key_downs[e.key.keysym.sym] = true;
			}
		}
	}

	bool Input::IsKey(SDL_Scancode scancode) const {
		if (m_keyboard_state) {
			return m_keyboard_state[scancode] != 0;
		}
		return false;
	}

	bool Input::IsKeyDown(SDL_Keycode keycode) const {
		auto it = m_key_downs.find(keycode);
		if (it != m_key_downs.end()) {
			return it->second;
		}
		return false;
	}

	void Input::OnBeginFrame() {
		if (s_instance) {
			s_instance->BeginFrame();
		}
	}

	void Input::OnProcessEvent(const SDL_Event& e) {
		if (s_instance) {
			s_instance->ProcessEvent(e);
		}
	}

	bool Input::GetKey(SDL_Scancode scancode) {
		if (s_instance) {
			return s_instance->IsKey(scancode);
		}
		const Uint8* state = SDL_GetKeyboardState(nullptr);
		return state ? (state[scancode] != 0) : false;
	}

	bool Input::GetKeyDown(SDL_Keycode keycode) {
		if (s_instance) {
			return s_instance->IsKeyDown(keycode);
		}
		return false;
	}
}
