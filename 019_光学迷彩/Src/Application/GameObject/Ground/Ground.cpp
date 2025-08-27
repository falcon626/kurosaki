#include "Ground.h"
#include "../../Scene/SceneManager.h"

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
}

void Ground::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void Ground::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void Ground::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(SceneManager::Instance().GetRenderTargetTex(), 450, 250, 1280 / 4, 720 / 4);
}
