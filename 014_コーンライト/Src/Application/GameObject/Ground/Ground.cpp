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
}

void Ground::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void Ground::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);

	auto rotM{ Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(90)) * Math::Matrix::CreateScale(100.0f)};
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, rotM);
}