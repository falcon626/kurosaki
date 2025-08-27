#include "GameScene.h"
#include"../SceneManager.h"

#include "../../GameObject/Ground/Ground.h"
#include "../../GameObject/Grass/Grass.h"
#include "../../GameObject/Tank/Tank.h"
#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"

void GameScene::Event()
{
	static auto ang{ 0 };
	ang += 1;
	if (ang > 360)ang -= 360;
	auto w{ sin(DirectX::XMConvertToRadians(ang)) };

	KdShaderManager::Instance().SetWindowVal({ w, 0, 0 });
}

void GameScene::Init()
{
	KdShaderManager::Instance().SetWindowEnable(true);

	auto ground = std::make_shared<Ground>();
	ground->Init();
	AddObject(ground);
	
	for (auto i{ 0U }; i < 1000; ++i)
	{
		auto siz{ static_cast<float>(rand() % 30 + 10)/10 };

		auto grass = std::make_shared<Grass>(
			Math::Vector3{siz,siz,siz}, 
			rand() % 181, 
			Math::Vector3{ static_cast<float>((rand() % 201) - 100),0,static_cast<float>((rand() % 201) - 100) }
		);
		AddObject(grass);
	}

	auto tank = std::make_shared<Tank>();
	tank->Init();
	AddObject(tank);

	auto camera = std::make_shared<TPSCamera>();
	camera->Init();
	camera->SetTarget(tank);
	tank->SetCameraWeakPtr(camera);
	camera->RegisHitObject(ground);
	AddObject(camera);
}
