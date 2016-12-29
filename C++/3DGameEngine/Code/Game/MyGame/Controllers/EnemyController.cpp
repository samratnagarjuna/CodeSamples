// Header Files
//=============

#include "EnemyController.h"

#include "../../../Engine/UserInput/UserInput.h"
#include "../../../Engine/Windows/Includes.h"
#include "../../../Engine/Time/Time.h"

// Implementation
//===============

eae6320::EnemyController::EnemyController()
{
	m_attackRange = 6.0f;
	m_stopRange = 1.45f;
	m_speed = 1.5;
}

void eae6320::EnemyController::Update(SmartPtr<GameObject> i_pGameObject,const void* i_data)
{
	float dt = Time::GetElapsedSecondCount_duringPreviousFrame();
	const Math::cVector playerPosition = *(reinterpret_cast<const Math::cVector*>(i_data));

	Math::cVector offset = playerPosition - i_pGameObject->position;
	float offsetLength = offset.GetLength();
	if (offsetLength < m_attackRange && offsetLength > m_stopRange)
	{
		float distance = m_speed * dt;
		Math::cVector displacement = offset.CreateNormalized() * distance;

		i_pGameObject->position += displacement;
	}

}

eae6320::EnemyController::~EnemyController()
{

}