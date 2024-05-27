#include "GameScene.h"
#include "../SceneManager.h"

#include "../../Object/Stage/Stage.h"
#include "../../Object/Player/player.h"

void GameScene::Event()
{
	if (GetAsyncKeyState('T') & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Title
		);
	}
	// プレイヤー
	// ①プレイヤー情報をObjListからとってくる

	// ②プレイヤーは最初からメンバー変数でもっておく
	// ②を採用、参照カウンタ２
	// 参照カウンタを１にするにはweakポインタを使う

	// プレイヤー座標を所得
	// m_playerが保持しているアドレス先がまだ有効か？
	Math::Vector3 playerPos;
	if (m_player.expired() == false)
	{
		playerPos = m_player.lock()->GetPos();
	}

	// カメラの移動
	Math::Matrix transMat;
	Math::Vector3 cameraPos = { 3,1.5,-5 };
	transMat = Math::Matrix::CreateTranslation(cameraPos + playerPos);
	m_camera->SetCameraMatrix(transMat);
}

void GameScene::Init()
{
	// カメラ
	m_camera = std::make_unique<KdCamera>();

	// ステージ
	std::shared_ptr<Stage> stage;
	stage = std::make_shared<Stage>();
	m_objList.emplace_back(stage);

	auto player = std::make_shared<Player>();
	player->Init();
	m_objList.push_back(player);
	m_player = player;
}
