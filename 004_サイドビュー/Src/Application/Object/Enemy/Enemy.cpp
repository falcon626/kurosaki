#include "Enemy.h"

#include "../../Scene/SceneManager.h"

void Enemy::Init()
{
	m_objectType = KdGameObject::Enemy;
	m_polygon.SetMaterial("Asset/Textures/char.png");
	m_polygon.SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
	// 画面分割
	m_polygon.SetSplit(6, 6);
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
	static float posX = NULL;
	m_pos = { -20+posX,0,0 };
	posX++;
	// 当たり判定
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("EnemyCollision",{0,0.5f,0}, 0.2f, KdCollider::Type::TypeBump);
	//													  ↑座標(ローカル座標)
}

void Enemy::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(m_polygon, m_mWorld);
}

void Enemy::DrawLit()
{
	// ポイントライト（点光源）
	auto tempPos = m_pos + Math::Vector3{ 0,0.5f,0 };
	KdShaderManager::Instance().WorkAmbientController().AddPointLight({ 3,0,0 }, 5, m_pos + tempPos);
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(m_polygon, m_mWorld);
}

void Enemy::Update()
{
	// アニメーション
	static float anime = NULL;
	int Walk[] = { 3,4,3,5 };
	anime += 0.05f;
	if (anime >= 4)anime = NULL;
	m_polygon.SetUVRect(Walk[static_cast<int>(anime)]);

	// 移動
	static float move = 0.025f;
	static int frame;
	static float scalX = 1.0f;
	if (frame % 200 == 0) {
		move *= -1;
		scalX *= -1;
	}
	m_pos.x += move;
	++frame;
	if (INT_MAX < frame) frame -= INT_MAX;
	//重力キャラに反映
	m_pos.y -= m_gravity;

	m_gravity += 0.005f;

	auto scalMat = Math::Matrix::CreateScale({scalX,1,1});
	m_mWorld = scalMat * Math::Matrix::CreateTranslation(m_pos);
}

void Enemy::PostUpdate()
{
	// 敵当たり判定可視化
	m_pDebugWire->AddDebugSphere({m_pos.x,m_pos.y+0.5f,m_pos.z}, 0.2f, kGreenColor);
	// // // //
	//レイ判定
	// // // //
	KdCollider::RayInfo ray;
	// レイ発射位置
	ray.m_pos = m_pos; // 足元
	// レイ発射方法
	ray.m_dir = Math::Vector3::Down;
	// 少し高いところから飛ばす
	ray.m_pos.y += 0.1f;
	// 段差の許容範囲
	float enableStepHigh = 0.2f;
	ray.m_pos.y += enableStepHigh;
	// レイ長さ
	ray.m_range = m_gravity + enableStepHigh;
	// 当たり判定をしたいタイプを設定
	ray.m_type = KdCollider::Type::TypeGround;
	// デバック表示
	m_pDebugWire->AddDebugLine(ray.m_pos, ray.m_dir, ray.m_range, kGreenColor);
	// レイに当たったオブジェクト情報のリスト
	std::list<KdCollider::CollisionResult> retRayList;
	//レイと当たり判定
	for (decltype(auto) obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects(ray, &retRayList);
	}
	// レイに当たった一番近いオブジェクトを検出
	float maxOverLap = 0; // はみ出たレイの長さ
	Math::Vector3 hitPos; // レイが遮断された座標
	bool isHit = false;    // 当たっていたらtrue
	for (decltype(auto) ret : retRayList)
	{
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			hitPos = ret.m_hitPos;
			isHit = true;
		}
	}
	if (isHit)
	{
		m_pos = hitPos + Math::Vector3(0, -0.1f, 0);
		m_gravity = 0;
	}

	// // // //
	// 球判定
	// // // //

	// 球判定用の変数作成
	KdCollider::SphereInfo sphere;
	Math::Vector3 pos = { m_pos.x,m_pos.y + 0.5f,m_pos.z };
	sphere.m_sphere.Center = pos;
	// 球の半径
	sphere.m_sphere.Radius = 0.3f;
	// 当たり判定をしたいタイプ
	sphere.m_type = KdCollider::Type::TypeGround | KdCollider::Type::TypeBump;
	//デバック表示
	m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius);
	// 球に当たった情報を格納するリスト
	std::list<KdCollider::CollisionResult> retSphereList;
	// 当たり判定
	for (decltype(auto) obj : SceneManager::Instance().GetObjList())
	{
		if (obj.get() == this) continue;
		obj->Intersects(sphere, &retSphereList);
	}
	// 球に当たった一番近いオブジェクトを検出
	maxOverLap = 0.0f;
	Math::Vector3 hitDir; // 当たった方向
	isHit = false;    // 当たっていたらtrue
	for (decltype(auto) ret : retSphereList)
	{
		//球にめり込んでオーバーした長さが一番長いものを探す
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			hitDir = ret.m_hitDir;
			isHit = true;
		}
	}
	if (isHit)
	{
		// zへの押し戻し無効
		hitDir.z = 0;
		// 正規化(長さを1にする)
		// 方向は絶対長さ１
		hitDir.Normalize();

		m_pos += hitDir * maxOverLap;
	}
}

void Enemy::OnHit()
{
	m_isExpired = true;
}
