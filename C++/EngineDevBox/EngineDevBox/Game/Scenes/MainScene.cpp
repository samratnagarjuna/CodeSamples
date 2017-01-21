#include "MainScene.h"

#include "Entities\NActor.h"
#include "Entities\NGameObject.h"
#include "Utility\NSmartPtr.h"
#include "Utility\NConsolePrint.h"
#include "Collision\NCollisionManager.h"
#include "Controller\NControllerManager.h"
#include "NEngine.h"

#include "../Controllers/PlayerController.h"
#include "../Controllers/BallController.h"
#include "../Controllers/MonsterController.h"
#include "../Controllers/BulletController.h"

MainScene :: MainScene()
{

}

void MainScene :: Init()
{	
	m_pPlayerController = new PlayerController();
	m_pBallController = new BallController();
	m_pMonsterController = new MonsterController();
	m_pBulletController = new BulletController();

	Controller::RegisterController("PlayerController", m_pPlayerController);
	Controller::RegisterController("BallController", m_pBallController);
	Controller::RegisterController("MonsterController", m_pMonsterController);
	Controller::RegisterController("BulletController", m_pBulletController);

	GameEngine::LoadLuaActor("Resources\\Actors\\Player.lua");
	GameEngine::LoadLuaActor("Resources\\Actors\\Monster.lua");

	GameEngine::LoadLuaActor("Resources\\Actors\\Ball.lua");
}

void MainScene :: Update(const float i_dt)
{
	
}

void MainScene::ShutDown()
{
	Controller::DeRegisterController("PlayerController", m_pPlayerController);
	Controller::DeRegisterController("BallController", m_pBallController);
	Controller::DeRegisterController("MonsterController", m_pMonsterController);
	Controller::DeRegisterController("BulletController", m_pBulletController);

	delete m_pPlayerController;
	delete m_pBallController;
	delete m_pMonsterController;
	delete m_pBulletController;
}

MainScene :: ~MainScene()
{
	if (m_pPlayerController)
		delete m_pPlayerController;

	if (m_pBallController)
		delete m_pBallController;

	if (m_pMonsterController)
		delete m_pMonsterController;

	if (m_pBulletController)
		delete m_pBulletController;
}

