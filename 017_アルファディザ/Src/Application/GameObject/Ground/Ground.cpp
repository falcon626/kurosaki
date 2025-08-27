#include "Ground.h"

void Ground::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelData>();
		m_spModelMap = std::make_shared<KdModelData>();
		m_spModel->Load("Asset/Models/Ground/Ground.gltf");
		m_spModelMap->Load("Asset/Models/map/map.gltf");

		m_pCollider = std::make_unique<KdCollider>();
		m_pCollider->RegisterCollisionShape("Ground", m_spModel, KdCollider::TypeGround);

		SetScale(100.0f);
	}
}

void Ground::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModelMap, Math::Matrix::Identity);
}

void Ground::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModelMap, Math::Matrix::Identity);
}