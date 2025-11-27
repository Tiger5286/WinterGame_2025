#pragma once

/// <summary>
/// アプリケーション全体を管理するシングルトンクラス
/// </summary>
class Application
{
private:
	Application();
	Application(const Application& app) = delete;
	void operator=(const Application& app) = delete;
public:
	~Application();
	static Application& GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <returns>初期化成功:true / 初期化失敗:false</returns>
	bool Init();

	/// <summary>
	/// アプリケーションをスタートする
	/// ゲームループを内包する
	/// </summary>
	void Run();

	/// <summary>
	/// アプリケーション終了処理
	/// </summary>
	void Terminate();
};

