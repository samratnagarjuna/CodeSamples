#ifndef EAE6320_ICONTROLLER_H
#define EAE6320_ICONTROLLER_H

#include "../Utility/SmartPtr.h"
#include "GameObject.h"

namespace eae6320
{
	class IController
	{
	public:
		virtual void Update(SmartPtr<GameObject> i_pGameObject, const void* i_data) = 0;
	};
}

#endif // !EAE6320_ICONTROLLER_H
