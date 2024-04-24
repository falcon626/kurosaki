#pragma once
#include"../BaseScene/BaseScene.h"

class GameScene :public BaseScene
{
public:
	GameScene(){ Init(); }
	~GameScene() { Release(); }

	void Draw() override;
	void Update() override;
	void Init() override;
	void Release() override;

private:


	KdTexture m_playerTex;
	Math::Vector3 m_pos;
};