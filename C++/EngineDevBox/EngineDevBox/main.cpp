#include <Windows.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include "NEngine.h"
#include "Message\NMessageManager.h"
#include "Math\NMatrix4.h"
#include "Math\NVector4.h"
#include "Utility\NHashedString.h"
#include "Utility\NConsolePrint.h"
#include "Game\Game.h"

#if defined _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#endif // _DEBUG

using namespace Illehc;

class TestMessageHandler : public MessageSystem::IMessageHandler
{
	void OnReceive(const HashedString & i_Message, void * i_Data);
};

void TestMessageHandler :: OnReceive(const HashedString & i_Message, void * i_Data)
{
	DEBUG_PRINT("Test Message Handler Triggered\n");
}

void MessageHandlerUnitTest()
{
	MessageSystem::Init();
	MessageSystem::IMessageHandler * handler = new TestMessageHandler();
	MessageSystem::RegisterMessageHandler("Test", handler);
	MessageSystem::NotifyMessage("Test", NULL);
	MessageSystem::DeRegisterMessageHandler("Test", handler);
	MessageSystem::ShutDown();
	delete handler;
}

void MatrixUnitTest()
{
	Vector4 invector = Vector4 (1.0f, 0.0f, 0.0f, 1.0f);
	
	Matrix4 transmatrix = Matrix4::CreateTranslationCV(10.0f, 0.0f, 0.0f);
	Vector4 outvector = transmatrix.MultiplyRight(invector);
	DEBUG_PRINT("Expected Output = 11.0 , Produced Output = %f\n",outvector.getX());

	Matrix4 rotmatrix = Matrix4::CreateZRotationCV((float)M_PI_2);
	outvector = rotmatrix.MultiplyRight(invector);
	DEBUG_PRINT("Expected Output = 1.0 , Produced Output = %f\n", outvector.getY());

	Matrix4 transrotmatrix = transmatrix * rotmatrix;
	outvector = transrotmatrix.MultiplyRight(invector);
	DEBUG_PRINT("Expected Output = (10.0, 1.0) , Produced Output = (%f, %f)\n", outvector.getX(), outvector.getY());
}

int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
{
	/* 
	  NOTE::
	  When loading Serialize Scene Press B (BackUp) to Serialize Data 
	  and L(Load) to Deserialize 
	*/

	if (GameEngine::Init("Engine Illehc Dev Box", i_hInstance, i_nCmdShow))
	{
		if (Game::Init())
		{
			GameEngine::Run();
			Game::ShutDown();
		}
		GameEngine::ShutDown();
	}

#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG
	return 0;
}