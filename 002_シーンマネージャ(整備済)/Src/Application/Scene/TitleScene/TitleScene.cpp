#include"TitleScene.h"
#include"../SceneManager.h"

void TitleScene::Init()
{
	//===================================================================
	// title初期化
	//===================================================================
	m_titleTex.Load("Asset/Textures/title.png");
	m_startTex.Load("Asset/Textures/start.png");
}

void TitleScene::Release()
{
	m_titleTex.Release();
	m_startTex.Release();
}

void TitleScene::Draw()
{
	// 2D描画はSetMatrixはしなくていい
	// ※回転させたい場合は必要
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_titleTex, 0, 0);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_startTex, 0, -100, nullptr, &m_color);
}

void TitleScene::Update()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::GetInstance().SetNextScene(SceneManager::Game);
	}
	m_alpha += m_alpSp;
	if (m_alpha > 1)
	{
		m_alpha = 1;
		m_alpSp *= -1;
	}
	if (m_alpha < 0)
	{
		m_alpha = 0;
		m_alpSp *= -1;
	}
	m_color = { 1,1,1,m_alpha };
}
