#include "Tank.h"
#include "../../GameObject/Bullet/Bullet.h"
#include "../Camera/TPSCamera/TPSCamera.h"
#include "../../main.h"
#include "../../Scene/SceneManager.h"

void Tank::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelData>();
		m_spModel->Load("Asset/Models/tank/tank.gltf");
	}

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Tank::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void Tank::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void Tank::Update()
{
	auto camRotYMat{ Math::Matrix::Identity };
	if (!m_wpCamera.expired())
	{
		camRotYMat = m_wpCamera.lock()->GetRotationYMatrix();
	}

	float moveSpd = 0.1f;
	Math::Vector3 nowPos = m_mWorld.Translation();

	bool moveFlg(false);
	Math::Vector3 moveVec = Math::Vector3::Zero;
	if (GetAsyncKeyState('D')) { moveVec += Math::Vector3::TransformNormal({ 1,0,0 },  camRotYMat); moveFlg = true; }
	if (GetAsyncKeyState('A')) { moveVec += Math::Vector3::TransformNormal({ -1,0,0 }, camRotYMat); moveFlg = true; }
	if (GetAsyncKeyState('W')) { moveVec += Math::Vector3::TransformNormal({ 0,0,1 },  camRotYMat); moveFlg = true; }
	if (GetAsyncKeyState('S')) { moveVec += Math::Vector3::TransformNormal({ 0,0,-1 }, camRotYMat); moveFlg = true; }

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		auto bullet{ std::make_shared<Bullet>() };
		bullet->Init();

		// 発射方向
		Math::Vector3 dir{};
		// 発射位置
		Math::Vector3 pos{ m_pos };
		pos.y += 2.f;

		if (m_isTPS)
		{
			// ADS
			Math::Vector3 camPos{};
			float range = 0;
			if (!m_wpCamera.expired())
			{
				camPos = m_wpCamera.lock()->GetPos();
				m_wpCamera.lock()->WorkCamera()->GenerateRayInfoFromClientPos({ 640,360 }, camPos, dir, range);
			}

			// レイ判定
			KdCollider::RayInfo rayInfo;
			rayInfo.m_dir = dir;
			rayInfo.m_pos = camPos;
			rayInfo.m_range = range;
			rayInfo.m_type = KdCollider::TypeGround | KdCollider::TypeDamage;
			std::list<KdCollider::CollisionResult> result;
			for (const auto& obj: SceneManager::Instance().GetObjList())
			{
				if (obj->Intersects(rayInfo, &result))obj->OnHit();
			}
			float overlap = 0;
			Math::Vector3 hitPos{};
			for (const auto& re : result)
			{

				if (overlap < re.m_overlapDistance)
				{
					overlap = re.m_overlapDistance;
					hitPos  = re.m_hitPos;
					// 方向ベクトルを算出
					dir = hitPos - pos;
					dir.Normalize(); // 方向ベクトルは長さ１
				}
			}
		}
		else
		{
			// NADS
			dir = m_mWorld.Backward();
		}
		moveVec.Normalize();
		bullet->SetParam(pos, dir, 1, 1000);
		SceneManager::Instance().AddObject(bullet);
	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		m_isTPS = true;
	}
	else m_isTPS = false;

	auto mRot{ Math::Matrix::Identity };
	if (m_isTPS)
	{
		// TPS(背面Camera)
		moveVec.Normalize();

		mRot = camRotYMat;
	}
	else
	{
		// TPV
		m_isAtk = false;
		if (moveFlg)
		{
			m_isAtk = true;
			// 正規化
			moveVec.Normalize();

			// 今キャラが向いている方向
			auto mNowRot{ Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle)) };

			// 現在の方向
			auto nowVex{ Math::Vector3::TransformNormal(Math::Vector3(0, 0, 1), mNowRot) };

			// 向きたい方向
			auto toVec{ moveVec };
			toVec.Normalize();

			// 内積を使って回転する角度を求める
			// ベクトルA*ベクトルB*コサインΘ(ベクトルAとベクトルBのなす角)

			auto d{ nowVex.Dot(toVec) };

			// 丸め誤差の都合上「1」を超える可能性があるためクランプ(遮断)する
			// まるめ誤差…小数点以下を省略した際の誤差
			d = std::clamp(d, -1.0f, 1.0f);

			// d＝cosθ
			auto ang{ DirectX::XMConvertToDegrees(acos(d)) };

			// 内積から角度を求めて少しでも角度が変わったらゆっくり回転
			if (ang >= 0.1f)
			{
				if (ang > 5)
				{
					ang = 5.0f;
				}

				// 外積を求める(どっちに回転するのかを求める)
				auto c{ toVec.Cross(nowVex) };

				if (c.y >= NULL)
				{
					// 右回転
					m_angle -= ang;

					if (m_angle < 0)m_angle = 360.f;
				}
				else
				{
					// 左回転
					m_angle += ang;

					if (m_angle > 360)m_angle = 0.f;
				}
			}
		}
		mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle));
	}

	moveVec *= moveSpd;
	m_pos += moveVec;

	m_mWorld = mRot * Math::Matrix::CreateTranslation(m_pos);
}

void Tank::PostUpdate()
{
	if (!m_isAtk)return;

	KdCollider::SphereInfo sphereInfo;

	sphereInfo.m_sphere.Center = m_pos;
	sphereInfo.m_sphere.Center.y = 0.5f;

	sphereInfo.m_sphere.Radius = 2.f;
	sphereInfo.m_type = KdCollider::TypeDamage;

	for (decltype(auto) obj : SceneManager::Instance().GetObjList())
	{
		if (obj->Intersects(sphereInfo, nullptr))
		{
			obj->OnHit();
			break;
		}
	}
	m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, kRedColor);

}
