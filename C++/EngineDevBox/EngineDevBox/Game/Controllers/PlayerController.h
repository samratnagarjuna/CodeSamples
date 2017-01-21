#pragma once

#ifndef __GAME__PLAYERCONTROLLER__H__
#define __GAME__PLAYERCONTROLLER__H__

#include "Entities\NIController.h"
#include "Entities\NActor.h"

using namespace Illehc;

class PlayerController : public IController
{
public:
	PlayerController();
	PlayerController(Actor * i_pActor);
	void Update(Actor * i_pActor, const float i_dt);

private:
	Actor *		m_pActor;
	float		m_Thrust;
};

#endif // !__GAME__PLAYERCONTROLLER__H__

