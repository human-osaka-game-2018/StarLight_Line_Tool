#ifndef FBX_STORAGE_H
#define FBX_STORAGE_H

#include <windows.h>
#include <tchar.h>

#include <map>

#include <d3dx9.h>

#include "FbxRelated\FbxRelated.h"

class FbxStorage
{
public:
	FbxStorage(LPDIRECT3DDEVICE9& rpDX3DDev) :m_rpDX3DDev(rpDX3DDev) {};
	~FbxStorage() 
	{
		m_pFbxRelatedMap.clear();
	}

	inline VOID CreateFbx(const TCHAR* pKey, const CHAR* pFilePath)
	{
		m_pFbxRelatedMap[pKey] = new FbxRelated(m_rpDX3DDev);
		m_pFbxRelatedMap[pKey]->LoadFbx(pFilePath);
	}

	inline FbxRelated& GetFbx(const TCHAR* pKey)
	{
		return *m_pFbxRelatedMap[pKey];
	}

private:
	LPDIRECT3DDEVICE9& m_rpDX3DDev;

	std::map<const TCHAR*, FbxRelated*> m_pFbxRelatedMap;
};

#endif // !FBX_STORAGE_H
