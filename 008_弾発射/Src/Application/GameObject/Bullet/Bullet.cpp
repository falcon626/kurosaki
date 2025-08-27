#include "Bullet.h"

void Bullet::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelData>();
		m_spModel->Load("Asset/Models/Bullet/Bullet.gltf");
	}
}

void Bullet::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void Bullet::DrawBright()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void Bullet::Update()
{
	m_pos += m_dir * m_speed;
	auto tarans = Math::Matrix::CreateTranslation(m_pos);

	m_mWorld = tarans;

	--m_lifeSpan;
	if (m_lifeSpan < 0) m_isExpired = true;
}

void Bullet::SetParam(const Math::Vector3& pos, const Math::Vector3& dir, const float speed, int life)
{
	m_pos = pos + Math::Vector3(0, 2, 0);
	m_dir = dir;
	m_speed    = speed;
	m_lifeSpan = life;
}