// Header Files
//=============

#include "PlayerController.h"

#include "../../../Engine/UserInput/UserInput.h"
#include "../../../Engine/Windows/Includes.h"
#include "../../../Engine/Time/Time.h"

// Implementation
//===============

eae6320::PlayerController::PlayerController()
{
	m_currentYOffset = 0.0f;
	m_yOffset = 0.20f;
	m_isGoingUp = true;
	m_currentYOffset = 0.0f;
	m_ospeed = 1.8f;
}

void eae6320::PlayerController::Update(SmartPtr<GameObject> i_pGameObject,const void* i_data)
{
	float dt = Time::GetElapsedSecondCount_duringPreviousFrame();

	float offset[3] = { 0.0f, 0.0f, 0.0f };
	{
		if (eae6320::UserInput::IsKeyPressed(VK_UP))
			offset[2] -= 1.0f;
		if (eae6320::UserInput::IsKeyPressed(VK_DOWN))
			offset[2] += 1.0f;
		if (eae6320::UserInput::IsKeyPressed(VK_LEFT))
			offset[0] -= 1.0f;
		if (eae6320::UserInput::IsKeyPressed(VK_RIGHT))
			offset[0] += 1.0f;
	}
	{
		const float speed_unitsPerSecond = 2.0f;
		const float offsetModifier = speed_unitsPerSecond * dt;
		offset[0] *= offsetModifier;
		offset[1] *= offsetModifier;
		offset[2] *= offsetModifier;
	}

	float yOffset = 0;
	// Handling oscillations through shader
	/*{
		if (m_currentYOffset > m_yOffset) {
			m_isGoingUp = false;
		}
		else if (m_currentYOffset < -m_yOffset)
		{
			m_isGoingUp = true;
		}

		yOffset = (m_isGoingUp ? m_ospeed : -m_ospeed) * dt;
		if (yOffset < m_yOffset)
			m_currentYOffset += yOffset;
	}*/

	i_pGameObject->position += Math::cVector(offset[0], yOffset, offset[2]);
}

eae6320::PlayerController::~PlayerController()
{

}