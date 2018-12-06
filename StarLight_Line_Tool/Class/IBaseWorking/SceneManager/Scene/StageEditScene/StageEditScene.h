#ifndef STAGE_EDIT_SCENE_H
#define STAGE_EDIT_SCENE_H

#include <windows.h>

#include "../Scene.h"
#include "../ObjManager/Obj/Obj.h"
#include "../ObjManager/Obj/StageEditSceneObj/StageEditSceneObj.h"

class StageEditScene :public Scene
{
public:
	StageEditScene()
	{
		m_pObjManager->Resister(static_cast<Obj*>(new StageEditStageEditor()));
	}

	~StageEditScene() {};
};

#endif //! STAGE_EDIT_SCENE_H
