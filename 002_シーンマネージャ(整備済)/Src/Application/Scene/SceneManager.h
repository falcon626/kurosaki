#pragma once
// シーンマネージャは実行時には実体化している
// #SceneManager.hをインクルードすればどこからでもシーン切替可能!!

class BaseScene; // 前方宣言

class SceneManager
{
public:
	enum SceneType
	{
		Title,
		Game,
		Result
	};

	void PreUpdate();
	void Update();

	void Draw();

	// 次のシーンを予約するための関数
	void SetNextScene(SceneType _nextScene) { m_nextSceneType = _nextScene; }
	
	SceneType GetCurrentSceneType() { return m_currentSceneType; }

private:
	void Init();
	void Release();

	void ChangeScene(SceneType _sceneType);

	std::shared_ptr<BaseScene> m_currentScene;
	
	SceneType m_currentSceneType = SceneType::Title;

	SceneType m_nextSceneType = m_currentSceneType;

// シングルトンパターン
// そのクラスのインスタンスが1つしか生成されないことを保証するデザインパターン
// あまり多用しない！！
private:
	SceneManager() { Init(); }
	~SceneManager() { Release(); }
public:
	static auto& GetInstance(){ static SceneManager Instance; return Instance; }
};
