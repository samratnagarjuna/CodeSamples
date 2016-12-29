#ifndef EAE6320_GAMEOBJECT_H
#define EAE6320_GAMEOBJECT_H

#include "../Graphics/Math/cVector.h"
#include "../Graphics/Math/cQuaternion.h"

namespace eae6320
{
	struct GameObject
	{
		Math::cVector		position;
		Math::cVector		velocity;
		Math::cQuaternion	orientation;
	};
} // namespace eae6320

#endif // !EAE6320_GAMEOBJECT_H
