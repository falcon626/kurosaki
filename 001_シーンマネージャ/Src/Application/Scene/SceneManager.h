#pragma once
class BaseScene;
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
	
	SceneType GetNowScene() { return m_currentSceneType; }

private:
	void Init();
	void Release();

	void ChangeScene(SceneType _sceneType);

	std::shared_ptr<BaseScene> m_currentScene;
	
	SceneType m_currentSceneType = SceneType::Title;

	SceneType m_nextSceneType = m_currentSceneType;

// シングルトンパターン
private:
	SceneManager() { Init(); }
	~SceneManager() { Release(); }
public:
	static auto& GetInstance(){ static SceneManager Instance; return Instance; }
};
