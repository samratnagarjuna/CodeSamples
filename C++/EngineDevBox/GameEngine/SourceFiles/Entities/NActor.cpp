#include "Entities\NActor.h"

#include <cassert>
#include <cstring>

#include "Utility\NConsolePrint.h"

namespace Illehc
{
	Actor :: Actor(SmartPtr<GameObject>& i_GameObject, const char * i_pSpriteFileName) :
		m_pName(NULL),
		m_pType(),
		m_GameObject(i_GameObject),
		m_RigidBody(i_GameObject, 1.0f, 0.005f),
		m_Collider(i_GameObject, Vector3::Zero, Vector3(30.0f, 30.0f, 20.0f)),
		m_Renderer(i_GameObject, i_pSpriteFileName),
		m_pController(NULL),
		m_Data(0.0f)
	{
		m_RigidBody.AddCollider(&m_Collider);
	}

	Actor::Actor(
		const char * i_pName, const char * i_pType,
		SmartPtr<GameObject>& i_GameObject,
		const float i_Mass, const float i_DragK,
		const Vector3 i_Center, const Vector3 i_Extents,
		const char * i_pSpriteFileName,
		const  bool i_IsSerializable) :
		m_pName(_strdup(i_pName)),
		m_pType(i_pType),
		m_GameObject(i_GameObject),
		m_RigidBody(i_GameObject, i_Mass, i_DragK),
		m_Collider(i_GameObject, i_Center, i_Extents),
		m_Renderer(i_GameObject, i_pSpriteFileName),
		m_pController(NULL),
		m_IsSerializable(i_IsSerializable),
		m_Data(0.0f)
	{
		m_RigidBody.AddCollider(&m_Collider);
	}

	void Actor :: AttachController(IController * i_pController)
	{
		m_pController = i_pController;
	}

	IController * Actor :: GetController(void)
	{
		return m_pController;
	}

	void Actor::SetData(const float i_Data)
	{
		m_Data = i_Data;
	}

	float Actor :: GetData() const
	{
		return m_Data;
	}

	void Actor :: SetPosition(Vector3 i_Position)
	{
		m_GameObject->position = i_Position;
	}

	Vector3 Actor::GetPosition() const
	{
		return m_GameObject->position;
	}

	void Actor :: SetVelocity(Vector3 i_Velocity)
	{
		m_GameObject->velocity = i_Velocity;
	}

	Vector3 Actor::GetVelocity() const
	{
		return m_GameObject->velocity;
	}

	bool Actor::IsSerializable() const
	{
		return m_IsSerializable;
	}

	size_t Actor::GetSerializableSize() const
	{
		size_t size = 0;

		size += sizeof(size_t);
		size += (strlen(m_pName) + 1) * sizeof(char);
		size += sizeof(m_GameObject->position);

		return size;
	}

	uint8_t * Actor :: Serialize(uint8_t * i_pBuffer) const
	{
		//serialize name length
		size_t * pSize = reinterpret_cast<size_t*>(i_pBuffer);
		size_t length = strlen(m_pName);
		(*pSize) = length;
		DEBUG_PRINT("Serialized Length %zu\n", length);
		i_pBuffer += sizeof(size_t);

		//serialize name
		char * pName = reinterpret_cast<char *>(i_pBuffer);
		strcpy_s(pName, (length + 1) * sizeof(char), m_pName);
		DEBUG_PRINT("Serialized Name %s\n", pName);
		i_pBuffer += (length + 1) * sizeof(char);

		//serialize position
		Vector3 * pVPosition = reinterpret_cast<Vector3 *>(i_pBuffer);
		*(pVPosition) = m_GameObject->position;

		i_pBuffer += sizeof(m_GameObject->position);
		return i_pBuffer;

	}

	uint8_t * Actor::Deserialze(uint8_t * i_pBuffer)
	{
		size_t * pSize = reinterpret_cast<size_t*>(i_pBuffer);
		size_t length = strlen(m_pName);
		i_pBuffer += sizeof(size_t);

		char * pName = reinterpret_cast<char *>(i_pBuffer);
		i_pBuffer += (length + 1) * sizeof(char);

		Vector3 * pVPosition = reinterpret_cast<Vector3 *>(i_pBuffer);
		m_GameObject->position = (*pVPosition);

		i_pBuffer += sizeof(m_GameObject->position);
		return i_pBuffer;

	}

	bool Actor :: IsType(const char * i_pType)
	{
		assert(i_pType);
		return m_pType == i_pType;
	}

	Physics::RigidBody & Actor :: GetRigidBody()
	{
		return m_RigidBody;
	}

	Actor :: ~Actor()
	{
		if (m_pName)
			free(const_cast<char *>(m_pName));
	}
} // namespace Illehc

