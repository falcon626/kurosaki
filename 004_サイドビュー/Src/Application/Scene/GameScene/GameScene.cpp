#include "GameScene.h"
#include "../SceneManager.h"

#include "../../Object/Stage/Stage.h"
#include "../../Object/Stage/Fly.h"
#include "../../Object/Player/player.h"
#include "../../Object/Back/back.h"
#include "../../Object/Enemy/Enemy.h"
#include "../../Object/Ui/Ui.h"

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

	// 被写界深度	　↓ピントの位置
	m_camera->SetFocus(5, 3, 8);// ←奥のぼかし距離
	//					  ↑手前のぼかし距離
	// ブルーム															↓閾値
	KdShaderManager::Instance().m_postProcessShader.SetBrightThreshold(0.9);
}

void GameScene::Init()
{
	// カメラ
	m_camera = std::make_unique<KdCamera>();

	// 平行光（ディレクショナルライト）								  ↓方向		↓RGB
	KdShaderManager::Instance().WorkAmbientController().SetDirLight({ 0,-1,0.1f }, { 3,3,3 });

	// 環境光（アンビエントライト）										↓RGBA　デフォルト0.3f
	//KdShaderManager::Instance().WorkAmbientController().SetAmbientLight({1, 1, 1, 1});

	// フォグ（霧）
	KdShaderManager::Instance().WorkAmbientController().SetFogEnable(false, true);
	// 距離フォグ														↓色	↓密度
	KdShaderManager::Instance().WorkAmbientController().SetDistanceFog({1,1,1},0.05f);
	// 高さフォグ														↓色　　↓上・下・カメラとの距離
	KdShaderManager::Instance().WorkAmbientController().SetheightFog({ 1,1,1 }, 0, -2, 0);

	// ステージ
	for (size_t i = 0; i < 10; ++i)
	{
		std::shared_ptr<Stage> stage;
		stage = std::make_shared<Stage>();
		m_objList.emplace_back(stage);
		auto fly = std::make_shared<Fly>();
		fly->Init();
		AddObject(fly);
	}
	auto player = std::make_shared<Player>();
	player->Init();
	m_objList.push_back(player);
	m_player = player;

	auto ui = std::make_shared<Ui>();
	ui->Init();
	m_objList.push_back(ui);

	for (size_t i = 0; i < 5; ++i)
	{
		auto enemy = std::make_shared<Enemy>();
		enemy->Init();
		m_objList.push_back(enemy);
	}

	auto back = std::make_shared<Back>();
	m_objList.push_back(back);
}
