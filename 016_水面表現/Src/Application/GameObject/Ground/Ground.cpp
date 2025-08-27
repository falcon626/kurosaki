#include "Ground.h"

void Ground::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelData>();
		m_spModel->Load("Asset/Models/Ground/Ground.gltf");

		m_pCollider = std::make_unique<KdCollider>();
		m_pCollider->RegisterCollisionShape("Ground", m_spModel, KdCollider::TypeGround);

		SetScale(100.0f);
	}

	if (!m_spTex)
	{
		// 水面表現２テクスチャを読み込む
		m_spTex = std::make_shared<KdTexture>("Asset/Textures/waterTEST.png");

		// 水面表現 テクスチャ転送
		KdShaderManager::Instance().m_StandardShader.SetNormalTexture(*m_spTex);
	}
}

void Ground::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void Ground::DrawLit()
{
	static Math::Vector2 offset{ 0,0 };
	offset.x += 0.0001;
	offset.y += 0.0001;
	if (offset.x > 1)
		offset.x -= 1;
	if (offset.y > 1)
		offset.y -= 1;
	KdShaderManager::Instance().m_StandardShader.SetWaterUVOffset(offset);
	// 板ポリに貼るテクスチャの数を増やす
	KdShaderManager::Instance().m_StandardShader.SetUVTiling({ 10,10 });

	KdShaderManager::Instance().m_StandardShader.SetWaterEnable(true);
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
	KdShaderManager::Instance().m_StandardShader.SetWaterEnable(false);
}