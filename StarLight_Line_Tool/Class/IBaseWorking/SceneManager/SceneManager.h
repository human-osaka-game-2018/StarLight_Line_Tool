#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <windows.h>

#include <vector>

#include "../../Singleton/Singleton.h"
#include "../IBaseWorking.h"
#include "Scene\Scene.h"
#include "Scene\Enum\SCENE_KIND.h"
#include "Scene\TitleScene\TitleScene.h"
#include "../../../GameLib/GameLib.h"
#include "../../../GameLib/DX/DX3D/CustomVertexEditor/Data/ObjData.h"
#include "../../../GameLib/DX/DX3D/CustomVertexEditor/Data/CustomVertex.h"

class SceneManager :public IBaseWorking, public Singleton<SceneManager>
{
public:
	friend class Singleton<SceneManager>;

	~SceneManager()
	{
		delete m_pScene;
	}

	inline VOID Update()
	{
		if (m_isRequestedChangeResent) return;

		m_pScene->Update();
	}

	inline VOID Render()
	{
		m_pScene->Render();

		StageSceneTransration();
	}

	inline VOID SetCurrentScene(SCENE_KIND currentScene)
	{
		m_currentScene = currentScene;
	}

	inline VOID SetNextScene(SCENE_KIND nextScene)
	{
		m_nextScene = nextScene;

		m_isRequestedChangeResent = TRUE;
	}

	VOID Factory();

private:
	SceneManager() 
	{
		m_pScene = new TitleScene();
	}
	
	VOID StageSceneTransration();

	Scene* m_pScene = nullptr;

	SCENE_KIND m_currentScene	= SK_TITLE;
	SCENE_KIND m_nextScene		= SK_TITLE;

	BOOL m_isRequestedChangeResent = FALSE;
};

#endif // !SCENE_MANAGER_H
