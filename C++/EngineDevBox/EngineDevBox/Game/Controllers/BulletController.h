#pragma once

#ifndef __GAME__BULLETCONTROLLER__H__
#define __GAME__BULLETCONTROLLER__H__

#include "Entities\NIController.h"
#include "Entities\NActor.h"

using namespace Illehc;

class BulletController : public IController
{
public:
	BulletController();
	void Update(Actor * i_pActor, const float i_dt);

private:
	float m_Time;
	float m_KillTime;
};

#endif // !__GAME__BULLETCONTROLLER__H__

