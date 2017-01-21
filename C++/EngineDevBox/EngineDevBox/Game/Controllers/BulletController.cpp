#include "BulletController.h"

#include "NEngine.h"
#include "Utility\NConsolePrint.h"

BulletController::BulletController() : m_Time(0.0f), m_KillTime(2000.0f)
{

}

void BulletController::Update(Actor * i_pActor, const float i_dt)
{
	int currTime = i_pActor->GetData();
	currTime += i_dt;
	if (currTime > m_KillTime)
		GameEngine::DeleteActor(i_pActor);
	else
		i_pActor->SetData(currTime);
}