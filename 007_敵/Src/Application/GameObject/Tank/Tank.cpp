#include "Tank.h"
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
	if (GetAsyncKeyState('D')) { moveVec += Math::Vector3::TransformNormal({ 1,0,0 }, camRotYMat); moveFlg = true; }
	if (GetAsyncKeyState('A')) { moveVec += Math::Vector3::TransformNormal({ -1,0,0 }, camRotYMat); moveFlg = true; }
	if (GetAsyncKeyState('W')) { moveVec += Math::Vector3::TransformNormal({ 0,0,1 }, camRotYMat); moveFlg = true; }
	if (GetAsyncKeyState('S')) { moveVec += Math::Vector3::TransformNormal({ 0,0,-1 }, camRotYMat); moveFlg = true; }

	m_isAtk = false;
	if (moveFlg)
	{
		m_isAtk = true;
		// 正規化
		moveVec.Normalize();
		moveVec *= moveSpd;
		m_pos += moveVec;

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

		auto mRot{ Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle)) };

		m_mWorld = mRot * Math::Matrix::CreateTranslation(m_pos);

		Application::Instance().m_log.AddLog("ang = %.2f m_angle = %.2f \n", ang, m_angle);
	}
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
