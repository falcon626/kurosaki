#include "GameScene.h"

void GameScene::Draw()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_playerTex, m_pos.x, m_pos.y);
}

void GameScene::Update()
{
	if (GetAsyncKeyState(VK_UP) & 0x8000)m_pos.y += 5.f;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)m_pos.y -= 5.f;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)m_pos.x -= 5.f;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)m_pos.x += 5.f;
}

void GameScene::Init()
{
	m_playerTex.Load("Asset/Textures/player.png");
}

void GameScene::Release()
{
	m_playerTex.Release();
}
