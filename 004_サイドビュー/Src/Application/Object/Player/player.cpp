#include "player.h"
#include "../../Scene/SceneManager.h"

void Player::Init()
{
	m_polygon.SetMaterial("Asset/Textures/char.png");
	m_polygon.SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
	// 画面分割
	m_polygon.SetSplit(6,6);
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
	m_pos = { -55,0,0 };
}

void Player::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(m_polygon, m_mWorld);
}

void Player::DrawLit()
{
	static float d = 0.0f;
	d += 0.01f;
	if (d > 1)d = 0;
	KdShaderManager::Instance().m_StandardShader.SetDissolve(d);
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(m_polygon,m_mWorld);
}

void Player::Update()
{
	// アニメーション
	static float anime = NULL;
	int Run[] = { 24,25,24,26 };
	anime += 0.1f;
	if (anime >= 4)anime = NULL;
	m_polygon.SetUVRect(Run[static_cast<int>(anime)]);
	//m_polygon.SetUVRect(32);

	// 移動
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_pos.x -= 0.1f;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_pos.x += 0.1f;
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		m_pos.y += 0.1f;
		m_gravity = 0;
	}
	//重力キャラに反映
	m_pos.y -= m_gravity;
	
	m_gravity += 0.005f;

	m_mWorld = Math::Matrix::CreateTranslation(m_pos);
}

void Player::PostUpdate()
{
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
	m_pDebugWire->AddDebugLine(ray.m_pos,ray.m_dir,ray.m_range,kGreenColor);
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
	Math::Vector3 pos = { m_pos.x,m_pos.y+0.5f,m_pos.z };
	sphere.m_sphere.Center = pos;
	// 球の半径
	sphere.m_sphere.Radius = 0.3f;
	// 当たり判定をしたいタイプ
	sphere.m_type = KdCollider::Type::TypeGround | KdCollider::Type::TypeBump;
	//デバック表示
	m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center,sphere.m_sphere.Radius);
	// 球に当たった情報を格納するリスト
	std::list<KdCollider::CollisionResult> retSphereList;
	// 当たり判定
	for (decltype(auto) obj : SceneManager::Instance().GetObjList())
	{
		if (obj->Intersects(sphere, &retSphereList))
		{
			if (obj->GetObjectType() == KdGameObject::ObjectType::Enemy)
			{
				obj->OnHit();
				//OnHit();
			}
		}
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

void Player::OnHit()
{
	SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
	// m_isExpired = true;
}
