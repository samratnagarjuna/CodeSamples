#include "BallController.h"

#include "NEngine.h"
#include "../Scenes/MainScene.h"
#include "Scene\NAbstractScene.h"
#include "Scene\NSceneManager.h"

BallController :: BallController()
{

}

void BallController::Update(Actor * i_pActor, const float i_dt)
{
	if (i_pActor->GetPosition().getX() > 960)
	{
		GameEngine::DeleteActor(i_pActor);
		GameEngine::LoadLuaActor("Resources\\Actors\\TracerWin.lua");
		// Tracer Win

	}
	else if (i_pActor->GetPosition().getX() < -960)
	{
		GameEngine::DeleteActor(i_pActor);
		GameEngine::LoadLuaActor("Resources\\Actors\\ReaperWin.lua");
		// Reaper Win
	}
}