#ifndef EAE6320_PLAYER_CONTROLLER_H
#define EAE6320_PLAYER_CONTROLLER_H

// Header Files
//=============

#include "../../../Engine/Entities/IController.h"

// Class Declaration
//==================

namespace eae6320
{
	class PlayerController : public IController
	{
	private:

		float	m_yOffset;
		bool	m_isGoingUp = true;
		float	m_currentYOffset;
		float	m_ospeed;

	public:
		PlayerController();
		virtual void Update(SmartPtr<GameObject> i_pGameObject,const void* i_data);
		~PlayerController();
	};
}



#endif // !EAE6320_PLAYER_CONTROLLER_H

