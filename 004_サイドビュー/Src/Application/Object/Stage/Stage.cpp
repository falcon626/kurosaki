#include"Stage.h"

void Stage::Init()
{
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/SideViewMap/SideViewMap.gltf");
	//SetScale({ 5, 1, 1 });
	static float z =NULL;
	auto traMat = Math::Matrix::CreateTranslation(0, -2, z*3);
	z++;
	auto scaMat = Math::Matrix::CreateScale(5,1,1);
	m_mWorld = scaMat * traMat;
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("StageCollision", m_model, KdCollider::Type::TypeGround);
}

void Stage::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model,m_mWorld);
}