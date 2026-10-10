#include "UiWidget.h"

namespace Engine
{
	bool Button::Listen() {
		ImVec2 renderPos = ImVec2(m_position.x - m_size.x * m_alignment.x, m_position.y - m_size.y * m_alignment.y);

		ImGui::SetCursorPos(renderPos);
		if (ImGui::Button(m_label, m_size)) {
			return true; // ボタンが押された場合にtrueを返す
		}

		return false; // ボタンが押されなかった場合にfalseを返す
	}
}
