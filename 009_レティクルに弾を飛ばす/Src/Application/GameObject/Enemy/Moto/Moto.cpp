#include "Moto.h"

void Moto::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void Moto::Update()
{
	auto targetPos{ Math::Vector3::Zero };
	if (!m_wpTarget.expired())
	{
		targetPos = m_wpTarget.lock()->GetPos();
	}
	// 自分から見て、Targetの方向を算出
	auto v{ targetPos - m_pos };
	v.Normalize();

	m_pos += v * 0.06f;

	auto mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = mTrans;
}

void Moto::OnHit()
{
	m_isExpired = true;
}

void Moto::Init()
{
	m_spModel = std::make_shared<KdModelData>();
	m_spModel->Load("Asset/Models/enemy/moto/moto.gltf");
	SetPos({10,0,10});

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("EnemyMoto", m_spModel, KdCollider::TypeDamage);
}