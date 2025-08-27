#include "Yama.h"

void Yama::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void Yama::Update()
{
	auto targetPos{ Math::Vector3::Zero };
	if (!m_wpTarget.expired())
	{
		targetPos = m_wpTarget.lock()->GetPos();
	}
	// 自分から見て、Targetの方向を算出
	auto v{ targetPos - m_pos };
	v.Normalize();

	if (m_isJump)
	{
		m_jump += m_jumpPow;
		m_pos.y += m_jump;
		if (m_pos.y >= 5.0f)
		{
			m_jump = 0;
			m_isJump = false;
		}
	}
	else
	{
		m_jump += m_jumpPow;
		m_pos.y -= m_jump;
	}

	if (m_pos.y <= 0)
	{
		m_pos.y = 0;
		m_isJump = true;
		m_jump = 0;
	}
	m_pos += v * 0.05f;

	auto mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = mTrans;
}

void Yama::OnHit()
{
	m_isExpired = true;
}

void Yama::Init()
{
	m_spModel = std::make_shared<KdModelData>();
	m_spModel->Load("Asset/Models/enemy/yama/yama.gltf");
	SetPos({ -10,0,10 });

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("EnemyYama", m_spModel, KdCollider::TypeDamage);

	m_jump = 0;
	m_jumpPow = 0.02f;
	m_isJump = false;
}