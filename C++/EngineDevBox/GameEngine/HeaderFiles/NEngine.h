#pragma once

#ifndef __ENGINE__NENGINE__H__
#define __ENGINE__NENGINE__H__

#include <Windows.h>
#include <list>

#include "Allocators\NStringPool.h"
#include "Entities\NActor.h"

namespace Illehc
{
	namespace GameEngine
	{
		bool Init(const char * i_WindowName, HINSTANCE i_hInstance, int i_nCmdShow);
		void LoadActor(const char * i_pLoadFileName);
		void LoadLuaActor(const char * i_pLoadFileName);
		void LoadLuaActor(const char * i_pLoadFileName, Vector3 i_Position, Vector3 i_Velocity = Vector3::Zero);
		void SerializeActors(void);
		void DeSerializeActors(void);
		void DeleteActor(Actor * i_pActor);
		void Run(void);
		void ShutDown(void);

		class Engine
		{
		public:
			void Init(void);
			void Run(void);
			void ShutDown(void);
			void LoadActor(const char * i_pLoadFileName);
			void LoadLuaActor(const char * i_pLoadFileName);
			void LoadLuaActor(const char * i_pLoadFileName, Vector3 i_Position, Vector3 i_Velocity = Vector3::Zero);
			void SerializeActors();
			void DeSerializeActors();
			void AddActor(Actor * i_Actor);
			void DeleteActor(Actor * i_Actor);
			void DeleteAllActors(void);
			static Engine * Instance(void);
			~Engine(void);

		private:
			static Engine * _instance;

			Allocators::StringPool * m_pStringPool;

			bool m_bQuit;
			Engine(void);
			Engine(const Engine & i_other) {};
			Engine & operator = (Engine const & i_other) {};

			std::list< Actor *>		m_ActorsActive;
			std::list< Actor *>		m_ActorsToBeDeleted;
			
			Actor * CreateActorFromLua(const char * i_pLoadFileName);
			void InitAllocators(void);
			void FreeAllocators(void);
			void UpdateActors(const float i_dt);
			void DeleteActorsToBeDeleted(void);
			void DeleteActiveActors(void);
		};
	} // namespace GameEngine
} // namespace Illehc

#endif // !__ENGINE__NENGINE__H__

