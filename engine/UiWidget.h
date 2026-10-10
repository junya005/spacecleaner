#pragma once
#include <imgui.h>

namespace Engine {

	/// <summary>
	/// 指定座標およびサイズで ImGui ボタンプリミティブを配置し、クリックイベントを監視・検知するために存在するウィジェットクラスです。
	/// </summary>
	/// <remarks>
	/// 毎フレームの OnImGuiRender() や Render() 内で Listen() を呼び出すことで、描画と押下検知を同時に行います。
	/// <code>
	/// Engine::Button startButton("Start", ImVec2(500.0f, 300.0f), ImVec2(150.0f, 50.0f));
	/// 
	/// void OnImGuiRender() override {
	///     if (startButton.Listen()) {
	///         // ボタンがクリックされた時の処理
	///     }
	/// }
	/// </code>
	/// </remarks>
	class Button {
	public:
		/// <summary>
		/// ボタンの表示ラベル、画面上の絶対配置座標、およびボタンサイズを指定してインスタンスを構築します。
		/// </summary>
		/// <param name="label">ボタン上に表示されるテキスト（ImGuiの識別子兼用）</param>
		/// <param name="position">ボタンを配置する左上座標（ImVec2）</param>
		/// <param name="size">ボタンの横幅と縦幅（ImVec2）</param>
		/// <remarks>
		/// <code>
		/// Engine::Button myBtn("Click Me", ImVec2(100.0f, 200.0f), ImVec2(120.0f, 40.0f));
		/// </code>
		/// </remarks>
		Button(const char* label, const ImVec2& position, const ImVec2& size) :
			m_label(label), m_position(position), m_size(size), m_alignment(0.5f, 0.5f) {
		}

		/// <summary>
		/// ボタンインスタンスを破棄します。
		/// </summary>
		~Button() {}

		/// <summary>
		/// ImGui カーソルを設定してボタンを描画し、現在のフレームでユーザーにクリックされたかを判定するために存在します。
		/// </summary>
		/// <returns>ボタンが左クリックされた瞬間のフレームであれば true、それ以外は false</returns>
		/// <remarks>
		/// <code>
		/// if (button.Listen()) {
		///     // 決定処理
		/// }
		/// </code>
		/// </remarks>
		bool Listen();

	private:
		/// <summary>ImGui ウィンドウ内におけるボタンの配置左上座標（ピクセル単位）</summary>
		ImVec2 m_position;

		/// <summary>ボタンの描画横幅および縦幅サイズ（ピクセル単位）</summary>
		ImVec2 m_size;

		/// <summary>ボタン内部のテキスト位置揃え基準点（0.5, 0.5 で中央揃え）</summary>
		ImVec2 m_alignment;

		/// <summary>ボタンに表示されるテキストラベルポインタ</summary>
		const char* m_label = nullptr;
	};
}
