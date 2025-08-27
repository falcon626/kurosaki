#include "Ui.h"

void Ui::Init()
{
	m_tex.Load("Asset/Textures/hp.png");
}

void Ui::Update()
{
	if (GetAsyncKeyState('Z') & 0x8000)
	{
		m_hp--;
		if (m_hp < 0)m_hp = 0;
	}
	if (GetAsyncKeyState('X') & 0x8000)
	{
		m_hp++;
		if (m_hp > 100)m_hp = 100;
	}
}

void Ui::DrawSprite()
{
	// textureの幅を所得
	int w = m_tex.GetWidth();
	// HP1の時に幅をいくつの領域を表示するか
	float singleW = w / 100.0f;
	Math::Rectangle rc = {0,0,static_cast<long>(singleW*m_hp),40};

	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, -600, 290, rc.width,rc.height,&rc,nullptr,{0,0.5f});
}
