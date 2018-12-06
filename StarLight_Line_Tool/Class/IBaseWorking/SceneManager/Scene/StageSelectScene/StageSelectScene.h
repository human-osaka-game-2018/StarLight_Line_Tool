#ifndef STAGE_SELECT_SCENE_H
#define STAGE_SELECT_SCENE_H

#include <windows.h>

#include "../Scene.h"
#include "../ObjManager/Obj/Obj.h"
#include "../ObjManager/Obj/StageSelectSceneObj/StageSelectSceneObj.h"

class StageSelectScene :public Scene
{
public:
	StageSelectScene()
	{
		m_pObjManager->Resister(static_cast<Obj*>(new StageSelectBack()));
		m_pObjManager->Resister(static_cast<Obj*>(new StageSelectSceneStages()));
	}

	~StageSelectScene() {};
};

#endif // !STAGE_SELECT_SCENE_H
