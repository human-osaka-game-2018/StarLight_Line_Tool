/// <summary>
/// セーブデータを読み込むシーンクラスのヘッダ
/// </summary>

#ifndef SAVE_DATA_SCENE_H
#define SAVE_DATA_SCENE_H

#include <windows.h>

#include "../Scene.h"
#include "../ObjManager/Obj/Obj.h"
#include "../ObjManager/Obj/SaveDataSceneObj/SaveDataSceneObj.h"

class SaveDataScene :public Scene
{
public:
	SaveDataScene()
	{
		m_pObjManager->Resister(static_cast<Obj*>(new SaveDataBack()));
		m_pObjManager->Resister(static_cast<Obj*>(new SaveDataSaveDatas()));
	}

	~SaveDataScene() {};
};

#endif //! SAVE_DATA_SCENE_H
