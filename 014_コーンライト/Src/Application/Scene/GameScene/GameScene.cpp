#include "GameScene.h"
#include"../SceneManager.h"

#include "../../GameObject/Ground/Ground.h"
#include "../../GameObject/Tank/Tank.h"
#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"

void GameScene::Event()
{
}

void GameScene::Init()
{
	KdShaderManager::Instance().WorkAmbientController().SetDirLight({ 1,-1,1 }, {0,0,0});
	KdShaderManager::Instance().WorkAmbientController().SetAmbientLight({ 0.1,0.1,0.1,1 });

	auto ground = std::make_shared<Ground>();
	ground->Init();
	AddObject(ground);

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
