#ifndef SCENE_H
#define SCENE_H

#include <windows.h>

#include "../../IBaseWorking.h"
#include "ObjManager\ObjManager.h"

class Scene :public IBaseWorking
{
public:
	Scene()
	{
		m_pObjManager = new ObjManager();
	}

	~Scene()
	{
		delete m_pObjManager;
	}

	inline VOID Update()
	{
		m_pObjManager->Update();
	}

	inline VOID Render()
	{
		m_pObjManager->Render();
	}

protected:
	ObjManager* m_pObjManager = nullptr;
};

#endif // !SCENE_H
