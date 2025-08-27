#include "Fly.h"

void Fly::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/SideViewMap/CannonCOL.gltf");
	//SetScale({ 5, 1, 1 });
	static float z = NULL;
	auto traMat = Math::Matrix::CreateTranslation(0, -2, z*3);
	z++;
	auto rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(90));
	m_mWorld = rotMat * traMat;
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("StageCollision", m_model, KdCollider::Type::TypeGround);
}

void Fly::DrawLit()
{
	// KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void Fly::DrawBright()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
