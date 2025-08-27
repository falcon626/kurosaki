#include "back.h"

void Back::Init()
{
	m_polygon.SetMaterial("Asset/Textures/backGround.png");
	SetScale({300.0f, 100.0f, 1});
	m_mWorld *= Math::Matrix::CreateTranslation(0,0,50);
}

void Back::DrawUnLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(m_polygon, m_mWorld);
}
