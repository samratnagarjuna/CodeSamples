#ifndef EAE6320_ENEMY_CONTROLLER_H
#define EAE6320_ENEMY_CONTROLLER_H

// Header Files
//=============

#include "../../../Engine/Entities/IController.h"

// Class Declaration
//==================

namespace eae6320
{
	class EnemyController : public IController
	{
		float m_attackRange;
		float m_stopRange;
		float m_speed;

	public:
		EnemyController();
		virtual void Update(SmartPtr<GameObject> i_pGameObject,const void* i_data);
		~EnemyController();
	};
}



#endif // !EAE6320_PLAYER_CONTROLLER_H

