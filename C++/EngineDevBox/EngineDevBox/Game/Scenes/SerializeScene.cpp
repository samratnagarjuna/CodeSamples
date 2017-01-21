#include "SerializeScene.h"

#include "Input\NInputManager.h"
#include "Controller\NControllerManager.h"
#include "NEngine.h"

#include "../Controllers/PlayerController.h"
#include "../Controllers/MonsterController.h"

SerializeScene::SerializeScene()
{

}

void SerializeScene::Init()
{
	m_pPlayerController = new PlayerController();
	m_pMonsterController = new MonsterController();

	Controller::RegisterController("PlayerController", m_pPlayerController);
	Controller::RegisterController("MonsterController", m_pMonsterController);

	GameEngine::LoadLuaActor("Resources\\Actors\\Player.lua");
	GameEngine::LoadLuaActor("Resources\\Actors\\Monster.lua");
}

void SerializeScene::Update(const float i_dt)
{
	if (Input::IsKeyDown('B'))
	{
		GameEngine::SerializeActors();
	}

	if (Input::IsKeyDown('L'))
	{
		GameEngine::DeSerializeActors();
	}
}

void SerializeScene::ShutDown()
{
	Controller::DeRegisterController("PlayerController", m_pPlayerController);
	Controller::DeRegisterController("MonsterController", m_pMonsterController);
	delete m_pPlayerController;
	delete m_pMonsterController;
}

SerializeScene::~SerializeScene()
{
	if (m_pPlayerController)
		delete m_pPlayerController;

	if (m_pMonsterController)
		delete m_pMonsterController;
}
