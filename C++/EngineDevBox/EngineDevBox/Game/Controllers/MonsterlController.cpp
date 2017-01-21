#include "MonsterController.h"

#include "Input\NInputManager.h"
#include "Physics\NRigidBody.h"

#include "NEngine.h"

using namespace Illehc;

MonsterController::MonsterController() : m_Thrust(0.005f)
{

}

MonsterController::MonsterController(Actor * i_pActor) : m_Thrust(0.005f)
{

}

void MonsterController::Update(Actor * i_pActor, const float i_dt)
{

	if (i_pActor->GetPosition().getX() > 960)
	{
		GameEngine::LoadLuaActor("Resources\\Actors\\TracerWin.lua");
	}

	if (Input::IsKeyDown('I'))
	{
		i_pActor->GetRigidBody().ApplyForce(Vector3(0.0f, m_Thrust, 0.0f));
	}

	if (Input::IsKeyDown('K'))
	{
		i_pActor->GetRigidBody().ApplyForce(Vector3(0.0f, -m_Thrust, 0.0f));
	}

	i_pActor->SetData(i_pActor->GetData() + i_dt);
	if (Input::IsKeyDown('U') && i_pActor->GetData() > 1000.0f)
	{
		Vector3 offset = Vector3(-144.0f, 0.0f, 0.0f);
		Vector3 velocity = Vector3(-2.0f, 0.0f, 0.0f);
		GameEngine::LoadLuaActor("Resources\\Actors\\BulletL.lua", i_pActor->GetPosition() + offset, velocity);
		i_pActor->SetData(0.0f);
	}
}
