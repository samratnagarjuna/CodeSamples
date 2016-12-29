// Header Files
//=============

#include "cMyGame.h"

#include "../../Engine/Graphics/Graphics.h"
#include "../../Engine/Graphics/MeshRenderer.h"
#include "../../Engine/UserInput/UserInput.h"
#include "../../Engine/Time/Time.h"
#include "../../Engine/Graphics/Math/cVector.h"
#include "../../Engine/Graphics/Math/cQuaternion.h"
#include "../../Engine/Graphics/Math/Functions.h"
#include "../../Engine/UserSettings/UserSettings.h"

// Interface
//==========

// Initialization / Clean Up
//--------------------------

eae6320::cMyGame::~cMyGame()
{
	delete m_pPlaneMeshRenderer;
	delete m_pPlayerMeshRenderer;
	delete m_pEnemyMeshRenderer;

	delete m_pCopyRightSprite;
}

// Inherited Implementation
//=========================

// Initialization / Clean Up
//--------------------------

bool eae6320::cMyGame::Initialize()
{
	// Create a Camera
	{
		Math::cQuaternion isometricAngle = Math::cQuaternion(Math::ConvertDegreesToRadians(53.0f), Math::cVector(1.0f, 0.0f, 0.0f)) * Math::cQuaternion(Math::ConvertDegreesToRadians(-45.0f), Math::cVector(0.0f, 1.0f, 0.0f));
		Math::cVector isometricPosition = Math::cVector(15.0f, 20.0f, 10.0f);

		float aspectRatio = (UserSettings::GetResolutionWidth() * 1.0f) / UserSettings::GetResolutionHeight();
		m_pCamera = new Graphics::Camera(
			isometricPosition,
			isometricAngle,
			Math::ConvertDegreesToRadians(60.0f),
			0.1f,
			100.0f,
			aspectRatio
		);
		Graphics::SetCamera(m_pCamera);
	}

	// Create materials files
	{
		m_pStandardMaterial = new Graphics::cMaterial();
		m_pStandardMaterial->Load("data/Materials/standard.material");

		m_pOscillateMaterial = new Graphics::cMaterial();
		m_pOscillateMaterial->Load("data/Materials/oscillate.material");

		m_pFloorMaterial = new Graphics::cMaterial();
		m_pFloorMaterial->Load("data/Materials/floor.material");

		m_pShipMaterial = new Graphics::cMaterial();
		m_pShipMaterial->Load("data/Materials/ship.material");

		m_pCopyRightMaterial = new Graphics::cMaterial();
		m_pCopyRightMaterial->Load("data/Materials/copyright.material");
	}
	

	m_pPlaneRenderInfo = new Graphics::RenderInfo();
	{
		// Initialize Game Object
		{
			m_pPlaneObject = new GameObject;
			m_pPlaneObject->position = Math::cVector(0.0f, -1.0f, 0.0);
			m_pPlaneObject->orientation = Math::cQuaternion();
		}

		// Initialize Mesh Renderer
		{
			m_pPlaneMeshRenderer = new Graphics::MeshRenderer();
			const char * const path = "data/Meshes/Floor.blml";
			m_pPlaneMeshRenderer->Load(path);
		}

		// Populate render info
		m_pPlaneRenderInfo->pMaterial = m_pFloorMaterial;
		m_pPlaneRenderInfo->pMeshRenderer = m_pPlaneMeshRenderer;
		m_pPlaneRenderInfo->pGameObject = m_pPlaneObject;
	}

	m_pPlayerRenderInfo = new Graphics::RenderInfo();
	{
		// Initialize Game Object
		{
			m_pPlayerObject = new GameObject;
			m_pPlayerObject->position = Math::cVector(-9.0f, 0.0f, -3.0f);
			m_pPlayerObject->orientation = Math::cQuaternion();
		}

		// Initialize Mesh Renderer
		{
			m_pPlayerMeshRenderer = new Graphics::MeshRenderer();
			const char * const path = "data/Meshes/Ship.blml";
			m_pPlayerMeshRenderer->Load(path);
		}

		// Populate render info
		m_pPlayerRenderInfo->pMaterial = m_pShipMaterial;
		m_pPlayerRenderInfo->pMeshRenderer = m_pPlayerMeshRenderer;
		m_pPlayerRenderInfo->pGameObject = m_pPlayerObject;
	}

	m_pEnemyRenderInfo = new Graphics::RenderInfo();
	{
		// Initialize Game Object
		{
			m_pEnemyObject = new GameObject;
			m_pEnemyObject->position = Math::cVector(5.0f, 0.0f, -8.0);
		}

		// Initialize Mesh Renderer
		{
			m_pEnemyMeshRenderer = new Graphics::MeshRenderer();
			const char * const path = "data/Meshes/Enemy.blml";
			m_pEnemyMeshRenderer->Load(path);
		}

		// Populate render info
		{
			m_pEnemyRenderInfo->pMaterial = m_pStandardMaterial;
			m_pEnemyRenderInfo->pMeshRenderer = m_pEnemyMeshRenderer;
			m_pEnemyRenderInfo->pGameObject = m_pEnemyObject;
		}
	}

	// Initialize Controllers
	{
		m_pPlayerController = new PlayerController();
		m_pEnemyController = new EnemyController();
	}

	m_pCopyRightSpriteInfo = new Graphics::SpriteInfo();
	{
		m_pCopyRightSprite = new Graphics::cSprite(
			0.5f, 0.75f, -0.25f, -0.50f,
			0.0f, 1.0f, 1.0f, 0.0f
		);

		m_pCopyRightSpriteInfo->pSpriteRenderer = m_pCopyRightSprite;
		m_pCopyRightSpriteInfo->pMaterial = m_pCopyRightMaterial;
	}

	return true;
}

bool eae6320::cMyGame::CleanUp()
{
	// Clean Mesh Renderer
	m_pStandardMaterial->CleanUp();
	delete m_pStandardMaterial;

	m_pOscillateMaterial->CleanUp();
	delete m_pOscillateMaterial;

	m_pFloorMaterial->CleanUp();
	delete m_pFloorMaterial;

	m_pShipMaterial->CleanUp();
	delete m_pShipMaterial;

	m_pCopyRightMaterial->CleanUp();
	delete m_pCopyRightMaterial;

	delete m_pPlaneRenderInfo;
	m_pPlaneMeshRenderer->CleanUp();

	delete m_pPlayerRenderInfo;
	m_pPlayerMeshRenderer->CleanUp();

	delete m_pEnemyRenderInfo;
	m_pEnemyMeshRenderer->CleanUp();

	delete m_pCopyRightSpriteInfo;
	m_pCopyRightSprite->CleanUp();

	delete m_pCamera;
	return true;
}

// Load Render Data
//=================

void eae6320::cMyGame::UpdateFrameData()
{
	// Update GameObjects
	{
		m_pPlayerController->Update(m_pPlayerObject, NULL);
		m_pEnemyController->Update(m_pEnemyObject, &m_pPlayerObject->position);
	}

	// Submit RenderInfo to graphics
	{
		eae6320::Graphics::LoadFrameData(m_pPlayerRenderInfo);
		eae6320::Graphics::LoadFrameData(m_pPlaneRenderInfo);
		eae6320::Graphics::LoadFrameData(m_pEnemyRenderInfo);
	}

	{
		eae6320::Graphics::LoadSpriteData(m_pCopyRightSpriteInfo);
	}
}