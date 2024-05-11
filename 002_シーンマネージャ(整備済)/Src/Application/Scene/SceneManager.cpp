#include "SceneManager.h"
#include "TitleScene/TitleScene.h"
#include "GameScene/GameScene.h"
#include "BaseScene/BaseScene.h"


void SceneManager::PreUpdate()
{
	// Scene切替
	if (m_currentSceneType != m_nextSceneType)
	{
		ChangeScene(m_nextSceneType);
	}
}

void SceneManager::Update()
{
	m_currentScene->Update();
}

void SceneManager::Draw()
{
	m_currentScene->Draw();
}

void SceneManager::Init()
{
	m_currentScene = std::make_shared<TitleScene>();
}

void SceneManager::Release()
{
}

void SceneManager::ChangeScene(SceneType _sceneType)
{
	// 次のシーンを作成し、現在のシーンにする
	switch (_sceneType)
	{
	case SceneManager::Title:
		m_currentScene = std::make_shared<TitleScene>();
		break;
	case SceneManager::Game:
		m_currentScene = std::make_shared<GameScene>();
		break;
	case SceneManager::Result:
		break;
	default:
		break;
	}

	// シーン管理を更新
	m_currentSceneType = _sceneType;
}
