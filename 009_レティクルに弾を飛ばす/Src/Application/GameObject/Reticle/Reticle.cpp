#include "Reticle.h"

Reticle::Reticle(const std::string& path)
{
	m_spTex = std::make_shared<KdTexture>(path);
}

void Reticle::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex,0,0);
}
