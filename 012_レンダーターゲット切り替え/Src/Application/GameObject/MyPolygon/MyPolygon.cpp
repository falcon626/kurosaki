#include "MyPolygon.h"
#include "../../Scene/SceneManager.h"

MyPolygon::MyPolygon()
{
	m_spPol = std::make_shared<KdSquarePolygon>();
	SetPos({ 0,25,0 });
	SetScale({ 50,50,1 });
}

void MyPolygon::DrawUnLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPol, m_mWorld);
}

void MyPolygon::Update()
{
	m_spPol->SetMaterial(SceneManager::Instance().GetRenderTex());
}
