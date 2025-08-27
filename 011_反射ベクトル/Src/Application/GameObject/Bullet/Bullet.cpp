#include "Bullet.h"
#include "../../Scene/SceneManager.h"

void Bullet::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelData>();
		m_spModel->Load("Asset/Models/Bullet/Bullet.gltf");
	}

	m_tPoly.SetMaterial("Asset/Textures/Image20240909102819.png");
	//m_tPoly.SetLength(200);
}

void Bullet::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(m_tPoly);
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

	// トレイルポリゴン頂点追加
	m_tPoly.AddPoint(m_mWorld);
}

void Bullet::PostUpdate()
{
	// 早期リターン
	if (m_isExpired)return;

	// 当たり判定スフィア
	KdCollider::SphereInfo sphere{ KdCollider::TypeGround | KdCollider::TypeDamage,m_pos,0.5f };

	std::list<KdCollider::CollisionResult> retList;
	for (const auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects(sphere, &retList);
	}
	float overlap = 0;
	bool hit = false;
	Math::Vector3 normal;
	for (const auto& ret : retList)
	{
		if (overlap < ret.m_overlapDistance)
		{
			hit = true;
			normal = ret.m_hitNormal;
			overlap = ret.m_overlapDistance;
		}
	}
	// for文を抜けたら一番近くで当たっているオブジェクトの法線を所得している
	if (hit)
	{
		// 反射ベクトルを求める
		float a = -m_dir.Dot(normal);
		Math::Vector3 r = m_dir + 2 * a * normal;
		r.Normalize();
		m_dir = r;
	}
}

void Bullet::SetParam(const Math::Vector3& pos, const Math::Vector3& dir, const float speed, int life)
{
	m_pos = pos + Math::Vector3(0, 2, 0);
	m_dir = dir;
	m_speed    = speed;
	m_lifeSpan = life;
}