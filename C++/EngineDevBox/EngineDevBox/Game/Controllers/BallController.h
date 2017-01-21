#pragma once

#ifndef __GAME__BALLCONTROLLER__H__

#include "Entities\NIController.h"
#include "Entities\NActor.h"

using namespace Illehc;

class BallController : public IController
{
public:
	BallController();
	void Update(Actor * i_pActor, const float i_dt);
};

#endif // !__GAME__BALLCONTROLLER__H__
