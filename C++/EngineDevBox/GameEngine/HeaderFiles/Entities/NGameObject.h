#pragma once

#ifndef __ENGINE__NGAMEOBJECT__H__
#define __ENGINE__NGAMEOBJECT__H__

#include "Math\NVector3.h"
#include "Math\NMatrix3.h"

namespace Illehc
{
	typedef struct GameObject
	{
		Vector3		position;
		Vector3		velocity;
		Matrix3		orientation;
	} GameObject;
} // namespace Illehc

#endif // !__ENGINE__NGAMEOBJECT__H__
