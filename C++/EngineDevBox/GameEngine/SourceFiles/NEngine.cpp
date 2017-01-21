#include "NEngine.h"

#include "GLib.h"
#include "Timing\NTiming.h"
#include "Entities\NIController.h"
#include "Input\NInputManager.h"
#include "Message\NMessageManager.h"
#include "Controller\NControllerManager.h"
#include "Scene\NSceneManager.h"
#include "Physics\NPhysicsManager.h"
#include "Renderer\NRenderManager.h"
#include "Threading\NThreadedFileProcessor.h"
#include "Utility\NLuaLoadActor.h"
#include "Utility\NLuaUtility.h"
#include "Utility\NConsolePrint.h"

namespace Illehc
{
	namespace GameEngine
	{
		Engine * Engine::_instance = 0;

		Engine * Engine :: Instance(void)
		{
			if (_instance == 0) {
				_instance = new Engine();
			}
			return _instance;
		}

		bool Init(const char * i_WindowName, HINSTANCE i_hInstance, int i_nCmdShow)
		{
			bool success =  GLib::Initialize(i_hInstance, i_nCmdShow, i_WindowName, -1, 1920, 1080);
			if (success)
			{
				Engine::Instance()->Init();
			}

			return true;
		}

		void LoadLuaActor(const char * i_pFileName)
		{
			Engine::Instance()->LoadLuaActor(i_pFileName);
		}

		void LoadLuaActor(const char * i_pLoadFileName, Vector3 i_Position, Vector3 i_Velocity)
		{
			Engine::Instance()->LoadLuaActor(i_pLoadFileName, i_Position, i_Velocity);
		}

		void SerializeActors(void)
		{
			Engine::Instance()->SerializeActors();
		}

		void DeSerializeActors(void)
		{
			Engine::Instance()->DeSerializeActors();
		}

		void LoadActor(const char * i_pLoadFileName)
		{
			Engine::Instance()->LoadActor(i_pLoadFileName);
		}

		void DeleteActor(Actor * i_pActor)
		{
			Engine::Instance()->DeleteActor(i_pActor);
		}

		void Run(void)
		{
			Engine::Instance()->Run();
		}

		void ShutDown(void)
		{
			Engine::Instance()->ShutDown();
			delete Engine::Instance();
		}

		Engine :: Engine()
		{
			
		}

		void Engine :: Init(void)
		{
			InitAllocators();
			Controller::Init();
			Timing::Init();
			Input::Init();
			MessageSystem::Init();
			GameScene::Init();
			Physics::Init();
			Renderer::Init();
			m_bQuit = false;
		}

		Actor * Engine :: CreateActorFromLua(const char * i_pLoadFileName)
		{
			Actor * actor = NULL;

			// Read in a file
			size_t			i_BufferBytes = 0;
			uint8_t *		i_pBuffer = LoadFile(i_pLoadFileName, i_BufferBytes);

			// Initialize us a lua_State
			lua_State * pLuaState = luaL_newstate();
			assert(pLuaState);

			luaL_openlibs(pLuaState);

			if (i_pBuffer && i_BufferBytes)
			{
				int			result = 0;

				// Do the initial buffer parsing
				result = luaL_loadbuffer(pLuaState, reinterpret_cast<char *>(i_pBuffer), i_BufferBytes, nullptr);
				assert(result == 0);
				lua_pcall(pLuaState, 0, 0, 0);
				assert(result == 0);

				int			type = LUA_TNIL;

				// Find the global variable named "Player" and push it onto stack.
				// If it doesn't find it it pushes a nil value instead
				result = lua_getglobal(pLuaState, "Player");
				assert(result == LUA_TTABLE);

				// Get the Actor name from the Player table
				// 1. Push the key string
				lua_pushstring(pLuaState, "name");
				// 2. Get the value associated with the key
				type = lua_gettable(pLuaState, -2);
				assert(type == LUA_TSTRING);
				const char * pName = lua_tostring(pLuaState, -1);
				assert(pName != NULL);
				if (pName)
					pName = _strdup(pName);

				// 3. Remove the value from the stack now that we're done with it
				lua_pop(pLuaState, 1);

				// Get the Actor Type from the Player table
				// 1. Push the key string
				lua_pushstring(pLuaState, "class");
				// 2. Get the value associated with the key
				type = lua_gettable(pLuaState, -2);
				assert(type == LUA_TSTRING);
				const char * pType = lua_tostring(pLuaState, -1);
				assert(pType != NULL);
				if (pType)
					pType = _strdup(pType);

				// 3. Remove the value from the stack now that we're done with it
				lua_pop(pLuaState, 1);

				// Get the Actor controller from the Player table
				// 1. Push the key string
				lua_pushstring(pLuaState, "controller");
				// 2. Get the value associated with the key
				type = lua_gettable(pLuaState, -2);
				assert((type == LUA_TNIL) || (type == LUA_TSTRING));

				char * pControllerName = NULL;
				if (type == LUA_TSTRING)
				{
					const char * pController = lua_tostring(pLuaState, -1);
					assert(pController != NULL);
					if (pController)
						pControllerName = _strdup(pController);
				}

				// 3. Remove the value from the stack now that we're done with it
				lua_pop(pLuaState, 1);


				// Get the Actor serializable from the Player table
				// 1. Push the key string
				lua_pushstring(pLuaState, "serializable");
				// 2. Get the value associated with the key
				type = lua_gettable(pLuaState, -2);
				assert((type == LUA_TNIL) || (type == LUA_TSTRING));

				bool IsSerializable = false;
				if (type == LUA_TSTRING)
				{
					const char * pSerializable = lua_tostring(pLuaState, -1);
					assert(pSerializable != NULL);
					if (pSerializable)
					{
						IsSerializable = true;
						DEBUG_PRINT("Actor Serialzed");
					}
						
				}

				// 3. Remove the value from the stack now that we're done with it
				lua_pop(pLuaState, 1);

				// get initial_position from the Player table
				lua_pushstring(pLuaState, "initial_position");
				type = lua_gettable(pLuaState, -2);
				assert(type == LUA_TTABLE);

				float		Position[3] = { 0.0f };
				ReadFloatArray(pLuaState, -1, Position, 3);

				// we're done with the "initial_position" table - pop it
				lua_pop(pLuaState, 1);

				// get initial_velocity from the Player table
				lua_pushstring(pLuaState, "initial_velocity");
				type = lua_gettable(pLuaState, -2);
				assert(type == LUA_TTABLE);

				float		Velocity[3] = { 0.0f };
				ReadFloatArray(pLuaState, -1, Velocity, 3);

				// we're done with the "initial_velocty" table - pop it
				lua_pop(pLuaState, 1);

				// get bounding box table
				lua_pushstring(pLuaState, "bounding_box");
				type = lua_gettable(pLuaState, -2);
				assert(type == LUA_TTABLE);

				// get bounding box offset
				lua_pushstring(pLuaState, "offset");
				type = lua_gettable(pLuaState, -2);
				assert(type == LUA_TTABLE);

				float		BoundingBox_Offset[3] = { 0.0f };
				ReadFloatArray(pLuaState, -1, BoundingBox_Offset, 3);

				// we're done with the "offset" table - pop it
				lua_pop(pLuaState, 1);

				// get size from bounding box
				lua_pushstring(pLuaState, "size");
				type = lua_gettable(pLuaState, -2);
				assert(type == LUA_TTABLE);

				float		BoundingBox_Size[3] = { 0.0f };
				ReadFloatArray(pLuaState, -1, BoundingBox_Size, 3);

				// we're done with the "size" table - pop it
				lua_pop(pLuaState, 1);
				// we're done with the "bounding_box" table - pop it
				lua_pop(pLuaState, 1);

				// let's process the "physics_settings" sub table
				lua_pushstring(pLuaState, "physics_settings");
				type = lua_gettable(pLuaState, -2);
				// It either needs to be a table or not there (no physics applied)
				assert((type == LUA_TNIL) || (type == LUA_TTABLE));

				float mass = 0.0f;
				float drag = 0.0f;
				if (type == LUA_TTABLE)
				{
					lua_pushstring(pLuaState, "mass");
					type = lua_gettable(pLuaState, -2);
					assert(type == LUA_TNUMBER);

					lua_pushstring(pLuaState, "drag");
					type = lua_gettable(pLuaState, -3);
					assert(type == LUA_TNUMBER);

					mass = float(lua_tonumber(pLuaState, -2));
					drag = float(lua_tonumber(pLuaState, -1));

					// pop the "mass" and "drag" values together
					lua_pop(pLuaState, 2);
				}

				// we're done with the "physics_settings" table - pop it
				// NOTE: If it didn't exist Lua would have pushed a nil value so still need the pop
				lua_pop(pLuaState, 1);

				// let's process the "render_settings" sub table
				const char * pPath;
				lua_pushstring(pLuaState, "render_settings");
				type = lua_gettable(pLuaState, -2);
				assert((type == LUA_TNIL) || (type == LUA_TTABLE));

				if (type == LUA_TTABLE)
				{
					lua_pushstring(pLuaState, "sprite");
					type = lua_gettable(pLuaState, -2);
					assert(type == LUA_TTABLE);
					{
						//----------------------------------//
						lua_pushstring(pLuaState, "path");
						type = lua_gettable(pLuaState, -2);
						assert(type == LUA_TSTRING);

						pPath = lua_tostring(pLuaState, -1);
						assert(pPath != NULL);
						if (pPath)
							pPath = _strdup(pPath);

						//remove path value from the stack now that we're done with it
						lua_pop(pLuaState, 1);
						//----------------------------------//

						lua_pushstring(pLuaState, "offset");
						type = lua_gettable(pLuaState, -2);
						assert(type == LUA_TTABLE);

						float Sprite_Offset[2] = { 0.0f };
						ReadFloatArray(pLuaState, -1, Sprite_Offset, 2);

						// pop "offset" table
						lua_pop(pLuaState, 1);

						lua_pushstring(pLuaState, "extents");
						type = lua_gettable(pLuaState, -2);
						assert(type == LUA_TTABLE);

						float Sprite_Extents[4] = { 0.0f };
						ReadFloatArray(pLuaState, -1, Sprite_Extents, 4);

						// pop "extents" table
						lua_pop(pLuaState, 1);


					}
					// pop "sprite" table
					lua_pop(pLuaState, 1);

					lua_pushstring(pLuaState, "sprite_color");
					type = lua_gettable(pLuaState, -2);
					if (type == LUA_TTABLE)
					{
						int			SpriteRGBA[4];
						ReadIntArray(pLuaState, -1, SpriteRGBA, 4);
					}

					// pop "sprite_color" table
					lua_pop(pLuaState, 1);
				}

				// pop "render_settings" table
				lua_pop(pLuaState, 1);

				// pop "Player" table
				lua_pop(pLuaState, 1);

				int stack_items = lua_gettop(pLuaState);
				assert(stack_items == 0);

				//Create Actor
				SmartPtr<GameObject> go = new GameObject;

				go->position = Vector3(Position[0], Position[1], Position[2]);
				go->velocity = Vector3(Velocity[0], Velocity[1], Velocity[2]);
				go->orientation = Matrix3::CreateIdentity();

				Vector3 AABBCenter(BoundingBox_Offset[0], BoundingBox_Offset[1], BoundingBox_Offset[2]);
				Vector3 AABBExtents(BoundingBox_Size[0], BoundingBox_Size[1], BoundingBox_Size[2]);

				//Actor * actor = new Actor(go, "Resources\\Sprites\\Pikachu.dds");
				//m_pPlayerController = new PlayerController(m_pActor);

				actor = new Actor(
					pName, pType,
					go,
					mass, drag,
					AABBCenter, AABBExtents, pPath,
					IsSerializable);

				if (pControllerName)
				{
					IController * controller = Controller::GetController(pControllerName);
					actor->AttachController(controller);
				}

				//Register Actor to GameEngine
				GameEngine::Engine::Instance()->AddActor(actor);


				if (pName)
					delete pName;

				if (pPath)
					delete pPath;

				if (pType)
					delete pType;

				if (pControllerName)
					delete pControllerName;

				delete[] i_pBuffer;
			}

			lua_close(pLuaState);

			return actor;
		}

		void Engine :: InitAllocators(void)
		{
			const size_t stringPoolSize = 1024 * 16;
			m_pStringPool = Allocators::StringPool::Create(stringPoolSize);
		}

		void Engine :: LoadActor(const char * i_pFileName)
		{
			using namespace Threading;
			ThreadedFileProcessor &Processor = ThreadedFileProcessor::GetInstance();
			Processor.AddToLoadQueue(*(new LuaLoadActor(i_pFileName)));
		}

		void Engine::LoadLuaActor(const char * i_pLoadFileName)
		{
			Actor * actor = CreateActorFromLua(i_pLoadFileName);
		}

		void Engine::LoadLuaActor(const char * i_pLoadFileName, Vector3 i_Position, Vector3 i_Velocity)
		{
			Actor * actor = CreateActorFromLua(i_pLoadFileName);
			actor->SetPosition(i_Position);
			actor->SetVelocity(i_Velocity);
		}

		void Engine::SerializeActors()
		{
			DEBUG_PRINT("Serializing Actors\n");
			size_t totalSize = 0;
			for (std::list< Actor * >::iterator it = m_ActorsActive.begin(); it != m_ActorsActive.end(); it++)
			{
				if ((*it)->IsSerializable())
				{
					totalSize += (*it)->GetSerializableSize();
				}
			}

			DEBUG_PRINT("Serialize Data Size = %zu\n", totalSize);

			uint8_t * pBuffer = (uint8_t*)malloc( sizeof(uint8_t) * totalSize );
			uint8_t * pStart = pBuffer;

			for (std::list< Actor * >::iterator it = m_ActorsActive.begin(); it != m_ActorsActive.end(); it++)
			{
				if ((*it)->IsSerializable())
				{
					pBuffer = (*it)->Serialize(pBuffer);
				}
			}

			FILE * pFile = NULL;
			fopen_s(&pFile, "Resources\\SavedData\\SavedState.txt", "wb");
			fwrite(pStart, totalSize, 1, pFile);
			fclose(pFile);

			delete pStart;
		}

		void Engine :: DeSerializeActors()
		{
			size_t totalSize = 0;
			uint8_t * pBuffer = LoadFile("Resources\\SavedData\\SavedState.txt", totalSize);
			uint8_t * pStart = pBuffer;
			for (std::list< Actor * >::iterator it = m_ActorsActive.begin(); it != m_ActorsActive.end(); it++)
			{
				if ((*it)->IsSerializable())
				{
					pBuffer = (*it)->Deserialze(pBuffer);
				}
			}

			delete pStart;
		}

		void Engine :: AddActor(Actor * i_Actor)
		{
			m_ActorsActive.push_back(i_Actor);
		}

		void Engine :: DeleteActor(Actor * i_Actor)
		{
			m_ActorsToBeDeleted.push_back(i_Actor);
		}

		void Engine :: UpdateActors(const float i_dt)
		{
			for (std::list< Actor * >::iterator it = m_ActorsActive.begin(); it != m_ActorsActive.end(); it++)
			{
				IController * controller = (*it)->GetController();
				if (controller)
					controller->Update((*it), i_dt);
			}
		}

		void Engine :: Run(void)
		{
			do
			{
				GLib::Service(m_bQuit);

				if (!m_bQuit)
				{
					float dt = Timing::TimeSinceLastFrame();

					DeleteActorsToBeDeleted();
					UpdateActors(dt);

					GameScene::Update(dt);
					Physics::Update(dt);		
					Renderer::Draw();

				}

			} while (m_bQuit == false);
		}

		void Engine :: DeleteActiveActors(void)
		{
			for (std::list< Actor * >::iterator it = m_ActorsActive.begin(); it != m_ActorsActive.end(); it++)
			{
				delete (*it);
			}

			m_ActorsActive.clear();
		}

		void Engine :: DeleteActorsToBeDeleted(void)
		{
			for (std::list< Actor * >::iterator it = m_ActorsToBeDeleted.begin(); it != m_ActorsToBeDeleted.end(); it++)
			{
				m_ActorsActive.remove((*it));
				delete (*it);
			}

			m_ActorsToBeDeleted.clear();
		}

		void Engine :: DeleteAllActors(void)
		{
			DeleteActorsToBeDeleted();
			DeleteActiveActors();
		}

		void Engine :: FreeAllocators(void)
		{
			delete m_pStringPool;
		}

		Engine ::  ~Engine(void)
		{
			
		}

		void Engine :: ShutDown(void)
		{
			DeleteAllActors();

			Timing::ShutDown();
			Input::ShutDown();
			MessageSystem::ShutDown();
			GameScene::ShutDown();
			Physics::ShutDown();
			Renderer::ShutDown();
			Controller::ShutDown();

			Threading::ThreadedFileProcessor::Shutdown();
			FreeAllocators();
		}


	} // namespace GameEngine
} // namespace Illehc