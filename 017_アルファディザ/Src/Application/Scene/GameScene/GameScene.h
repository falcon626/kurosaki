#pragma once

#include"../BaseScene/BaseScene.h"

class GameScene : public BaseScene
{
public :

	GameScene() { Init(); }
	~GameScene() {}

private:

	void Event() override;
	void Init() override;

	std::shared_ptr<KdTexture> m_spTex;
};
