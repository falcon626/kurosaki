#include "GameScene.h"
#include"../SceneManager.h"

#include "../../GameObject/Ground/Ground.h"
#include "../../GameObject/Tank/Tank.h"
#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include "../../GameObject/Enemy/Moto/Moto.h"
#include "../../GameObject/Enemy/Yama/Yama.h"

void GameScene::Event()
{
}

void GameScene::Init()
{
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

	auto moto = std::make_shared<Moto>();
	moto->SetTarget(tank);
	AddObject(moto);

	auto yama = std::make_shared<Yama>();
	yama->SetTarget(tank);
	AddObject(yama);
}
