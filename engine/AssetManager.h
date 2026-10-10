#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <glad/glad.h>
#include <SDL_opengl.h>
#include <string>
#include <unordered_map>

namespace Engine {

	/// <summary>
	/// 画像ファイル（SDL_Surface）、フォントファイル（TTF_Font）、およびGPUテクスチャ（OpenGLテクスチャID）をキャッシュ・管理し、二重ロードやリソースリークを防ぐためのアセット管理クラスです。
	/// </summary>
	/// <remarks>
	/// 同一パスのリソースは初回ロード時にキャッシュされ、2回目以降はキャッシュから高速に返却されます。
	/// 静的アクセサを通じてゲーム中のあらゆる場所から簡潔にリソースをロード・利用できます。
	/// <code>
	/// // テクスチャのロードと取得の例
	/// GLuint playerTex = Engine::AssetManager::LoadTexture("assets/images/player.png");
	/// 
	/// // フォントのロードと取得の例
	/// TTF_Font* font = Engine::AssetManager::LoadFont("assets/fonts/font.ttf", 24);
	/// </code>
	/// </remarks>
	class AssetManager {
	public:
		/// <summary>
		/// AssetManager クラスのインスタンスを生成し、シングルトン参照ポインタを設定します。
		/// </summary>
		AssetManager();

		/// <summary>
		/// インスタンス破棄時にキャッシュされているすべての画像、フォント、テクスチャを解放します。
		/// </summary>
		~AssetManager();

		/// <summary>
		/// 静的なアセットマネージャーインスタンスを初期化し、利用可能にするために存在します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// Engine::AssetManager::Initialize();
		/// </code>
		/// </remarks>
		static void Initialize();

		/// <summary>
		/// キャッシュされているすべてのサーフェス、フォント、GPUテクスチャを一括解放し、マネージャーを終了するために存在します。
		/// </summary>
		/// <remarks>
		/// <code>
		/// Engine::AssetManager::Shutdown();
		/// </code>
		/// </remarks>
		static void Shutdown();

		/// <summary>
		/// 指定された画像ファイルパスから SDL_Surface を読み込み、キャッシュして返却します。
		/// </summary>
		/// <param name="file_path">読み込み対象の画像ファイルの相対または絶対パス</param>
		/// <returns>読み込まれた SDL_Surface へのポインタ。ロード失敗時は nullptr</returns>
		/// <remarks>
		/// <code>
		/// SDL_Surface* surface = manager.GetSurface("assets/icon.png");
		/// </code>
		/// </remarks>
		SDL_Surface* GetSurface(const std::string& file_path);

		/// <summary>
		/// 指定されたフォントファイルパスとサイズから TTF_Font を読み込み、キャッシュして返却します。
		/// </summary>
		/// <param name="file_path">フォントファイルのパス</param>
		/// <param name="font_size">フォントのポイントサイズ</param>
		/// <returns>読み込まれた TTF_Font へのポインタ。ロード失敗時は nullptr</returns>
		/// <remarks>
		/// <code>
		/// TTF_Font* font = manager.GetFont("assets/font.ttf", 16);
		/// </code>
		/// </remarks>
		TTF_Font* GetFont(const std::string& file_path, int font_size);

		/// <summary>
		/// 指定された画像ファイルを GPU テクスチャ（OpenGL GLuint）として生成・キャッシュして返却します。
		/// </summary>
		/// <param name="file_path">テクスチャ画像ファイルのパス</param>
		/// <returns>生成された OpenGL テクスチャID。ロード失敗時は 0</returns>
		/// <remarks>
		/// <code>
		/// GLuint texID = manager.GetTexture("assets/player.png");
		/// </code>
		/// </remarks>
		GLuint GetTexture(const std::string& file_path);

		/// <summary>
		/// 内部で保持している全画像サーフェス、全フォント、全GPUテクスチャを解放してキャッシュを空にします。
		/// </summary>
		/// <remarks>
		/// <code>
		/// manager.Cleanup();
		/// </code>
		/// </remarks>
		void Cleanup();

		// 静的アクセサ

		/// <summary>
		/// 現在アクティブな AssetManager シングルトンインスタンスへのポインタを取得します。
		/// </summary>
		/// <returns>現在有効な AssetManager インスタンスポインタ</returns>
		/// <remarks>
		/// <code>
		/// Engine::AssetManager* mgr = Engine::AssetManager::Get();
		/// </code>
		/// </remarks>
		static AssetManager* Get() { return s_instance; }

		/// <summary>
		/// グローバルアクセスにより、指定パスの画像ファイルを SDL_Surface としてキャッシュ読み込みします。
		/// </summary>
		/// <param name="file_path">読み込む画像のファイルパス</param>
		/// <returns>読み込まれた SDL_Surface ポインタ。失敗時は nullptr</returns>
		/// <remarks>
		/// <code>
		/// SDL_Surface* surf = Engine::AssetManager::LoadSurface("assets/bg.png");
		/// </code>
		/// </remarks>
		static SDL_Surface* LoadSurface(const std::string& file_path);

		/// <summary>
		/// グローバルアクセスにより、指定パスとサイズのフォントを TTF_Font としてキャッシュ読み込みします。
		/// </summary>
		/// <param name="file_path">フォントファイルのパス</param>
		/// <param name="font_size">フォントサイズ</param>
		/// <returns>読み込まれた TTF_Font ポインタ。失敗時は nullptr</returns>
		/// <remarks>
		/// <code>
		/// TTF_Font* font = Engine::AssetManager::LoadFont("assets/font.ttf", 32);
		/// </code>
		/// </remarks>
		static TTF_Font* LoadFont(const std::string& file_path, int font_size);

		/// <summary>
		/// グローバルアクセスにより、指定パスの画像から OpenGL テクスチャを生成・キャッシュして取得します。
		/// </summary>
		/// <param name="file_path">テクスチャ画像ファイルのパス</param>
		/// <returns>生成された OpenGL テクスチャID。失敗時は 0</returns>
		/// <remarks>
		/// <code>
		/// GLuint tex = Engine::AssetManager::LoadTexture("assets/enemy.png");
		/// </code>
		/// </remarks>
		static GLuint LoadTexture(const std::string& file_path);

	private:
		/// <summary>ファイルパスをキーとした SDL_Surface キャッシュテーブル</summary>
		std::unordered_map<std::string, SDL_Surface*> m_surfaces;

		/// <summary>「ファイルパス_サイズ」をキーとした TTF_Font キャッシュテーブル</summary>
		std::unordered_map<std::string, TTF_Font*> m_fonts;

		/// <summary>ファイルパスをキーとした OpenGL テクスチャID キャッシュテーブル</summary>
		std::unordered_map<std::string, GLuint> m_textures;

		/// <summary>シングルトンアクセス用の静的インスタンス参照ポインタ</summary>
		static AssetManager* s_instance;
	};

	/// <summary>
	/// SDL_Surface のピクセルデータを RGBA フォーマットに変換し、GPU上に OpenGL 2D テクスチャ（GLuint）を生成するために存在するヘルパー関数です。
	/// </summary>
	/// <param name="surface">テクスチャ化する元の SDL_Surface ポインタ</param>
	/// <returns>生成された OpenGL テクスチャ識別子（テクスチャID）。失敗時は 0</returns>
	/// <remarks>
	/// ピクセルアート（ドット絵）に適したニアレストネイバーフィルタ（GL_NEAREST）およびクランプ設定が施されます。
	/// <code>
	/// SDL_Surface* surf = IMG_Load("sample.png");
	/// GLuint texID = Engine::CreateTextureFromSurface(surf);
	/// SDL_FreeSurface(surf);
	/// </code>
	/// </remarks>
	inline GLuint CreateTextureFromSurface(SDL_Surface* surface) {
		if (!surface) {
			SDL_Log("Surface is null.");
			return 0;
		}

		SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(
			surface, 
			SDL_PIXELFORMAT_ABGR8888, 
			0
		);

		if (!formattedSurface) {
			SDL_Log("Failed to convert surface format: %s", SDL_GetError());
			return 0;
		}

		GLuint textureID = 0;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, formattedSurface->w, formattedSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, formattedSurface->pixels);
		glBindTexture(GL_TEXTURE_2D, 0);

		SDL_FreeSurface(formattedSurface);
		return textureID;
	}

}
