#pragma once

#ifndef __GAME__MONSTERCONTROLLER__H__
#define __GAME__MONSTERCONTROLLER__H__

#include "Entities\NIController.h"
#include "Entities\NActor.h"

using namespace Illehc;

class MonsterController : public IController
{
public:
	MonsterController();
	MonsterController(Actor * i_pActor);
	void Update(Actor * i_pActor, const float i_dt);

private:
	Actor *		m_pActor;
	float		m_Thrust;
};

#endif // !__GAME__MONSTERCONTROLLER__H__

