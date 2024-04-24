#pragma once
#include"../BaseScene/BaseScene.h"

class TitleScene :public BaseScene
{
public:
	TitleScene() { Init(); }
	~TitleScene() { Release(); }

	void Draw() override;
	void Update()override;
	void Init()override;
	void Release()override;

private:



	// title用 // 基本的にキャメル型
	KdTexture m_titleTex;
	KdTexture m_startTex;

	Math::Color m_color;
	float m_alpSp = 0.01f;
	float m_alpha = 0.5f;

};