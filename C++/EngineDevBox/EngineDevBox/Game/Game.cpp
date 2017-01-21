#include "Game.h"
#include "Scenes\MainScene.h"
#include "Scenes\SerializeScene.h"
#include "Scene\NAbstractScene.h"
#include "Scene\NSceneManager.h"

using namespace Illehc;

namespace Game
{
	GameScene::AbstractScene * mainScene;
	GameScene::AbstractScene * serializeScene;

	bool Init(void)
	{
		serializeScene = new SerializeScene();
		GameScene::SetScene(serializeScene);
		return true;
	}

	void ShutDown(void)
	{
		delete serializeScene;
	}
} // namespace Game