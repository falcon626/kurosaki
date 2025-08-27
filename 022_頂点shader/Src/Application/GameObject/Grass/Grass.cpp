#include "Grass.h"

void Grass::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void Grass::DrawLit()
{
	KdShaderManager::Instance().ChangeRasterizerState(KdRasterizerState::CullNone);
	KdShaderManager::Instance().m_StandardShader.WindowEnable(true);
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
	KdShaderManager::Instance().m_StandardShader.WindowEnable(false);
	KdShaderManager::Instance().ChangeRasterizerState(KdRasterizerState::CullBack);
}