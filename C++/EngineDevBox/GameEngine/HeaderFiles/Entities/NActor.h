#pragma once

#ifndef __ENGINE__NACTOR__H__
#define __ENGINE__NACTOR__H__

#include "Utility\NSmartPtr.h"
#include "Entities\NGameObject.h"
#include "Physics\NRigidBody.h"
#include "Collision\NAABBCollider.h"
#include "Renderer\NRenderer.h"
#include "Utility\NHashedString.h"

namespace Illehc
{
	class IController; // Forward Declaration

	class Actor
	{
	public:
		Actor(SmartPtr<GameObject> & i_GameObject, const char * i_pSpriteFileName);
		Actor(
			const char * i_pName, const char * i_pType,
			SmartPtr<GameObject> & i_GameObject,
			const float i_Mass, const float i_DragK,
			const Vector3 i_Center, const Vector3 i_Extents,
			const char * i_pSpriteFileName,
			const bool i_IsSerializable = false);
		bool IsType(const char * i_pType);
		Physics::RigidBody & GetRigidBody();
		void AttachController(IController * i_pContoller);
		void SetPosition(Vector3 i_Position);
		Vector3 GetPosition() const;
		void SetVelocity(Vector3 i_Velocity);
		Vector3 GetVelocity() const;
		bool IsSerializable() const;
		size_t GetSerializableSize() const;
		uint8_t * Serialize(uint8_t * i_pBuffer) const;
		uint8_t * Deserialze(uint8_t * i_pBuffer);
		IController * GetController(void);
		void SetData(const float i_Data);
		float GetData() const;

		~Actor();
	private:
		const char *				m_pName;
		const HashedString			m_pType;
		SmartPtr<GameObject>		m_GameObject;
		Physics::RigidBody			m_RigidBody;
		Physics::AABBCollider		m_Collider;
		Renderer::Renderer			m_Renderer;
		IController *				m_pController;
		bool						m_IsSerializable;
		float						m_Data;
	};
} // namespace Illehc
#endif // !__ENGINE__NACTOR__H__

