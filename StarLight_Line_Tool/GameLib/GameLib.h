/**
* @file GameLib.h
* @brief 汎用クラスのFacadeのヘッダ
* @author Harutaka-Tsujino
*/

#ifndef GAME_LIB_H
#define GAME_LIB_H

#include <Windows.h>
#include <tchar.h>

#include <d3dx9.h>

#include "../Class/Singleton/Singleton.h"
#include "Wnd\Wnd.h"
#include "DX\DX.h"
#include "DX\DX3D\CustomVertexEditor\Data\CustomVertex.h"
#include "DX\DX3D\CustomVertexEditor\Data\ObjData.h"

/**
* @brief 汎用クラスのFacade,ウィンドウ生成やDX関係の初期化も行う
*/
class GameLib :public Singleton<GameLib>
{
public:
	friend class Singleton<GameLib>;

	~GameLib()
	{
		delete m_pDX;
		delete m_pWnd;
	};

	/**
	* @brief ウィンドウを生成しDX関係を初期化する,一番初めにこれを呼ぶ
	* @param hInst インスタンスのハンドル
	* @param pAppName アプリケーションの名前のポインタ
	*/
	inline static VOID Create(const HINSTANCE hInst, const TCHAR* pAppName)
	{
		if (m_pWnd) return;
		if (m_pDX) return;

		m_pWnd = new Wnd(hInst, pAppName);

		m_pDX = new DX(m_pWnd->GetHWND(), m_pWnd->GetWndSize());

		GetInstance();
	}

	/**
	* @brief メッセージループを作成し引数で与えられた関数を60fpsで回す
	* @param メッセージループで回す関数のポインタ
	*/
	VOID RunFunc(VOID(*pMainFunc)());

	/**
	* @brief クライアント領域をm_WND_SIZEと同じにする
	*/
	inline VOID ResizeWnd() const
	{
		m_pWnd->ResizeWnd();
	}

	inline RectSize GetWndSize() const
	{
		return m_pWnd->GetWndSize();
	}

	inline VOID ToggleWndMode()								//使用時DX3DDevがロストする可能性がある
	{
		m_pDX->ToggleWndMode();
	}

	inline VOID DefaultBlendMode() const					//通常合成
	{
		m_pDX->DefaultBlendMode();
	}

	inline VOID AddtionBlendMode() const					//加算合成
	{
		m_pDX->AddtionBlendMode();
	}

	inline VOID DefaultColorBlending() const				//ウィンドウモードを切り替えた時には再設定する必要がある
	{
		m_pDX->DefaultColorBlending();
	}

	inline VOID SetLight(const D3DLIGHT9& rLight, DWORD index) const
	{
		m_pDX->SetLight(rLight, index);
	}

	inline VOID OnLight(DWORD index) const
	{
		m_pDX->OnLight(index);
	}

	inline VOID OffLight(DWORD index) const
	{
		m_pDX->OffLight(index);
	}

	inline VOID EnableLighting() const
	{
		m_pDX->EnableLighting();
	}

	inline VOID DisableLighting() const
	{
		m_pDX->DisableLighting();
	}

	inline VOID ChangeAmbientIntensity(DWORD aRGB) const
	{
		m_pDX->ChangeAmbientIntensity(aRGB);
	}

	inline VOID EnableSpecular() const
	{
		m_pDX->EnableSpecular();
	}

	inline VOID DisaableSpecular() const
	{
		m_pDX->DisaableSpecular();
	}

	inline VOID DefaultLighting() const						//ウィンドウモードを切り替えた時には再設定する必要がある
	{
		m_pDX->DefaultLighting();
	}

	inline VOID CreateTex(const TCHAR* pTexKey, const TCHAR* pTexPath)
	{
		m_pDX->CreateTex(pTexKey, pTexPath);
	}

	inline VOID ReleaseTex()
	{
		m_pDX->ReleaseTex();
	}

	inline const LPDIRECT3DTEXTURE9 GetTex(const TCHAR* pTexKey)
	{
		return m_pDX->GetTex(pTexKey);
	}

	inline VOID GetCameraPos(D3DXVECTOR3* pCameraPos) const
	{
		m_pDX->GetCameraPos(pCameraPos);
	}

	inline VOID SetCameraPos(FLOAT x, FLOAT y, FLOAT z)
	{
		m_pDX->SetCameraPos(x, y, z);
	}

	inline VOID SetCameraPos(const D3DXVECTOR3& rCameraPos)
	{
		m_pDX->SetCameraPos(rCameraPos);
	}

	inline VOID GetCameraEyePt(D3DXVECTOR3* pEyePoint) const
	{
		m_pDX->GetCameraPos(pEyePoint);
	}

	inline VOID SetCameraEyePt(FLOAT x, FLOAT y, FLOAT z)
	{
		m_pDX->SetCameraEyePt(x, y, z);
	}

	inline VOID SetCameraEyePt(const D3DXVECTOR3& rEyePt)
	{
		m_pDX->SetCameraEyePt(rEyePt);
	}

	inline VOID GetView(D3DXMATRIX* pView) const
	{
		m_pDX->GetView(pView);
	}

	inline VOID GetProj(D3DXMATRIX* pProj) const
	{
		m_pDX->GetProj(pProj);
	}

	VOID SetCameraTransform()
	{
		m_pDX->SetCameraTransform();
	}

	inline VOID TransBillBoard(D3DXMATRIX* pWorld) const
	{
		m_pDX->TransBillBoard(pWorld);
	}

	inline VOID RotateRectXYZ(CustomVertex* pCustomVertices, const D3DXVECTOR3& rDeg, const D3DXVECTOR3& rRelativeRotateCenter) const
	{
		m_pDX->RotateRectXYZ(pCustomVertices, rDeg, rRelativeRotateCenter);
	}

	inline VOID RotateRectX(CustomVertex* pCustomVertices, FLOAT deg, const D3DXVECTOR3& rRelativeRotateCenter) const
	{
		m_pDX->RotateRectX(pCustomVertices, deg, rRelativeRotateCenter);
	}

	inline VOID RotateRectY(CustomVertex* pCustomVertices, FLOAT deg, const D3DXVECTOR3& rRelativeRotateCenter) const
	{
		m_pDX->RotateRectY(pCustomVertices, deg, rRelativeRotateCenter);
	}

	inline VOID RotateRectZ(CustomVertex* pCustomVertices, FLOAT deg, const D3DXVECTOR3& rRelativeRotateCenter) const
	{
		m_pDX->RotateRectZ(pCustomVertices, deg, rRelativeRotateCenter);
	}

	inline VOID RescaleRect(CustomVertex* pCustomVertices, const D3DXVECTOR2& rScaleRate) const
	{
		m_pDX->RescaleRect(pCustomVertices, rScaleRate);
	}

	inline VOID MoveRect(CustomVertex* pCustomVertices, const D3DXVECTOR3& rMovement) const
	{
		m_pDX->MoveRect(pCustomVertices, rMovement);
	}

	inline VOID LocaleRect(CustomVertex* pCustomVertices, const D3DXVECTOR3& rPos) const
	{
		m_pDX->LocaleRect(pCustomVertices, rPos);
	}

	inline VOID SetRectTexUV(CustomVertex* pCustomVertices,
		FLOAT startTU = 0.0f, FLOAT startTV = 0.0f, FLOAT endTU = 1.0f, FLOAT endTV = 1.0f) const
	{
		m_pDX->SetRectTexUV(pCustomVertices, startTU, startTV, endTU, endTV);
	}

	inline VOID SetRectARGB(CustomVertex *pCustomVertices, DWORD aRGB) const
	{
		m_pDX->SetRectARGB(pCustomVertices, aRGB);
	}

	inline VOID CreateRect(CustomVertex *pCustomVertices, const D3DXVECTOR3& rCenter, const D3DXVECTOR3& rHalfScale,
		DWORD aRGB = 0xFFFFFFFF, FLOAT startTU = 0.0f, FLOAT startTV = 0.0f, FLOAT endTU = 1.0f, FLOAT endTV = 1.0f) const
	{
		m_pDX->CreateRect(pCustomVertices, rCenter, rHalfScale, aRGB, startTU, startTV, endTU, endTV);
	}

	inline VOID CreateRect(CustomVertex *pCustomVertices, const ObjData& rObjData) const
	{
		m_pDX->CreateRect(pCustomVertices, rObjData);
	}

	inline VOID Render(const FbxRelated& rFBXModel, const D3DXMATRIX& rWorld, const LPDIRECT3DTEXTURE9 pTexture = nullptr) const
	{
		m_pDX->Render(rFBXModel, rWorld, pTexture);
	}

	inline VOID Render(const CustomVertex* pCustomVertices, const LPDIRECT3DTEXTURE9 pTexture = nullptr) const
	{
		m_pDX->Render(pCustomVertices, pTexture);
	}

	inline VOID CreateFbx(const TCHAR* pKey, const CHAR* pFilePath)
	{
		m_pDX->CreateFbx(pKey, pFilePath);
	}

	inline FbxRelated& GetFbx(const TCHAR* pKey)
	{
		return m_pDX->GetFbx(pKey);
	}

	inline BOOL MouseIsPressed(INT key) const
	{
		return m_pDX->MouseIsPressed(key);
	}

	inline BOOL MouseIsHeld(INT key) const
	{
		return m_pDX->MouseIsHeld(key);
	}

	inline BOOL MouseIsReleased(INT key) const
	{
		return m_pDX->MouseIsReleased(key);
	}

	inline BOOL MouseIsNeutral(INT key) const
	{
		return m_pDX->MouseIsNeutral(key);
	}

	inline BOOL MouseAnyKeyIsPressed() const
	{
		return m_pDX->MouseAnyKeyIsPressed();
	}

	inline VOID CursorPos(POINT* pPos) const
	{
		m_pDX->CursorPos(pPos);
	}

	inline LONG MouseWheelScrollingFromPrev() const
	{
		return m_pDX->MouseWheelScrollingFromPrev();
	}

	inline BOOL KeyboardIsPressed(INT key) const
	{
		return m_pDX->KeyboardIsPressed(key);
	}

	inline BOOL KeyboardIsHeld(INT key) const
	{
		return m_pDX->KeyboardIsHeld(key);
	}

	inline BOOL KeyboardIsReleased(INT key) const
	{
		return m_pDX->KeyboardIsReleased(key);
	}

	inline BOOL KeyboardIsNeutral(INT key) const
	{
		return m_pDX->KeyboardIsNeutral(key);
	}

	inline BOOL KeyboardAnyKeyIsPressed() const
	{
		return m_pDX->KeyboardAnyKeyIsPressed();
	}

private:
	GameLib() {};

	static Wnd* m_pWnd;

	static DX* m_pDX;
};

#endif //! GAME_LIB_H
