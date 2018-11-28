#ifndef SCENE_OBJ_MANAGER_H
#define SCENE_OBJ_MANAGER_H

#include <windows.h>

#include <vector>
#include <algorithm>
#include <functional>

#include "../../../../Singleton/Singleton.h"
#include "../../../IBaseWorking.h"
#include "Obj\Obj.h"

class ObjManager :public Singleton<ObjManager>, public IBaseWorking
{
public:
	ObjManager() {};
	~ObjManager()
	{
		Release();
	}

	inline VOID Resister(Obj* pSceneObj)
	{
		m_pSceneObjVec.push_back(pSceneObj);
	}

	inline VOID Release()
	{
		for (Obj* pSceneObj : m_pSceneObjVec)
		{
			delete pSceneObj;
		}

		m_pSceneObjVec.clear();
		m_pSceneObjVec.shrink_to_fit();
	}

	inline VOID Update()
	{
		for (Obj* pSceneObj : m_pSceneObjVec)
		{
			pSceneObj->Update();
		}
	}

	inline VOID Render()
	{
		std::vector<Obj*>pFarDescendingVec = m_pSceneObjVec;	//遠い順に描画しないとアルファブレンドがうまくいかない場合がある
		std::sort(
				pFarDescendingVec.begin(), pFarDescendingVec.end(), 
				IsFarther);

		for (Obj* pSceneObj : pFarDescendingVec)
		{
			pSceneObj->Render();
		}
	}

private:
	static BOOL IsFarther(const Obj* pCompares, const Obj* pCompared)
	{
		return (pCompares->GetZ() > pCompared->GetZ());
	}

	std::vector<Obj*>m_pSceneObjVec;
};

#endif // !SCENE_OBJ_MANAGER_H
