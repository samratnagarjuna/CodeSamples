#pragma once

#ifndef __ENGINE__NICONTROLLER__H__
#define __ENGINE__NICONTROLLER__H__

#include "Entities\NActor.h"

namespace Illehc
{
	class IController
	{
	public:
		virtual void Update(Actor * i_pActor, const float i_dt) {};
	};
}



#endif // !__ENGINE__NICONTROLLER__H__
