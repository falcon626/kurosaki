#include "Tank.h"
#include "../Camera/TPSCamera/TPSCamera.h"
#include "../../main.h"

void Tank::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelData>();
		m_spModel->Load("Asset/Models/tank/tank.gltf");
	}
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
	float moveSpd = 0.05f;
	Math::Vector3 nowPos = m_mWorld.Translation();

	bool moveFlg(false);
	Math::Vector3 moveVec = Math::Vector3::Zero;
	if (GetAsyncKeyState('D')) { moveVec.x = 1.0f; moveFlg = true; }
	if (GetAsyncKeyState('A')) { moveVec.x = -1.0f; moveFlg = true; }
	if (GetAsyncKeyState('W')) { moveVec.z = 1.0f; moveFlg = true; }
	if (GetAsyncKeyState('S')) { moveVec.z = -1.0f; moveFlg = true; }

	if (moveFlg)
	{
		// 正規化
		moveVec.Normalize();
		moveVec *= moveSpd;
		nowPos += moveVec;

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

		m_mWorld = mRot * Math::Matrix::CreateTranslation(nowPos);

		Application::Instance().m_log.AddLog("ang = %.2f m_angle = %.2f \n", ang, m_angle);
	}
}