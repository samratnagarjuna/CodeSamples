#pragma once

#ifndef __GAME__SERIALIZESCENE__H__
#define __GAME__SERIALIZESCENE__H__

#include "Scene\NAbstractScene.h"
#include "Entities\NIController.h"

using namespace Illehc;

class SerializeScene : public GameScene::AbstractScene
{
public:
	SerializeScene();
	void Init();
	void Update(const float i_dt);
	void ShutDown();
	~SerializeScene();

private:
	IController *			m_pPlayerController;
	IController *			m_pMonsterController;
};

#endif // !__GAME__SERIALIZESCENE__H__
