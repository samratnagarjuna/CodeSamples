#pragma once

#ifndef __GAME__MAINSCENE__H__
#define __GAME__MAINSCENE__H__

#include "Scene\NAbstractScene.h"
#include "Entities\NActor.h"
#include "Entities\NIController.h"

using namespace Illehc;

class MainScene : public GameScene::AbstractScene 
{
public:
	MainScene();
	void Init();
	void Update(const float i_dt);
	void ShutDown();
	~MainScene();
private:

	IController *			m_pPlayerController;
	IController *			m_pBallController;
	IController *			m_pMonsterController;
	IController *			m_pBulletController;
};

#endif // !__GAME__MAINSCENE__H__
